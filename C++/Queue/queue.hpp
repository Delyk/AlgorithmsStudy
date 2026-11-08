#pragma once
//Очередь, стековая очередь, дек, очередь с приоритетом, очередь Майкла-Скотта,
//персистентная очередь

#include <initializer_list>
#include <memory>
#include <stdexcept>
namespace queues {

// FIFO - first in, first out (первый пришёл - первый ушёл)
template <typename T> class queue {
protected:
  struct element {
    T data;
    std::shared_ptr<element> next;
    std::shared_ptr<element> prev;
    element(T data = T{}, std::shared_ptr<element> next = nullptr,
            std::shared_ptr<element> prev = nullptr)
        : data(data), next(next), prev(prev) {}
    void clear() { data = T{}; }
    ~element() { this->clear(); }
    operator bool() const { return next || prev; }
  };

  std::shared_ptr<element> head;
  std::shared_ptr<element> tail;

public:
  queue();
  queue(std::initializer_list<T>);
  queue(const queue &);
  queue(queue &&) noexcept;
  queue &operator=(std::initializer_list<T>);
  queue &operator=(const queue &);
  queue &operator=(queue &&);

  void enqueue(T);
  T dequeue();
  T front() const;
  void clear();
  bool empty() const;
};

//Дек
template <typename T> class deque : public queue<T> {
public:
  deque();
  deque(std::initializer_list<T>);
  deque(const deque<T> &);
  deque(deque &&) noexcept;
  deque &operator=(std::initializer_list<T>);
  deque &operator=(const deque &);
  deque &operator=(deque &&);

  void enqueue_front(T);
  T dequeue_back();
  T back() const;
};

//Очередь на стеках
template <typename T> class queue_stack {
  template <typename E> class stack {
    struct element {
      T data;
      std::shared_ptr<element> prev;
      element(T data = T{}, std::shared_ptr<element> current = nullptr)
          : data(data), prev(current) {}
      void clear() {
        prev = nullptr;
        data = T{};
      }
      ~element() { this->clear(); }
    };
    std::shared_ptr<element> head;

  public:
    stack();
    void push(T);
    T pop();
    T front() const;
    bool empty() const;
    void clear();
    stack<E> &operator=(const stack<E> &);
  };
  stack<T> right;
  stack<T> left;

  void refillStacks();

public:
  queue_stack();
  queue_stack(std::initializer_list<T>);
  queue_stack(const queue_stack<T> &);
  queue_stack(queue_stack<T> &&);
  queue_stack &operator=(std::initializer_list<T>);
  queue_stack &operator=(const queue_stack<T> &);
  queue_stack &operator=(queue_stack<T> &&);
  void enqueue(T);
  T dequeue();
  T front();
  bool empty() const;
  void clear();
};

/*** Методы простой очереди ***/
//Конструкторы
template <typename T> queue<T>::queue() : head(), tail() {}

template <typename T> queue<T>::queue(std::initializer_list<T> list) {
  for (auto i : list) {
    this->enqueue(i);
  }
}

template <typename T> queue<T>::queue(const queue<T> &right) {
  head = right.head;
  tail = right.tail;
}

template <typename T> queue<T>::queue(queue<T> &&right) noexcept {
  try {
    while (right.head) {
      this->enqueue(right.dequeue());
    }
  } catch (std::runtime_error &e) {
  }
  right.clear();
}

//Операторы присваивания
template <typename T>
queue<T> &queue<T>::operator=(std::initializer_list<T> list) {
  this->clear();
  for (auto i : list) {
    this->enqueue(i);
  }
  return *this;
}

template <typename T> queue<T> &queue<T>::operator=(const queue<T> &right) {
  this->clear();
  head = right.head;
  tail = right.tail;
  return *this;
}

template <typename T> queue<T> &queue<T>::operator=(queue<T> &&right) {
  try {
    while (right.head) {
      this->enqueue(right.dequeue());
    }
  } catch (std::runtime_error &e) {
  }
  right.clear();
  return *this;
}

//Добавить элемент в очередь
template <typename T> void queue<T>::enqueue(T data) {
  element new_el(data);
  new_el.next =
      tail; //Создаётся новый хвост и текущий хвост считается как следующий
  tail = std::make_shared<element>(new_el);
  if (tail->next) {
    tail->next->prev =
        tail; //У предыдущего хвоста устанавлием текущий хвост как предыдущий
  }
  if (!head) {
    head = tail; //Если головы нет, то хвост и голова одинаковы
  } else {
    if (!head->prev) { //Если предыдущий элемент у головы пуст
      head->prev = tail; //Устанавливаем предыдущий в хвост
      tail->next = head; //У хвоста следующий - голова
    }
  }
}

//Убрать элемент из очереди
template <typename T> T queue<T>::dequeue() {
  T data = T{};
  if (head) {
    data = head->data;
    std::shared_ptr<element> old_head = head;
    head = head->prev; //Устанавливаем текущую голову как предыдущую
    if (head) { //Если голова не пуста, то следущий элемент обнуляем
      head->next = nullptr;
    } else {
      tail = nullptr; //Иначе полностью обнуляем очередь
    }
    old_head->clear(); //Очищаем старую голову
  } else {
    throw std::runtime_error("Empty queue");
  }
  return data;
}

//Получить первый элемент очереди
template <typename T> T queue<T>::front() const {
  if (head) {
    return head->data;
  } else {
    throw std::runtime_error("Empty queue");
  }
}

//Очистить очередь
template <typename T> void queue<T>::clear() {
  while (head) {
    std::shared_ptr<element> prev = head->prev;
    head->clear();
    head = prev;
  }
  tail = nullptr;
}

//Проверка на пустоту
template <typename T> bool queue<T>::empty() const {
  return head.get() == nullptr ? true : false;
}

/*** Методы дека ***/
//Конструкторы
template <typename T> deque<T>::deque() : queue<T>() {}

template <typename T>
deque<T>::deque(std::initializer_list<T> list) : queue<T>(list) {}

template <typename T>
deque<T>::deque(const deque<T> &right) : queue<T>(right) {}

template <typename T>
deque<T>::deque(deque<T> &&right) noexcept : queue<T>(right) {}

//Операторы присваивания

template <typename T>
deque<T> &deque<T>::operator=(std::initializer_list<T> list) {
  this->clear();
  for (auto i : list) {
    this->enqueue(i);
  }
  return *this;
}

template <typename T> deque<T> &deque<T>::operator=(const deque<T> &right) {
  this->clear();
  this->head = right.head;
  this->tail = right.tail;
  return *this;
}

template <typename T> deque<T> &deque<T>::operator=(deque<T> &&right) {
  this->clear();
  try {
    while (right.head) {
      this->enqueue(right.dequeue());
    }
  } catch (std::runtime_error &e) {
  }
  return *this;
}

//Добавить в голову очереди
template <typename T> void deque<T>::enqueue_front(T data) {
  std::shared_ptr<typename queue<T>::element> add =
      std::make_shared<typename queue<T>::element>(data, nullptr,
                                                   queue<T>::head);
  if (this->head) {
    this->head->next = add;
  }
  this->head = add;
  if (!this->tail) {
    this->tail = this->head;
  }
}

//Убрать из хвоста дека
template <typename T> T deque<T>::dequeue_back() {
  if (this->tail) {
    T data = this->tail->data;
    std::shared_ptr<typename queue<T>::element> old_tail = this->tail;
    if (this->tail->next) {
      this->tail->next->prev = nullptr;
    }
    this->tail = this->tail->next;
    old_tail->clear();
    return data;
  } else {
    throw std::runtime_error("Empty deque");
  }
}

//Последний элемент дека
template <typename T> T deque<T>::back() const {
  if (this->tail) {
    return this->tail->data;
  } else {
    throw std::runtime_error("Empty deque");
  }
}

/*** Методы стековой очереди ***/
/*** Стек ***/

//Конструктор
template <typename T>
template <typename E>
queue_stack<T>::stack<E>::stack() : head(nullptr) {}

//Оператор присваивания
template <typename T>
template <typename E>
typename queue_stack<T>::template stack<E> &
queue_stack<T>::stack<E>::operator=(const stack<E> &right) {
  this->clear();
  if (right.head) {
    this->head = std::make_shared<element>(right.head->data, nullptr);
    std::shared_ptr<element> cursor = right.head->prev;
    std::shared_ptr<element> lcursor = this->head;
    while (cursor) {
      lcursor->prev = std::make_shared<element>(cursor->data, nullptr);
      cursor = cursor->prev;
      lcursor = lcursor->prev;
    }
  }
  return *this;
}

// push
template <typename T>
template <typename E>
void queue_stack<T>::stack<E>::push(T data) {
  std::shared_ptr<element> add = std::make_shared<element>(data, head);
  head = add;
}

// pop
template <typename T> template <typename E> T queue_stack<T>::stack<E>::pop() {
  T data = T{};
  if (head) {
    data = head->data;
    std::shared_ptr<element> old_head = head;
    head = head->prev;
    old_head->clear();
  } else {
    throw std::runtime_error("Empty stack");
  }
  return data;
}

//Верхний элемент очереди
template <typename T>
template <typename E>
T queue_stack<T>::stack<E>::front() const {
  if (head.get() != nullptr) {
    return head->data;
  } else {
    throw std::runtime_error("Empty stack");
  }
}

//Очистка стека
template <typename T>
template <typename E>
void queue_stack<T>::stack<E>::clear() {
  while (head) {
    this->pop();
  }
}

//Проверка на пустоту
template <typename T>
template <typename E>
bool queue_stack<T>::stack<E>::empty() const {
  return head.get() == nullptr ? true : false;
}
/*** Стековая очередь ***/
//Конструкторы
template <typename T> queue_stack<T>::queue_stack() {
  left = stack<T>();
  right = stack<T>();
}

template <typename T>
queue_stack<T>::queue_stack(std::initializer_list<T> list) {
  for (auto i : list) {
    left.push(i);
  }
}

template <typename T> queue_stack<T>::queue_stack(const queue_stack<T> &right) {
  this->left = right.left;
  this->right = right.right;
}

template <typename T> queue_stack<T>::queue_stack(queue_stack<T> &&right) {
  this->left = right.left;
  this->right = right.right;
  right.clear();
}

//Операторы присваивания
template <typename T>
queue_stack<T> &queue_stack<T>::operator=(std::initializer_list<T> list) {
  this->clear();
  for (auto i : list) {
    left.push(i);
  }
  return *this;
}

template <typename T>
queue_stack<T> &queue_stack<T>::operator=(const queue_stack<T> &right) {
  this->clear();
  this->left = right.left;
  this->right = right.right;
  return *this;
}

template <typename T>
queue_stack<T> &queue_stack<T>::operator=(queue_stack<T> &&right) {
  this->clear();
  this->left = right.left;
  this->right = right.right;
  right.clear();
  return *this;
}

//Перенести из левого стека в правый
template <typename T> void queue_stack<T>::refillStacks() {
  while (!left.empty()) {
    right.push(left.pop()); //Если очередь пуста, то освобождает левый стек и
                            //перемещаем если элементы в обратном порядке в
                            //правый стек
  }
}

//Добавить в очередь
template <typename T> void queue_stack<T>::enqueue(T data) {
  left.push(data); //При push просто помещаем элемент в левый стек
}

//Убрать из очереди
template <typename T> T queue_stack<T>::dequeue() {
  //При попытке извлечения элемента из очереди
  if (right.empty()) {
    refillStacks();
  }
  if (right.empty()) {
    throw std::runtime_error("Empty queue");
  }
  return right.pop();
}

//Получить первый элемент очереди
template <typename T> T queue_stack<T>::front() {
  if (right.empty()) {
    refillStacks();
  }
  if (right.empty()) {
    throw std::runtime_error("Empty queue");
  }
  return right.front();
}

//Проверка на пустоту
template <typename T> bool queue_stack<T>::empty() const {
  return left.empty() && right.empty();
}

template <typename T> void queue_stack<T>::clear() {
  left.clear();
  right.clear();
}

} // namespace queues
