#pragma once
#include <exception>
#include <future>
#include <iostream>
#include <memory>
#include <stdexcept>
namespace stack {
// Стек на основе массива
template <typename T, unsigned int max_size = 100> class array_stack {
  T array[max_size];
  int last_index;

public:
  array_stack();
  void push(T value);   // Добавить в конец
  T pop();              // Удалить из конца
  T peek() const;       // Получить значение в конце
  int getSize() const;  // Получить размер
  bool isEmpty() const; // Проверить на пустоту
  void print() const;   // Напечатать стек
};

// Стек на основе списка
template <typename T> class list_stack {
  struct node {
    T data;
    T max = T{};
    T min = T{};
    std::shared_ptr<node> prev;
    node(T data, bool, std::shared_ptr<node> prev = nullptr);
  };
  bool find_min_max; // Если true, то сохранять max и min
  std::shared_ptr<node> head = nullptr;
  int size = 0;

public:
  list_stack(bool find_min_max = false);
  void push(T value);   // Добавить в конец
  T pop();              // Удалить из конца
  T peek() const;       // Получить значение в конце
  int getSize() const;  // Получить размер
  bool isEmpty() const; // Проверить на пустоту
  void print() const;   // Напечатать стек
  T getMax() const;
  T getMin() const;
};

// Методы стека на основе массива
// Конструктор
template <typename T, unsigned int max_size>
array_stack<T, max_size>::array_stack() : last_index(-1) {}

// push
template <typename T, unsigned int max_size>
void array_stack<T, max_size>::push(T value) {
  last_index++;
  if (last_index < static_cast<int>(max_size)) {
    array[last_index] = value;
  } else {
    last_index = max_size - 1;
    throw std::overflow_error("Stack overflow");
  }
}

// pop
template <typename T, unsigned int max_size> T array_stack<T, max_size>::pop() {
  if (isEmpty()) {
    throw std::underflow_error("Stack empty");
  }
  T tmp = array[last_index];
  array[last_index] = T{};
  last_index--;
  return tmp;
}

// peek
template <typename T, unsigned int max_size>
T array_stack<T, max_size>::peek() const {
  if (isEmpty()) {
    throw std::underflow_error("Stack empty");
  }
  return array[last_index];
}

// Получить размер стека
template <typename T, unsigned int max_size>
int array_stack<T, max_size>::getSize() const {
  return last_index + 1;
}

// Проверка на пустоту
template <typename T, unsigned int max_size>
bool array_stack<T, max_size>::isEmpty() const {
  return last_index == -1;
}

// Напечатать стек
template <typename T, unsigned int max_size>
void array_stack<T, max_size>::print() const {
  if (isEmpty()) {
    throw std::underflow_error("Stack empty");
  }
  for (int i = last_index; i >= 0; i--) {
    std::cout << array[i] << std::endl;
  }
}

// Методы стека на основе списка

// Конструктор элемента
template <typename T>
list_stack<T>::node::node(T data, bool min_max_flag, std::shared_ptr<node> prev)
    : data(data), prev(prev) {
  if (min_max_flag) {
    if (prev) {
      if (prev->max < data) {
        max = data;
        min = prev->min;
      } else if (data < prev->min) {
        max = prev->max;
        min = data;
      } else {
        max = prev->max;
        min = prev->min;
      }
    } else {
      max = data;
      min = data;
    }
  }
}

//Конструктор
template <typename T>
list_stack<T>::list_stack(bool find_min_max) : find_min_max(find_min_max) {}

// push
template <typename T> void list_stack<T>::push(T value) {
  std::shared_ptr<node> new_node =
      std::make_shared<node>(value, find_min_max, head);
  head = new_node;
  size++;
}

// pop
template <typename T> T list_stack<T>::pop() {
  if (!head) {
    throw std::underflow_error("Stack empty");
  }
  T tmp = head->data;
  head = head->prev;
  size--;
  return tmp;
}

// peek
template <typename T> T list_stack<T>::peek() const {
  if (!head) {
    throw std::underflow_error("Stack empty");
  }
  return head->data;
}

// Получить размер стека
template <typename T> int list_stack<T>::getSize() const { return size; }

// Проверить стек на пустоту
template <typename T> bool list_stack<T>::isEmpty() const { return size == 0; }

// Напечатать стек
template <typename T> void list_stack<T>::print() const {
  std::shared_ptr<node> cursor = head;
  while (cursor) {
    std::cout << cursor->data << std::endl;
    cursor = cursor->prev;
  }
}

// Получить максимальное
template <typename T> T list_stack<T>::getMax() const {
  if (!find_min_max) {
    throw std::runtime_error("Writing max/min off");
  }
  return head->max;
}

// Получить минимальное
template <typename T> T list_stack<T>::getMin() const {
  if (!find_min_max) {
    throw std::runtime_error("Writing max/min off");
  }
  return head->min;
}

} // namespace stack
