#pragma once
#include <stdexcept>
#include <type_traits>

namespace utils {
template <typename T, std::size_t Capacity>
class fixed_size_vector {
  public:
  using value_type = T;
  using size_type = std::size_t;
  using iterator = T *;
  using const_iterator = const T *;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  fixed_size_vector() = default;
  fixed_size_vector(std::initializer_list<value_type> initializer_list);
  template <typename InputIt>
  fixed_size_vector(InputIt first, InputIt last);
  fixed_size_vector(const fixed_size_vector &other);
  fixed_size_vector(fixed_size_vector &&other) noexcept;

  fixed_size_vector &operator=(const fixed_size_vector &other);
  fixed_size_vector &operator=(fixed_size_vector &&other) noexcept;

  ~fixed_size_vector();

  static constexpr size_type capacity();
  static constexpr size_type max_size();
  size_type size() const;
  void push_back(const value_type &val);
  void push_back(value_type &&val);
  template <typename... Args>
  void emplace_back(Args &&... args);
  value_type &operator[](size_type pos);
  const value_type &operator[](size_type pos) const;
  reference at(size_type pos);
  const_reference at(size_type pos) const;

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;
  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  pointer data();
  const_pointer data() const;

  bool empty() const;
  iterator insert(iterator pos, const value_type &value);
  iterator insert(iterator pos, value_type &&value);

  void clear();
  iterator erase(iterator pos);
  void pop_back();

  private:
  static constexpr size_type capacity_size{Capacity};
  using storage_type = typename std::aligned_storage<sizeof(value_type),
                                                     alignof(value_type)>::type;
  storage_type storage[capacity_size];
  size_type current_size{0};
  template <typename U>
  static U cast_pointer(storage_type *p);
  iterator get_storage();
  const_iterator get_storage() const;
};

template <typename T, std::size_t Capacity>
fixed_size_vector<T, Capacity>::fixed_size_vector(
    std::initializer_list<value_type> initializer_list) {
  for (auto &item : initializer_list) {
    emplace_back(item);
  }
}

template <typename T, std::size_t Capacity>
template <typename InputIt>
fixed_size_vector<T, Capacity>::fixed_size_vector(InputIt first, InputIt last) {
  for (InputIt iter = first; iter != last; ++iter) {
    emplace_back(*iter);
  }
}

template <typename T, std::size_t Capacity>
fixed_size_vector<T, Capacity>::fixed_size_vector(
    const fixed_size_vector &other) {
  for (const auto &item : other) {
    emplace_back(item);
  }
}

template <typename T, std::size_t Capacity>
fixed_size_vector<T, Capacity>::fixed_size_vector(
    fixed_size_vector &&other) noexcept {
  for (auto &item : other) {
    emplace_back(std::move(item));
  }
}

template <typename T, std::size_t Capacity>
fixed_size_vector<T, Capacity> &fixed_size_vector<T, Capacity>::operator=(
    const fixed_size_vector &other) {
  for (const auto &item : other) {
    emplace_back(item);
  }
  return *this;
}

template <typename T, std::size_t Capacity>
fixed_size_vector<T, Capacity> &fixed_size_vector<T, Capacity>::operator=(
    fixed_size_vector &&other) noexcept {
  for (auto &item : other) {
    emplace_back(std::move(item));
  }
  return *this;
}

template <typename T, std::size_t Capacity>
fixed_size_vector<T, Capacity>::~fixed_size_vector() {
  clear();
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::size_type
fixed_size_vector<T, Capacity>::size() const {
  return current_size;
}

template <typename T, std::size_t Capacity>
void fixed_size_vector<T, Capacity>::push_back(const value_type &val) {
  new (storage + current_size) value_type{val};
  ++current_size;
}

template <typename T, std::size_t Capacity>
void fixed_size_vector<T, Capacity>::push_back(value_type &&val) {
  new (storage + current_size) value_type{std::move(val)};
  ++current_size;
}

template <typename T, std::size_t Capacity>
template <typename... Args>
void fixed_size_vector<T, Capacity>::emplace_back(Args &&... args) {
  new (storage + current_size) value_type{std::forward<Args>(args)...};
  ++current_size;
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::value_type
    &fixed_size_vector<T, Capacity>::operator[](const size_type pos) {
  return get_storage()[pos];
}

template <typename T, std::size_t Capacity>
const typename fixed_size_vector<T, Capacity>::value_type
    &fixed_size_vector<T, Capacity>::operator[](const size_type pos) const {
  return get_storage()[pos];
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::value_type &
fixed_size_vector<T, Capacity>::at(const size_type pos) {
  if (pos > current_size - 1) throw std::out_of_range{""};
  return get_storage()[pos];
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_reference
fixed_size_vector<T, Capacity>::at(const size_type pos) const {
  if (pos > current_size - 1) throw std::out_of_range{""};
  return get_storage()[pos];
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::iterator
fixed_size_vector<T, Capacity>::begin() {
  return get_storage();
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_iterator
fixed_size_vector<T, Capacity>::begin() const {
  return get_storage();
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_iterator
fixed_size_vector<T, Capacity>::cbegin() const {
  return get_storage();
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::iterator
fixed_size_vector<T, Capacity>::end() {
  return get_storage() + current_size;
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_iterator
fixed_size_vector<T, Capacity>::end() const {
  return get_storage() + current_size;
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_iterator
fixed_size_vector<T, Capacity>::cend() const {
  return get_storage() + current_size;
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::reference
fixed_size_vector<T, Capacity>::front() {
  return get_storage()[0];
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_reference
fixed_size_vector<T, Capacity>::front() const {
  return get_storage()[0];
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::reference
fixed_size_vector<T, Capacity>::back() {
  return get_storage()[current_size - 1];
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_reference
fixed_size_vector<T, Capacity>::back() const {
  return get_storage()[current_size - 1];
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::pointer
fixed_size_vector<T, Capacity>::data() {
  return get_storage();
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_pointer
fixed_size_vector<T, Capacity>::data() const {
  return get_storage();
}

template <typename T, std::size_t Capacity>
bool fixed_size_vector<T, Capacity>::empty() const {
  return current_size == 0u;
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::iterator
fixed_size_vector<T, Capacity>::insert(iterator pos, const value_type &value) {
  if (current_size == capacity_size) throw std::bad_alloc{};
  for (auto iter = end(); iter != pos; --iter) {
    *iter = std::move(*(iter - 1));
  }
  *pos = value;
  ++current_size;
  return pos;
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::iterator
fixed_size_vector<T, Capacity>::insert(iterator pos, value_type &&value) {
  if (current_size == capacity_size) throw std::bad_alloc{};
  for (auto iter = end(); iter != pos; --iter) {
    *iter = std::move(*(iter - 1));
  }
  *pos = std::move(value);
  ++current_size;
  return pos;
}

template <typename T, std::size_t Capacity>
void fixed_size_vector<T, Capacity>::clear() {
  for (value_type &item : *this) {
    item.~value_type();
  }
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::iterator
fixed_size_vector<T, Capacity>::erase(iterator pos) {
  pos->~value_type();
  const_iterator last = end() - 1;
  for (auto iter = pos; iter != last; ++iter) {
    *(iter) = std::move(*(iter + 1));
  }
  --current_size;
  return pos;
}

template <typename T, std::size_t Capacity>
void fixed_size_vector<T, Capacity>::pop_back() {
  erase(end() - 1);
}

template <typename T, std::size_t Capacity>
template <typename U>
U fixed_size_vector<T, Capacity>::cast_pointer(storage_type *p) {
  return reinterpret_cast<U>(p);
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::iterator
fixed_size_vector<T, Capacity>::get_storage() {
  return cast_pointer<iterator>(storage);
}

template <typename T, std::size_t Capacity>
typename fixed_size_vector<T, Capacity>::const_iterator
fixed_size_vector<T, Capacity>::get_storage() const {
  return cast_pointer<const_iterator>(const_cast<storage_type *>(storage));
}

template <typename T, std::size_t Capacity>
constexpr typename fixed_size_vector<T, Capacity>::size_type
fixed_size_vector<T, Capacity>::capacity() {
  return capacity_size;
}

template <typename T, std::size_t Capacity>
constexpr typename fixed_size_vector<T, Capacity>::size_type
fixed_size_vector<T, Capacity>::max_size() {
  return capacity_size;
}
}  // namespace utils
