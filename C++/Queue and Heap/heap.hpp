#pragma once
#include <cstddef>
#include <ctime>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
// Двоичная куча, биномиальная куча, левая куча, фибоначчиева куча

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
  void decrease_key(std::size_t, T);
  void insert(T);
  T extract();
  void clear();
};

//Левая куча
template <typename T> class left_heap {
  struct node {
    T key;
    std::shared_ptr<node> left;
    std::shared_ptr<node> right;
    int dist;
    node(T val = T{}) : key(val), left(nullptr), right(nullptr), dist(0) {}
  };

  std::shared_ptr<node> head;

  void swap(std::shared_ptr<node> &, std::shared_ptr<node> &);

  std::shared_ptr<node> merge(std::shared_ptr<node>, std::shared_ptr<node>);

public:
  left_heap() : head(nullptr) {}
  left_heap(T);
  left_heap(std::initializer_list<T>);
  void insert(T);
  T extract() const;
  void decrease_key(T, T) {}
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
    operator T() { return key; }
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
  void swap_parent(std::shared_ptr<Node>, std::shared_ptr<Node>);
  void swap(std::shared_ptr<Node>, std::shared_ptr<Node>);
  std::shared_ptr<Node> find(std::shared_ptr<Node>, T) const;
  std::vector<T> collect(std::weak_ptr<Node>) const;

public:
  binomial_heap();
  binomial_heap(T);
  binomial_heap(std::shared_ptr<Node> &);
  binomial_heap(std::initializer_list<T>);
  binomial_heap(const binomial_heap<T> &);
  binomial_heap(binomial_heap<T> &&) noexcept;
  binomial_heap<T> &operator=(std::initializer_list<T>);
  binomial_heap<T> &operator=(const binomial_heap<T> &);
  binomial_heap<T> &operator=(binomial_heap<T> &&) noexcept;
  T extract();
  T getMin() const;
  void merge(binomial_heap<T> &);
  std::shared_ptr<Node> find(T) const;
  void insert(T);
  void decrease_key(T, std::shared_ptr<Node>);
  void deleteKey(T);
  void clear();
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

template <typename T> void binary_heap<T>::clear() { this->array.clear(); }

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
template <typename T> void binary_heap<T>::decrease_key(std::size_t i, T data) {
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
  decrease_key(array.size() - 1, data);
}

// Удаление корня
template <typename T> T binary_heap<T>::extract() {
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
// Конструкторы
template <typename T> binomial_heap<T>::binomial_heap() : head(nullptr) {}

template <typename T> binomial_heap<T>::binomial_heap(T val) {
  this->head = std::make_shared<Node>(val);
}

template <typename T>
binomial_heap<T>::binomial_heap(
    std::shared_ptr<typename binomial_heap<T>::Node> &node)
    : head(node) {}

template <typename T>
binomial_heap<T>::binomial_heap(std::initializer_list<T> list) {
  for (auto i : list) {
    insert(i);
  }
}

template <typename T>
binomial_heap<T>::binomial_heap(const binomial_heap<T> &other) {
  std::vector<T> array = other.collect(other.head);
  for (auto i : array) {
    insert(i);
  }
}

template <typename T>
binomial_heap<T>::binomial_heap(binomial_heap<T> &&other) noexcept {
  while (!other.empty()) {
    insert(other.extract());
  }
}

//Операторы присваивания
template <typename T>
binomial_heap<T> &binomial_heap<T>::operator=(std::initializer_list<T> list) {
  this->clear();
  for (auto i : list) {
    insert(i);
  }
  return *this;
}

template <typename T>
binomial_heap<T> &binomial_heap<T>::operator=(const binomial_heap<T> &other) {
  this->clear();
  std::vector<T> array = other.collect(other.head);
  for (auto i : array) {
    insert(i);
  }
  return *this;
}

template <typename T>
binomial_heap<T> &
binomial_heap<T>::operator=(binomial_heap<T> &&other) noexcept {
  this->clear();
  while (!other.empty()) {
    insert(other.extract());
  }
  return *this;
}

// Merge для списка деревьев
template <typename T>
std::shared_ptr<typename binomial_heap<T>::Node>
binomial_heap<T>::mergeSort(std::shared_ptr<Node> left,
                            std::shared_ptr<Node> right) {
  std::shared_ptr<Node> result = nullptr; //Новая голова списка
  std::shared_ptr<Node> *cursor = &result;

  while (left && right) { //Пока есть правый или левы списокё
    if (left->degree <=
        right->degree) { //Если степень правого больше степени левого
      *cursor = left; //Левый ставим в текущую позицию
      left = left->sibling; //Сдвигаем дальше левый список
    } else {
      *cursor = right; //Правый ставим в текущую позицию
      right = right->sibling; //Сдвигаем дальше правый список
    }
    cursor = &((*cursor)->sibling); //Следующая пустая позиция
  }

  *cursor = left ? left : right; //Остаток ставим в хвост списка

  return result;
}

// Объединение деревьев
template <typename T>
void binomial_heap<T>::link(std::shared_ptr<Node> left,
                            std::shared_ptr<Node> right) {
  left->sibling = right->sibling; //Потомок ребёнка становится потомком родителя
  right->sibling = left->child; //Потомком ребёнка становится ребёнок родителя
  right->parent = left; //Родителем потомка становится родителеь
  left->child = right; //Ребёнком родителя становится потомок
  left->degree++; //Увеличиваем степень
}

// Поменять элементы местами
template <typename T>
void binomial_heap<T>::swap(std::shared_ptr<Node> left,
                            std::shared_ptr<Node> right) {
  std::shared_ptr<Node> tmp = left->parent;
  left->parent = right; //Родитель левого это правый
  left->sibling = right->sibling; //Потомок левого это бывший потомок родителя

  right->parent = tmp; //Родитель правого это бывший родитель левого
  right->sibling = left; //Потомок правого это левый
}

// Объединяем списки, после сливаем одинаковые деревья
template <typename T> void binomial_heap<T>::merge(binomial_heap<T> &other) {
  this->head = mergeSort(this->head, other.head);
  std::shared_ptr<Node> cursor = this->head;

  while (cursor->sibling) {
    // cursor->sibling->parent = cursor;
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

// Вставка элемента
template <typename T> void binomial_heap<T>::insert(T val) {
  binomial_heap<T> new_el(val);
  this->merge(new_el);
}

// Получить ссылку на минимальный элемент
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

// Получить значение минимального элемента
template <typename T> T binomial_heap<T>::getMin() const {
  return this->findMin()->key;
}

// Извлечь минимальный
template <typename T> T binomial_heap<T>::extract() {
  if (!head) {
    throw std::runtime_error("Empty heap");
  }
  std::shared_ptr<Node> min = findMin();
  std::shared_ptr<Node> local_head = min->child;
  T min_key = min->key;
  if (min == head) {
    head = head->sibling;
  }

  std::shared_ptr<Node> cursor = head;
  if (cursor) {
    while (cursor) {
      if (!cursor->sibling) {
        break;
      }
      if (cursor->sibling == min) {
        cursor->sibling = min->sibling;
      }
      cursor = cursor->sibling;
    }
  }
  min->delete_this();
  min = nullptr;

  if (local_head) {
    binomial_heap<T> tmp(local_head);
    this->merge(tmp);
  }

  return min_key;
}

//Конвертировать кучу в массив
template <typename T>
std::vector<T> binomial_heap<T>::collect(std::weak_ptr<Node> head) const {
  if (!head.lock()->sibling) {
    return {head.lock()->key};
  }
  std::vector<T> elements;
  std::weak_ptr<Node> cursor(head);
  while (cursor.lock()) {
    elements.push_back(cursor.lock()->key);
    if (cursor.lock()->child) {
      std::vector<T> child = collect(cursor.lock()->child);
      elements.insert(elements.end(), child.begin(), child.end());
    }
    cursor = cursor.lock()->sibling;
  }
  return elements;
}

//Очистить кучу
template <typename T> void binomial_heap<T>::clear() { this->head = nullptr; }

// Найти ноду по ключу
template <typename T>
std::shared_ptr<typename binomial_heap<T>::Node>
binomial_heap<T>::find(std::shared_ptr<Node> head, T key) const {
  if (!head) {
    return nullptr;
  }
  std::shared_ptr<Node> cursor = head;
  do {
    if (cursor->key == key) {
      return cursor;
    }
    cursor = cursor->sibling;
  } while (cursor);
  return find(head->child, key);
}

template <typename T>
std::shared_ptr<typename binomial_heap<T>::Node>
binomial_heap<T>::find(T key) const {
  std::shared_ptr<Node> cursor = head;
  std::shared_ptr<Node> node = nullptr;
  while (cursor) {
    if (cursor->key == key) {
      return cursor;
    } else if (cursor->child) {
      node = find(cursor->child, key);
      if (node) {
        break;
      }
    }
    cursor = cursor->sibling;
  }
  return node;
}

//Поменять местами родителей
template <typename T>
void binomial_heap<T>::swap_parent(std::shared_ptr<Node> left,
                                   std::shared_ptr<Node> right) {
  T tmp = left->key;
  left->key = right->key;
  right->key = tmp;
}

// Уменьшить ключ
template <typename T>
void binomial_heap<T>::decrease_key(T key, std::shared_ptr<Node> node) {
  if (!node) {
    return;
  }
  if (node->key < key)
    return;
  node->key = key;
  while (node->parent && node->key <= node->parent->key) {
    swap_parent(node, node->parent);
    node = node->parent;
  }
}

// Удалить ключ
template <typename T> void binomial_heap<T>::deleteKey(T key) {
  std::shared_ptr<Node> node = find(key);
  T min = getMin();
  decrease_key(min, node);
  extract();
}

// Проверка на пустоту
template <typename T> bool binomial_heap<T>::empty() const { return !head; }

/*** Левосторонняя куча ***/
//Конструкторы
template <typename T> left_heap<T>::left_heap(std::initializer_list<T> list) {
  for (auto i : list) {
    insert(i);
  }
}

template <typename T>
left_heap<T>::left_heap(T key) : head(std::make_shared<node>(key)) {}

//Поменять местами указатели
template <typename T>
void left_heap<T>::swap(std::shared_ptr<node> &left,
                        std::shared_ptr<node> &right) {
  T key = left->key;
  left->key = right->key;
  right->key = key;
}

//Слияние куч
template <typename T>
std::shared_ptr<typename left_heap<T>::node>
left_heap<T>::merge(std::shared_ptr<node> left, std::shared_ptr<node> right) {
  if (!left) {
    return right;
  }
  if (!right) {
    return left;
  }
  if (left->key < right->key) {
    swap(left, right);
  }

  left->right = merge(left->right, right);

  if (left->left && left->right) {
    if (left->right->dist > left->left->dist) {
      swap(left->left, right->right);
    }
  }
  left->dist++;
  return left;
}

//Вставка нового элемента
template <typename T> void left_heap<T>::insert(T value) {
  left_heap<T> lh(value);
  head = merge(head, lh.head);
}

//Извлечь минимальное
template <typename T> T left_heap<T>::extract() const {
  if (!head) {
    throw std::runtime_error("Empty heap");
  }
  return head->key;
}
