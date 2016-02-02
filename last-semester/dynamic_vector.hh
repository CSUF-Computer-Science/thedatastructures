
#pragma once

#include <cassert>

#include "exceptions.hh"
#include "fixed_vector.hh"

template <typename ELEMENT>
class DynamicVector {
public:
  DynamicVector() {
    // start out empty with capacity 1
    _size = 0;
    _storage = new FixedVector<ELEMENT>(1);
  }

  ~DynamicVector() {
    delete _storage;
  }

  int size() {
    return _size;
  }

  bool is_empty() {
    return (0 == _size);
  }

  ELEMENT get(int index) throw(IndexException) {
    check_index(index);
    return _storage->get(index);
  }

  void set(int index, ELEMENT e) throw(IndexException) {
    check_index(index);
    _storage->set(index, e);
  }

  void add_back(ELEMENT e) {
    // n < c
    assert(_size < current_capacity());

    _storage->set(_size, e);
    _size++;

    maintain();
  }

  void remove_back() throw(UnderflowException) {
    if (is_empty())
      throw UnderflowException();

    _size--;

    maintain();
  }

  void clear() {
    _size = 0;
    maintain();
    assert(1 == current_capacity());
  }

private:
  FixedVector<ELEMENT> *_storage;
  int _size;

  void check_index(int index) throw(IndexException) {
    if ((index < 0) || (index >= _size))
      throw IndexException(index, _size);
  }

  int current_capacity() {
    return _storage->capacity();
  }

  // Guarantee that capacity invariants are met, by resizing only when necessary.
  void maintain() {
    int c = current_capacity();
    if (_size == c) {
      // grow to double current capacity
      resize(c * 2);
    } else if (_size == 0) {
      // shrink to capacity 1
      resize(1);
    } else if (3*_size < c) {
      // shrink to half current capacity
      resize(c / 2);
    } else {
      // no maintenance required
    }
  }
    
  void resize(int new_capacity) {
    assert(new_capacity > 0);
    assert(new_capacity >= _size);

    // allocate new fixed vector
    FixedVector<ELEMENT> *new_storage = new FixedVector<ELEMENT>(new_capacity);

    // copy elements over to new vector
    for (int i = 0; i < _size; ++i)
      new_storage->set(i, _storage->get(i));

    // free old vector, start using new vector
    delete _storage;
    _storage = new_storage;
  }

};