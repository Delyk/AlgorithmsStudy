#pragma once
//Очередь, стековая очередь, дек, очередь с приоритетом

#include <initializer_list>
#include <memory>
#include <stdexcept>
namespace queues {

// FIFO - first in, first out (первый пришёл - первый ушёл)
template <typename T> class queue {
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
  new_el.next = tail;
  tail = std::make_shared<element>(new_el);
  if (tail->next) {
    tail->next->prev = tail;
  }
  if (!head) {
    head = tail;
  } else {
    if (!head->prev) {
      head->prev = tail;
      tail->next = head;
    }
  }
}

//Убрать элемент из очереди
template <typename T> T queue<T>::dequeue() {
  T data = T{};
  if (head) {
    data = head->data;
    std::shared_ptr<element> old_head = head;
    head = head->prev;
    if (head) {
      head->next = nullptr;
    } else {
      tail = nullptr;
    }
    old_head->clear();
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

} // namespace queues
