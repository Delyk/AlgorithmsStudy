#pragma once
// Напоминалка: список любых значенийб XOR-список, список с пропусками,
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

  std::shared_ptr<node> head;

public:
  // Конструкторы
  single_list();
  single_list(const T &);
  single_list(const single_list &);
  single_list(const single_list &&) noexcept;
  single_list(std::initializer_list<T>);
  // Операторы присваивания
  single_list &operator=(const single_list &);
  single_list &operator=(const single_list &&) noexcept;
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
  // Методы доступа к итератору
  Iterator begin() const;
  Iterator end() const;
};

// Методы односвязного списка
// Конструктор элемента
template <typename T>
single_list<T>::node::node(const T &value)
    : list<T>::baseNode::data(value), next(nullptr) {}

template <typename T> void single_list<T>::node::clear() {
  next = nullptr;
  list<T>::data = NULL;
}

// Конструкторы
template <typename T>
single_list<T>::single_list() : head(nullptr), list<T>::size(0) {}

template <typename T>
single_list<T>::single_list(const T &value)
    : head(std::make_shared<T>(value)), list<T>::size(1) {}

template <typename T> single_list<T>::single_list(const single_list &right) {
  for (auto i : right) {
    this->push(i);
  }
  list<T>::size = right.list<T>::size;
}

template <typename T>
single_list<T>::single_list(const single_list &&right) noexcept {
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
  this->clear();
  for (auto i : right) {
    this->push(i);
  }
  list<T>::size = right.list<T>::size;
}

template <typename T>
single_list<T> &single_list<T>::operator=(const single_list &&right) noexcept {
  this->clear();
  for (auto i : right) {
    this->push(i);
  }
  list<T>::size = right.list<T>::size;
  right.clear();
}

template <typename T>
single_list<T> &single_list<T>::operator=(std::initializer_list<T> init) {
  this->clear();
  for (auto i : init) {
    this->push(i);
  }
  list<T>::size = init.size();
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
  std::shared_ptr<node> cursor = head;
  while (cursor->next->next != nullptr) {
    cursor = head->next;
  }
  auto prev = cursor->next;
  T tmp = prev->list<T>::baseNode::data;
  prev->clear();
  cursor->next = nullptr;
  list<T>::size--;

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
    throw std::runtime_error("List empty\n");
  }
  T head_data = head->list<T>::baseNode::data;
  std::shared_ptr<node> tmp = head;
  head = head->next;
  tmp->clear();
  list<T>::size--;
  return head_data;
}

// Сортировка
template <typename T> void single_list<T>::sort() {}

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
  return Iterator(head.get());
}

template <typename T>
typename single_list<T>::Iterator single_list<T>::end() const {
  return Iterator(nullptr);
}

} // namespace list
