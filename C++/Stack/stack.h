#pragma once

#include <memory>
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
    T max = nullptr;
    T min = nullptr;
    std::shared_ptr<node> prev;
    node(T data);
  };
  bool find_min_max; // Если true, то сохранять max и min

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
} // namespace stack

#include "stack.inl"
