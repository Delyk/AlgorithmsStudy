#pragma once
#include <cstddef>
#include <ctime>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
//Двоичная куча, биномиальная куча, левая куча, фибоначчиева куча

// Двоичная куча
template <typename T> class binary_heap {
  std::vector<T> array;
  void heapify(std::vector<T> &array, std::size_t size, std::size_t root);

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

// Биномиальная куча
template <typename T> class binomial_heap {
  struct Node {
    T key;
    std::shared_ptr<Node> parent;
    std::shared_ptr<Node> child;
    std::shared_ptr<Node> sibling;
    int degree;
    Node(T key = T{})
        : key(key), parent(nullptr), child(nullptr), sibling(nullptr) {}
    void add(std::shared_ptr<Node> node) {}
    void delete_this() {
      if (parent) {
        parent->sibling = sibling;
      }
      if (child) {
        child->parent = nullptr;
      }
    }
    void clear() {
      parent = nullptr;
      child = nullptr;
      sibling = nullptr;
      key = T{};
      degree = 0;
    }
    ~Node() { this->clear(); }
  };
  std::shared_ptr<Node> head;

  std::shared_ptr<Node> findMin() const;
  std::shared_ptr<Node> mergeSort(std::shared_ptr<Node>, std::shared_ptr<Node>);
  void link(std::shared_ptr<Node>, std::shared_ptr<Node>);
  void swap(std::shared_ptr<Node>, std::shared_ptr<Node>);

public:
  binomial_heap();
  binomial_heap(T);
  binomial_heap(std::shared_ptr<Node> &);
  T extractMin();
  T getMin() const;
  void merge(binomial_heap<T> &);
  void insert(T);
  bool empty() const;
};

/*** Методы двоичной кучи ***/
// Конструкторы
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

// Операторы присваивания
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

    // Если левая часть не выходит за размеры массива
    // и левый потомок больше корня
    if (left < array.size() && array[left] > array[largest]) {
      largest = left; // Указываем на наибольший потомок
    }

    // Если правая часть не выходит за размеры массива
    // и правый потомок больше родителя
    if (right < array.size() && array[right] > array[largest]) {
      largest = right; // Указываем на наибольший потомок
    }

    // Если один из потомков оказался больше корня
    if (largest != root) {
      // Меняем местами корень и потомок и устанавливаем новое наибольшее
      std::swap(array[root], array[largest]);
      root = largest;
    } else {
      // Иначе значит у корня нет потомков, выходим из цикла
      break;
    }
  }
}

template <typename T>
void binary_heap<T>::heapify(std::vector<T> &array, std::size_t size,
                             std::size_t root) {
  std::size_t largest = root;
  while (true) {
    std::size_t left = 2 * root + 1;
    std::size_t right = 2 * root + 2;

    if (left < size && array[left] > array[largest]) {
      largest = left;
    }

    if (right < size && array[right] > array[largest]) {
      largest = right;
    }

    if (largest != root) {
      std::swap(array[root], array[largest]);
      root = largest;
    } else {
      break;
    }
  }
}

// Построение кучи
template <typename T> void binary_heap<T>::build_heap(std::vector<T> &arr) {
  // Начиная с самого старшего корня, у которого есть потомки, снизу вверх
  // создаём кучу
  for (int i = arr.size() / 2 - 1; i >= 0; i--) {
    heapify(arr, i);
  }
}

// Изменение элемента
template <typename T> void binary_heap<T>::increase_key(std::size_t i, T data) {
  array[i] = data;
  // Поднимаем элемент вверх по дереву
  int next = (i - 1) / 2;
  while (i > 0 && array[next] < array[i]) {
    std::swap(array[next], array[i]);
    i = next;
    next = (i - 1) / 2;
  }
}

// Вставка элемента
template <typename T> void binary_heap<T>::insert(T data) {
  array.push_back(1);
  increase_key(array.size() - 1, data);
}

// Удаление корня
template <typename T> T binary_heap<T>::extractMax() {
  T max = array[0];
  array[0] = array[array.size() - 1];
  array.pop_back();
  heapify(this->array, 0);
  return max;
}

// Пирамидальная сортировка
template <typename T> void binary_heap<T>::heapsort(std::vector<T> &arr) {
  build_heap(arr);
  for (int i = arr.size() - 1; i >= 0; i--) {
    std::swap(arr[0], arr[i]);
    heapify(arr, i, 0); // Просеиваем меньший элемент вниз, исключая самый
                        // нижний
  }
}
/*** Методы биномиальной кучи ***/
//Конструкторы
template <typename T> binomial_heap<T>::binomial_heap() : head(nullptr) {}

template <typename T> binomial_heap<T>::binomial_heap(T val) {
  this->head = std::make_shared<Node>(val);
}

template <typename T>
binomial_heap<T>::binomial_heap(
    std::shared_ptr<typename binomial_heap<T>::Node> &node)
    : head(node) {}

// Merge для списка деревьев
template <typename T>
std::shared_ptr<typename binomial_heap<T>::Node>
binomial_heap<T>::mergeSort(std::shared_ptr<Node> left,
                            std::shared_ptr<Node> right) {
  std::shared_ptr<Node> result = nullptr;
  std::shared_ptr<Node> *cursor = &result;

  while (left && right) {
    if (left->degree <= right->degree) {
      *cursor = left;
      left = left->sibling;
    } else {
      *cursor = right;
      right = right->sibling;
    }
    cursor = &((*cursor)->sibling);
  }

  *cursor = left ? left : right;

  return result;
}

// Объединение деревьев
template <typename T>
void binomial_heap<T>::link(std::shared_ptr<Node> left,
                            std::shared_ptr<Node> right) {
  left->sibling = right->sibling;
  right->sibling = left->child;
  right->parent = left;
  left->child = right;
  left->degree++;
}

//Поменять элементы местами
template <typename T>
void binomial_heap<T>::swap(std::shared_ptr<Node> left,
                            std::shared_ptr<Node> right) {
  std::shared_ptr<Node> tmp = left->parent;
  left->parent = right;
  left->sibling = right->sibling;

  right->parent = tmp;
  right->sibling = left;
}

// Объединяем списки, после сливаем одинаковые деревья
template <typename T> void binomial_heap<T>::merge(binomial_heap<T> &other) {
  this->head = mergeSort(this->head, other.head);
  std::shared_ptr<Node> cursor = this->head;

  while (cursor->sibling) {
    cursor->sibling->parent = cursor;
    if (cursor->degree == cursor->sibling->degree) {
      if (cursor->key > cursor->sibling->key) {
        swap(cursor, cursor->sibling);
        if (cursor == head) {
          head = cursor->parent;
        }
        cursor = cursor->parent;
      }
      link(cursor, cursor->sibling);
    } else {
      cursor = cursor->sibling;
    }
    if (!cursor) {
      break;
    }
  }
}

//Вставка элемента
template <typename T> void binomial_heap<T>::insert(T val) {
  binomial_heap<T> new_el{val};
  this->merge(new_el);
}

//Получить ссылку на минимальный элемент
template <typename T>
std::shared_ptr<typename binomial_heap<T>::Node>
binomial_heap<T>::findMin() const {
  if (!head) {
    throw std::runtime_error("Empty heap");
  }
  std::shared_ptr<Node> cur = head->sibling;
  std::shared_ptr<Node> min = head;
  while (cur) {
    if (min->key > cur->key) {
      min = cur;
    }
    cur = cur->sibling;
  }
  return min;
}

//Получить значение минимального элемента
template <typename T> T binomial_heap<T>::getMin() const {
  return this->findMin()->key;
}

//Извлечь минимальный
template <typename T> T binomial_heap<T>::extractMin() {
  if (!head) {
    throw std::runtime_error("Empty heap");
  }
  std::shared_ptr<Node> min = findMin();
  std::shared_ptr<Node> local_head = min->child;
  T min_key = min->key;
  if (min == head) {
    head = head->sibling;
  }
  min->delete_this();
  min = nullptr;

  if (local_head) {
    binomial_heap<T> tmp(local_head);
    this->merge(tmp);
  }

  return min_key;
}

//Проверка на пустоту
template <typename T> bool binomial_heap<T>::empty() const { return !head; }
