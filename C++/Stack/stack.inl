#include "stack.h"
#include <stdexcept>

namespace stack {

// Методы стека на основе массива
template <typename T, unsigned int max_size = 100>
array_stack::array_stack() : last_index(-1) {}

template <typename T, unsigned int max_size = 100>
void array_stack::push(T value) {
  if (last_index < max_size) {
    last_index++;
    array[last_index] = value;
  } else {
    last_index = max_size - 1;
    throw std::overflow_error("Stack overflow");
  }
}

template <typename T, unsigned int max_size = 100> T array_stack::pop() {
  if (isEmpty()) {
    throw std::underflow_error("Stack empty");
  }
  T tmp = array[last_index];
  array[last_index] = T{};
  last_index--;
  return tmp;
}

template <typename T, unsigned int max_size = 100> T array_stack::peek() const {
  if (isEmpty()) {
    throw std::underflow_error("Stack empty");
  }
  return array[last_index];
}

template <typename T, unsigned int max_size = 100>
int array_stack::getSize() const {
  return last_index + 1;
}

template <typename T, unsigned int max_size = 100>
bool array_stack::isEmpty() const {
  return last_index == -1;
}

template <typename T, unsigned int max_size = 100>
void array_stack::print() const {
  if (isEmpty()) {
    throw std::underflow_error("Stack empty");
  }
  for (int i = last_index; i >= 0; i--) {
    std::cout << array[i] << std::endl;
  }
}

// Методы стека на основе списка

} // namespace stack
