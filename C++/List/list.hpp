#pragma once
// список любых значений, XOR-список, список с пропусками
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
namespace list {
using t_size = unsigned int;

template <typename T> class list {
protected:
  struct baseNode { // Структура одного элемента
    virtual ~baseNode() = default;
    virtual void clear() = 0;
    T data;
  };
  t_size size; // Размер

public:
  virtual ~list() = default;
  virtual void push(const T &) = 0;    // Вставить в конец списка
  virtual T pop() = 0;                 // Убрать из конца списка
  virtual void unshift(const T &) = 0; // Вставить в начало списка
  virtual T shift() = 0;               // Убрать из начала списка
  virtual void sort() = 0;             // Сортировать список
  virtual t_size getSize() const = 0;  // Получить размер списка
  virtual bool empty() const = 0;      // Проверка на пустоту
  virtual void clear() = 0;            // Очистить список
};

template <typename T> class single_list : public list<T> {
  struct node : public list<T>::baseNode {
    std::shared_ptr<node> next;
    node(const T &);
    void clear() override;
  };

  // Итератор
  struct Iterator {
    // Теги для алгоритмов
    using iterator_category = std::forward_iterator_tag; // Тип итератора
    using difference_type = std::ptrdiff_t;              // Шаг итерации
    using value_type = T;                                // Тип итерации
    using pointer = node *; // Указатель над итератором
    using reference = T &;  // Ссылка на тип
    Iterator(pointer);
    reference operator*() const; // Получить значение итератора
    pointer operator->();        // Получить Указатель на тип
    // Инкременты
    Iterator &operator++();   // Префиксный
    Iterator operator++(int); // Постфиксный
                              // Операторы сравнения
    friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
      return lhs.m_ptr == rhs.m_ptr;
    }

    friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
      return !(lhs == rhs);
    }

  private:
    pointer m_ptr;
  };

  std::shared_ptr<node> head;

protected:
  std::shared_ptr<node> merge(single_list<T> &left, single_list<T> &right);
  std::shared_ptr<node> getMiddle(std::shared_ptr<node>);

public:
  // Конструкторы
  single_list();
  single_list(const T &);
  single_list(const single_list &);
  single_list(single_list &&) noexcept;
  single_list(std::initializer_list<T>);
  single_list(std::shared_ptr<node>);
  // Операторы присваивания
  single_list &operator=(const single_list &);
  single_list &operator=(single_list &&) noexcept;
  single_list &operator=(std::initializer_list<T>);
  // Добавление/удаление элементов
  void push(const T &) override;
  T pop() override;
  void unshift(const T &) override;
  T shift() override;
  // Вспомогательные методы
  void sort() override;
  t_size getSize() const override;
  bool empty() const override;
  void clear() override;
  // Методы доступа к итератору
  Iterator begin() const;
  Iterator end() const;
};

// Двусвязный список
template <typename T> class double_list : public list<T> {
  struct node : list<T>::baseNode {
    std::shared_ptr<node> next;
    std::shared_ptr<node> prev;
    void clear() override;
    node(const T &);
  };

  struct Iterator {
    // Теги для алгоритмов
    using iterator_category = std::bidirectional_iterator_tag; // Тип итератора
    using difference_type = std::ptrdiff_t;                    // Шаг итерации
    using value_type = T;                                      // Тип итерации
    using pointer = double_list<T>::node *; // Указатель над итератором
    using reference = T &;                  // Ссылка на тип
    Iterator(pointer);
    reference operator*() const; // Получить значение итератора
    pointer operator->();        // Получить Указатель на тип
    // Инкременты
    Iterator &operator++();   // Префиксный
    Iterator operator++(int); // Постфиксный
                              // Декремент
    Iterator &operator--();   // Префиксный
    Iterator operator--(int); // Постфиксный

    // Операторы сравнения
    friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
      return lhs.m_ptr == rhs.m_ptr;
    }

    friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
      return !(lhs == rhs);
    }

  private:
    pointer m_ptr;
  };

  std::shared_ptr<double_list<T>::node> head;
  std::shared_ptr<double_list<T>::node> tail;
  std::shared_ptr<node> getMiddle(std::shared_ptr<node>);
  std::shared_ptr<node> merge(double_list<T> &, double_list<T> &);

public:
  // Конструкторы
  double_list();
  double_list(const T &);
  double_list(std::shared_ptr<node>);
  double_list(const double_list &);
  double_list(double_list &&) noexcept;
  double_list(std::initializer_list<T>);
  // Присваивание
  double_list<T> &operator=(const double_list<T> &);
  double_list<T> &operator=(double_list<T> &&) noexcept;
  double_list<T> &operator=(std::initializer_list<T>);
  // Добавление/удаление элементов
  void push(const T &) override;
  T pop() override;
  void unshift(const T &) override;
  T shift() override;
  // Вспомогательные методы
  void sort() override;
  bool empty() const override;
  void clear() override;
  t_size getSize() const override;
  // Методы доступа к итератору
  Iterator begin() const;
  Iterator end() const;
};

// Методы односвязного списка
// Конструктор элемента
template <typename T>
single_list<T>::node::node(const T &value) : next(nullptr) {
  this->data = value;
}

template <typename T> void single_list<T>::node::clear() {
  next = nullptr;
  list<T>::baseNode::data = T{};
}

// Конструкторы
template <typename T> single_list<T>::single_list() : head(nullptr) {
  this->size = 0;
}

template <typename T>
single_list<T>::single_list(std::shared_ptr<typename single_list<T>::node> head)
    : head(head) {}

template <typename T>
single_list<T>::single_list(const T &value)
    : head(std::make_shared<node>(value)) {
  this->size = 1;
}

template <typename T> single_list<T>::single_list(const single_list &right) {
  for (auto i : right) {
    this->push(i);
  }
  list<T>::size = right.list<T>::size;
}

template <typename T>
single_list<T>::single_list(single_list &&right) noexcept {
  for (auto i : right) {
    this->push(i);
  }
  list<T>::size = right.list<T>::size;
  right.clear();
}

template <typename T>
single_list<T>::single_list(std::initializer_list<T> init) {
  for (auto i : init) {
    this->push(i);
  }
  list<T>::size = init.size();
}

// Присваивание
template <typename T>
single_list<T> &single_list<T>::operator=(const single_list &right) {
  if (&right != this) {
    this->clear();
    for (auto i : right) {
      this->push(i);
    }
    list<T>::size = right.list<T>::size;
  }
  return *this;
}

template <typename T>
single_list<T> &single_list<T>::operator=(single_list &&right) noexcept {
  if (&right != this) {
    this->clear();
    for (auto i : right) {
      this->push(i);
    }
    list<T>::size = right.list<T>::size;
    right.clear();
  }
  return *this;
}

template <typename T>
single_list<T> &single_list<T>::operator=(std::initializer_list<T> init) {
  this->clear();
  for (auto i : init) {
    this->push(i);
  }
  list<T>::size = init.size();
  return *this;
}

// Добавить в конец
template <typename T> void single_list<T>::push(const T &value) {
  auto new_node = std::make_shared<node>(value);
  if (head) {
    auto cursor = head;
    while (cursor->next != nullptr) {
      cursor = cursor->next;
    }
    cursor->next = new_node;
  } else {
    head = new_node;
  }
  list<T>::size++;
}
// Удалить из конца
template <typename T> T single_list<T>::pop() {
  T tmp{};
  if (head) {
    if (!head->next) {
      tmp = head->data;
      head->clear();
      head = nullptr;
      list<T>::size--;
      return tmp;
    }
    std::shared_ptr<node> cursor = head;
    while (cursor->next->next != nullptr) {
      cursor = cursor->next;
    }
    auto prev = cursor->next;
    tmp = prev->list<T>::baseNode::data;
    prev->clear();
    cursor->next = nullptr;
    list<T>::size--;
  } else {
    throw std::range_error("List empty");
  }

  return tmp;
}
// Добавить в начало
template <typename T> void single_list<T>::unshift(const T &value) {
  std::shared_ptr<node> first = std::make_shared<node>(value);
  if (head) {
    first->next = head;
  }
  head = first;
  list<T>::size++;
}
// Удалить из начала
template <typename T> T single_list<T>::shift() {
  if (!head) {
    throw std::range_error("List empty\n");
  }
  T head_data = head->list<T>::baseNode::data;
  std::shared_ptr<node> tmp = head;
  head = head->next;
  tmp->clear();
  list<T>::size--;
  return head_data;
}

// Сортировка

// Слияние двух списков
/*
 * Параметры:
 * - left  — первый отсортированный список,
 * - right — второй отсортированный список.
 * 1. Создается пустой результирующий список `result`.
 * 2. Заведены два указателя (`left_cursor` и `right_cursor`), которые проходят
 * по элементам списков left и right соответственно.
 * 3. Пока есть элементы и в левом, и в правом списках:
 *    - Сравниваем текущие элементы.
 *    - Добавляем меньший элемент в конец результирующего списка, передвигаем
 * соответствующий курсор вперёд.
 */
template <typename T>
std::shared_ptr<typename single_list<T>::node>
single_list<T>::merge(single_list<T> &left, single_list<T> &right) {
  single_list<T> result;
  std::shared_ptr<node> left_cursor = left.head;
  std::shared_ptr<node> right_cursor = right.head;
  std::shared_ptr<node> *result_cursor = &result.head;
  while (left_cursor && right_cursor) {
    if (left_cursor->data <= right_cursor->data) {
      *result_cursor = left_cursor;
      left_cursor = left_cursor->next;
    } else {
      *result_cursor = right_cursor;
      right_cursor = right_cursor->next;
    }
    result_cursor = &((*result_cursor)->next);
  }
  return result.head;
}

/* Поиск середины
 * Один указатель перемещается вперёд на 1 элемент
 * Второй указатель перепрыгивает через один и перещается на 2 элемента вперёд
 *за шаг Таким образом, когда "быстрый" указатель дойдёт до конца списка -
 *"медленный" указатель будет только на середине потому что он в 2 раза
 *медленнее
 */
template <typename T>
std::shared_ptr<typename single_list<T>::node>
single_list<T>::getMiddle(std::shared_ptr<typename single_list<T>::node> head) {
  if (!head) {
    return head;
  }

  std::shared_ptr<node> slow = head;
  std::shared_ptr<node> fast = head->next;

  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }

  return slow;
}

/*
 * Сортировка списка методом "разделяй и властвуй" (merge sort).
 * 1. Если список пустой или содержит один элемент, он уже отсортирован — выход
 * из функции.
 * 2. Ищется середина списка (узел середины).
 * 3. Список разбивается на два подсписка:
 *    - левый — от головы до середины,
 *    - правый — от узла после середины до конца.
 * 4. Рекурсивно сортируются левый и правый подсписки.
 * 5. Отсортированные подсписки объединяются в один отсортированный список с
 * помощью функции merge().
 *
 * Таким образом, список рекурсивно разбивается на всё меньшие части,
 * пока не останутся списки с одним элементом,
 * затем происходит их последовательное слияние с сортировкой,
 * разворачивая рекурсию и формируя итоговый отсортированный список.
 */
template <typename T> void single_list<T>::sort() {

  if (!head || !head->next) {
    return;
  }

  std::shared_ptr<node> mid = getMiddle(head);
  std::shared_ptr<node> middleNext = mid->next;
  mid->next = nullptr;

  single_list<T> left{head};
  single_list<T> right{middleNext};

  left.sort();
  right.sort();

  this->head = merge(left, right);
}

// Геттер
template <typename T> t_size single_list<T>::getSize() const {
  return list<T>::size;
}

// Проверка на пустоту
template <typename T> bool single_list<T>::empty() const {
  return head == nullptr;
}

// Очистить список

template <typename T> void single_list<T>::clear() {
  if (head) {
    auto next = head;
    auto cursor = head;
    while (cursor != nullptr) {
      next = cursor->next;
      cursor->clear();
      cursor = next;
    }
  }
  head = nullptr;
  this->size = 0;
}

// Итератор односвязного списка
// Конструктор
template <typename T>
single_list<T>::Iterator::Iterator(single_list<T>::Iterator::pointer ptr)
    : m_ptr(ptr) {}

// Разыменование
template <typename T>
typename single_list<T>::Iterator::reference
single_list<T>::Iterator::operator*() const {
  return m_ptr->data;
}

// Получить указатель
template <typename T>
typename single_list<T>::Iterator::pointer
single_list<T>::Iterator::operator->() {
  return m_ptr;
}

// Инкременты
template <typename T>
typename single_list<T>::Iterator &single_list<T>::Iterator::operator++() {
  if (m_ptr) {
    m_ptr = m_ptr->next.get();
  }
  return *this;
}

template <typename T>
typename single_list<T>::Iterator single_list<T>::Iterator::operator++(int) {
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}

// Доступ к итератору
template <typename T>
typename single_list<T>::Iterator single_list<T>::begin() const {
  if (head) {
    return Iterator(head.get());
  }
  return Iterator(nullptr);
}

template <typename T>
typename single_list<T>::Iterator single_list<T>::end() const {
  return Iterator(nullptr);
}

// Методы двусвязного списка
// Конструктор ноды
template <typename T>
double_list<T>::node::node(const T &value) : next(nullptr), prev(nullptr) {
  this->data = value;
}

template <typename T> void double_list<T>::node::clear() {
  this->next = nullptr;
  this->prev = nullptr;
  this->data = T{};
}

// Конструкторы
template <typename T>
double_list<T>::double_list() : head(nullptr), tail(nullptr) {
  this->size = 0;
}

template <typename T>
double_list<T>::double_list(std::shared_ptr<node> head)
    : head(head), tail(nullptr) {
  this->size = 0;
  auto cursor = head;
  std::shared_ptr<node> prev_node = nullptr;
  while (cursor != nullptr) {
    this->size++;
    cursor->prev = prev_node;
    prev_node = cursor;
    cursor = cursor->next;
  }
  this->tail = prev_node;
}

template <typename T> double_list<T>::double_list(const T &value) {
  this->head = std::make_shared<node>(value);
  this->tail = this->head;
  this->size = 1;
}

template <typename T> double_list<T>::double_list(const double_list<T> &right) {
  for (auto i : right) {
    this->push(i);
  }
  this->size = right.size;
}

template <typename T>
double_list<T>::double_list(double_list<T> &&right) noexcept {
  for (auto i : right) {
    this->push(i);
  }
  this->size = right.size;
  right.clear();
}

template <typename T>
double_list<T>::double_list(std::initializer_list<T> list) {
  for (auto i : list) {
    this->push(i);
  }
  this->size = list.size();
}

// Присваивание
template <typename T>
double_list<T> &double_list<T>::operator=(const double_list<T> &right) {
  this->clear();
  for (auto i : right) {
    this->push(i);
  }
  this->size = right.size;
  return *this;
}

template <typename T>
double_list<T> &double_list<T>::operator=(double_list<T> &&right) noexcept {
  this->clear();
  for (auto i : right) {
    this->push(i);
  }
  this->size = right.size;
  right.clear();
  return *this;
}

template <typename T>
double_list<T> &double_list<T>::operator=(std::initializer_list<T> list) {
  this->clear();
  for (auto i : list) {
    this->push(i);
  }
  this->size = list.size();
  return *this;
}

// Добавление/удаление
template <typename T> void double_list<T>::push(const T &value) {
  std::shared_ptr<node> new_node = std::make_shared<node>(value);
  this->size++;
  if (!this->head && !this->tail) {
    this->head = new_node;
    this->tail = new_node;
    return;
  }
  new_node->prev = this->tail;
  this->tail->next = new_node;
  this->tail = new_node;
}

template <typename T> T double_list<T>::pop() {
  std::shared_ptr<node> tmp = this->tail;
  this->tail = this->tail->prev;
  if (!this->tail) {
    this->head = nullptr;
  }
  T data = tmp->data;
  tmp->clear();
  this->size--;
  return data;
}

template <typename T> void double_list<T>::unshift(const T &value) {
  std::shared_ptr<node> new_node = std::make_shared<node>(value);
  this->size++;
  if (!this->head && !this->tail) {
    this->head = new_node;
    this->tail = new_node;
    return;
  }
  new_node->next = this->head;
  this->head->prev = new_node;
  this->head = new_node;
}

template <typename T> T double_list<T>::shift() {
  std::shared_ptr<node> tmp = this->head;
  T data = tmp->data;
  this->head = this->head->next;
  if (!this->head) {
    this->tail = nullptr;
  }
  tmp->clear();
  this->size--;
  return data;
}

// Вспомогательные методы
// Сортировка
// Слияние двух списков
template <typename T>
std::shared_ptr<typename double_list<T>::node>
double_list<T>::merge(double_list<T> &left, double_list<T> &right) {
  double_list<T> result;
  std::shared_ptr<node> left_cursor = left.head;
  std::shared_ptr<node> right_cursor = right.head;
  std::shared_ptr<node> *result_cursor = &result.head;
  std::shared_ptr<node> prev_cursor = nullptr;
  while (left_cursor && right_cursor) {
    if (left_cursor->data <= right_cursor->data) {
      *result_cursor = left_cursor;
      left_cursor = left_cursor->next;
    } else {
      *result_cursor = right_cursor;
      right_cursor = right_cursor->next;
    }
    (*result_cursor)->prev = prev_cursor;
    prev_cursor = *result_cursor;
    result_cursor = &((*result_cursor)->next);
  }

  if (left_cursor) {
    while (left_cursor) {
      *result_cursor = left_cursor;
      left_cursor = left_cursor->next;
      (*result_cursor)->prev = prev_cursor;
      prev_cursor = *result_cursor;
      result_cursor = &((*result_cursor)->next);
    }
  }

  if (right_cursor) {
    while (right_cursor) {
      *result_cursor = right_cursor;
      right_cursor = right_cursor->next;
      (*result_cursor)->prev = prev_cursor;
      prev_cursor = *result_cursor;
      result_cursor = &((*result_cursor)->next);
    }
  }
  return result.head;
}

// Поиск середины
template <typename T>
std::shared_ptr<typename double_list<T>::node>
double_list<T>::getMiddle(std::shared_ptr<typename double_list<T>::node> head) {
  if (!head) {
    return head;
  }

  std::shared_ptr<node> slow = head;
  std::shared_ptr<node> fast = head->next;

  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }

  return slow;
}

template <typename T> void double_list<T>::sort() {

  if (!head || !head->next) {
    return;
  }

  std::shared_ptr<node> mid = getMiddle(head);
  std::shared_ptr<node> middleNext = mid->next;
  mid->next = nullptr;
  mid->prev = nullptr;

  double_list<T> left{head};
  double_list<T> right{middleNext};

  left.sort();
  right.sort();

  this->head = merge(left, right);
}

template <typename T> bool double_list<T>::empty() const {
  return !this->head && !this->tail;
}

template <typename T> t_size double_list<T>::getSize() const {
  return this->size;
}

template <typename T> void double_list<T>::clear() {
  std::shared_ptr<node> cursor = this->head;
  std::shared_ptr<node> next = this->head;

  while (cursor) {
    next = cursor->next;
    cursor->clear();
    cursor = next;
  }
  this->head = nullptr;
  this->tail = nullptr;
  this->size = 0;
}

// Итератор двусвязного списка
// Конструктор
template <typename T>
double_list<T>::Iterator::Iterator(double_list<T>::Iterator::pointer ptr)
    : m_ptr(ptr) {}

// Разыменование
template <typename T>
typename double_list<T>::Iterator::reference
double_list<T>::Iterator::operator*() const {
  return this->m_ptr->data;
}

// Получить указатель
template <typename T>
typename double_list<T>::Iterator::pointer
double_list<T>::Iterator::operator->() {
  return m_ptr;
}

// Инкременты
template <typename T>
typename double_list<T>::Iterator &double_list<T>::Iterator::operator++() {
  if (m_ptr) {
    m_ptr = m_ptr->next.get();
  }
  return *this;
}

template <typename T>
typename double_list<T>::Iterator double_list<T>::Iterator::operator++(int) {
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}

// Декременты
template <typename T>
typename double_list<T>::Iterator &double_list<T>::Iterator::operator--() {
  if (m_ptr) {
    this->m_ptr = m_ptr->prev.get();
  }
  return *this;
}

template <typename T>
typename double_list<T>::Iterator double_list<T>::Iterator::operator--(int) {
  Iterator tmp = *this;
  --(*this);
  return tmp;
}

// Доступ к итерации
template <typename T>
typename double_list<T>::Iterator double_list<T>::begin() const {
  if (this->head) {
    return Iterator(this->head.get());
  }
  return nullptr;
}

template <typename T>
typename double_list<T>::Iterator double_list<T>::end() const {
  if (this->tail) {
    return Iterator(this->tail.get());
  }
  return nullptr;
}

} // namespace list
