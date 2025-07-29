#pragma once
#include <initializer_list>
#include <stdexcept>
#include <typeinfo>
namespace arrays {

using t_size = unsigned int;
// Контейнер хранения элементов для гетерогенного массива

class universal {
  class elementBase { // Базовый тип для шаблонного наследника
  public:
    virtual ~elementBase() = default;
    virtual elementBase *clone() const = 0;
    virtual const std::type_info &type() const = 0;
  };

  template <typename T> class element : public elementBase {
    T data;

  public:
    element(const T &value) : data(value) {} // Базовый конструктор
    element(T &&value) noexcept : data(std::move(value)) {} // Move-конструктор
    elementBase *clone() const override { return new element<T>(*this); }
    const std::type_info &type() const override {
      return typeid(T);
    } // Вернуть тип
    // Геттер
    T &get() { return this->data; }
  };
  elementBase *store;

public:
  universal() : store(nullptr) {}

  template <typename T>
  universal(const T &value)
      : store(new element<T>(value)) {} // Стандартный конструктор

  universal(const universal &right)
      : store(right.store ? right.store->clone() : nullptr) {
  } // Конструктор копирования

  universal(universal &&right) noexcept : store(right.store) {
    right.store = nullptr;
  } // Move-конструктор

  ~universal() { delete store; } // Деструктор

  universal &operator=(const universal &right) { // Присваивание с копированием
    if (this != &right) {
      delete store;
      store = right.store ? right.store->clone() : nullptr;
    }
    return *this;
  }

  universal &operator=(universal &&right) noexcept { // Move-присваивание
    if (this != &right) {
      delete store;
      store = right.store;
      right.store = nullptr;
    }

    return *this;
  }

  template <typename T>
  universal &operator=(const T &value) { // Присваивание значения
    delete this->store;
    this->store = new element<T>(value);
    return *this;
  }

  const std::type_info &type() const { // Тип
    if (!this->store) {
      throw std::bad_typeid();
    }
    return this->store->type();
  }

  // Кастинги
  template <typename T> friend T &universal_cast(const universal &var);
};

// Динамический массив
template <typename T> class array {
  t_size size;
  t_size capacity;
  T *static_array;

  void resize();

public:
  explicit array(t_size size = 0);      // Конструктор
  array(std::initializer_list<T> init); // Конструктор для списка
  ~array();                             // Деструктор
  array(const array &); // Конструктор копирования
  array &operator=(const array &); // Оператор присваивания
  array &operator=(std::initializer_list<T> init); // Присваивание со списком
  T &operator[](t_size index); // Перегрузка оператора индексирования
  const T &operator[](
      t_size index) const; // Перегрузка оператора индексирования для констант
  array(array &&) noexcept;            // Move-конструктор
  array &operator=(array &&) noexcept; // Move-присваивание
  t_size getSize() const;              // Геттеры
  t_size getCapacity() const;
  void push_back(const T &value); // Вставка в конец массива
};

// Функции динамического массива

// Конструктор размера
template <typename T> array<T>::array(t_size size) {
  this->size = size;
  this->capacity = size * 2;
  if (size > 0) {
    this->static_array = new T[this->capacity]();
  } else {
    this->static_array = nullptr;
  }
}

// Конструктор списка
template <typename T>
array<T>::array(std::initializer_list<T> init) : array(init.size()) {
  int i = 0;
  for (const T &value : init) {
    this->static_array[i++] = value;
  }
}

// Деструктор
template <typename T> array<T>::~array() { delete[] static_array; }

// Конструктор копирования
template <typename T>
array<T>::array(const array &right) : array(right.getSize()) {
  for (t_size i = 0; i < right.getSize(); i++) {
    this->static_array[i] = right[i];
  }
}

// Присваивание копирования
template <typename T> array<T> &array<T>::operator=(const array &right) {
  if (&right != this) {
    t_size rightSize = right.getSize();
    delete[] static_array;
    this->size = rightSize;
    this->capacity = right.getCapacity();
    static_array = new T[this->capacity]();
    for (t_size i = 0; i < rightSize; i++) {
      this->static_array[i] = right[i];
    }
  }

  return *this;
}

// Присваивание списка
template <typename T>
array<T> &array<T>::operator=(std::initializer_list<T> init) {
  delete[] static_array;
  this->size = init.size();
  this->capacity = this->size * 2;
  this->static_array = new T[this->capacity]();
  int i = 0;
  for (const T &value : init) {
    this->static_array[i++] = value;
  }

  return *this;
}

// Индексирование
template <typename T> T &array<T>::operator[](t_size index) {
  if (index >= this->size) {
    throw std::out_of_range("Out of range\n");
  }

  return this->static_array[index];
}

// Константное индексирование
template <typename T> const T &array<T>::operator[](t_size index) const {
  if (index >= this->size) {
    throw std::out_of_range("Out of range\n");
  }

  return this->static_array[index];
}

// Конструктор move-копирования
template <typename T>
array<T>::array(array &&right) noexcept
    : size(right.size), capacity(right.capacity),
      static_array(right.static_array) {
  right.size = 0;
  right.capacity = 0;
  right.static_array = nullptr;
}

// Move-присваивание
template <typename T> array<T> &array<T>::operator=(array &&right) noexcept {
  if (this != &right) {
    delete[] static_array;
    this->size = right.size;
    this->capacity = right.capacity;
    this->static_array = right.static_array;

    right.size = 0;
    right.capacity = 0;
    right.static_array = nullptr;
  }
  return *this;
}

// Геттеры
template <typename T> inline t_size array<T>::getSize() const {
  return this->size;
}

template <typename T> inline t_size array<T>::getCapacity() const {
  return this->capacity;
}

// Добавить в  конец  массива
template <typename T> void array<T>::push_back(const T &value) {
  if (this->size >= this->capacity) {
    resize();
  }
  this->static_array[this->size] = value;
  this->size++;
}

// Изменить  размер в  памяти
template <typename T> void array<T>::resize() {
  if (this->capacity > 0) {
    this->capacity = this->capacity * 2;
    T *new_array = new T[this->capacity]();

    for (t_size i = 0; i < this->size; i++) {
      new_array[i] = std::move(this->static_array[i]);
    }

    delete[] static_array;
    this->static_array = new_array;
  } else {
    this->capacity = 2;
    this->static_array = new T[this->capacity]();
  }
}

// Функции universal
template <typename T> T &universal_cast(const universal &var) {
  universal::element<T> *tmp = dynamic_cast<universal::element<T> *>(var.store);
  if (!tmp) {
    throw std::bad_typeid();
  }
  return tmp->get();
}

} // namespace arrays
