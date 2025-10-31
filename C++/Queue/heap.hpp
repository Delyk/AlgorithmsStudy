#pragma once
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <vector>

//Двоичная куча
template <typename T> class binary_heap {
  std::vector<T> array;

public:
  binary_heap();
  binary_heap(std::initializer_list<T>);
  binary_heap(const binary_heap &);
  binary_heap(binary_heap &&) noexcept;
  binary_heap &operator=(std::initializer_list<T>);
  binary_heap &operator=(const binary_heap &);
  binary_heap &operator=(binary_heap &&) noexcept;

  void heapify(std::vector<T> &, std::size_t);
  void build_heap(std::vector<T> &);
  void heapsort(std::vector<T> &);
  void increase_key(std::size_t, T);
  void insert(T);
  T extractMax();
};

/*** Методы двоичной кучи ***/
//Конструкторы
template <typename T> binary_heap<T>::binary_heap() {}

template <typename T>
binary_heap<T>::binary_heap(std::initializer_list<T> list) {
  array = std::vector<T>(list);
  build_heap(array);
}

template <typename T> binary_heap<T>::binary_heap(const binary_heap<T> &other) {
  array = other.array;
}

template <typename T>
binary_heap<T>::binary_heap(binary_heap<T> &&other) noexcept {
  array = other.array;
  other.array.clear();
}

//Операторы присваивания
template <typename T>
binary_heap<T> &binary_heap<T>::operator=(std::initializer_list<T> list) {
  array = std::vector<T>(list);
  build_heap(array);
  return *this;
}

template <typename T>
binary_heap<T> &binary_heap<T>::operator=(const binary_heap<T> &right) {
  array = right.array;
  return *this;
}

template <typename T>
binary_heap<T> &binary_heap<T>::operator=(binary_heap<T> &&right) noexcept {
  array = right.array;
  right.array.clear();
  return *this;
}

//"Всплытие" больших элементов вверх
template <typename T>
void binary_heap<T>::heapify(std::vector<T> &array, std::size_t root) {
  std::size_t largest = root;
  while (true) {
    std::size_t left = 2 * root + 1;
    std::size_t right = 2 * root + 2;

    //Если левая часть не выходит за размеры массива
    //и левый потомок больше корня
    if (left < array.size() && array[left] > array[largest]) {
      largest = left; //Указываем на наибольший потомок
    }

    //Если правая часть не выходит за размеры массива
    //и правый потомок больше родителя
    if (right < array.size() && array[right] > array[largest]) {
      largest = right; //Указываем на наибольший потомок
    }

    //Если один из потомков оказался больше корня
    if (largest != root) {
      //Меняем местами корень и потомок и устанавливаем новое наибольшее
      std::swap(array[root], array[largest]);
      root = largest;
    } else {
      break;
    }
  }
}

//Построение кучи
template <typename T> void binary_heap<T>::build_heap(std::vector<T> &arr) {
  //Начиная с самого старшего корня снизу вверх создаём кучу
  for (int i = arr.size() / 2 - 1; i >= 0; i--) {
    heapify(arr, i);
  }
}

//Изменение элемента
template <typename T> void binary_heap<T>::increase_key(std::size_t i, T data) {
  array[i] = data;
  //Поднимаем элемент вверх по дереву
  while (i > 0 && array[(i - 1) / 2] < array[i]) {
    std::swap(array[(i - 1) / 2], array[i]);
    i = (i - 1) / 2;
  }
}

//Вставка элемента
template <typename T> void binary_heap<T>::insert(T data) {
  array.push_back(1);
  increase_key(array.size() - 1, data);
}

//Удаление корня
template <typename T> T binary_heap<T>::extractMax() {
  T max = array[0];
  array[0] = array[array.size() - 1];
  heapify(this->array, 0);
  return max;
}

//Пирамидальная сортировка
template <typename T> void binary_heap<T>::heapsort(std::vector<T> &arr) {
  build_heap(arr);
  for (int i = arr.size() - 1; i >= 0; i--) {
    std::swap(arr[0], arr[i]);
    heapify(arr, 0);
  }
}
