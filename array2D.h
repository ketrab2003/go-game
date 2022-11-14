#pragma once

#include <cstdio>

#include <cstddef>
#include <cstring>

template<class T>
class Array2D {
private:
  T *_vals;
  unsigned int _width, _height;
  
public:
  Array2D(const unsigned int width, const unsigned int height);
  ~Array2D();
  Array2D(const Array2D<T> &other);
  Array2D(Array2D<T> &&other) noexcept;
  Array2D<T>& operator=(const Array2D<T> &other);
  Array2D<T>& operator=(Array2D<T> &&other) noexcept;

  bool operator==(const Array2D<T> &other);

  unsigned int getWidth() const;
  unsigned int getHeight() const;
  unsigned int getArea() const;   // number of all allocated values

  T* operator[](const unsigned int x) const;
  T* operator[](const unsigned int x);
};

template<class T>
Array2D<T>::Array2D(const unsigned int width, const unsigned int height) {
  _width = width;
  _height = height;
  _vals = new T[getArea()]();
}

template<class T>
Array2D<T>::~Array2D() {
  delete [] _vals;
}

template<class T>
Array2D<T>::Array2D(const Array2D<T> &other) 
: Array2D(other.getWidth(), other.getHeight()) {

  for(int x=0; x<getWidth(); ++x) {
    for(int y=0; y<getHeight(); ++y) {
      const T value = other.getValue(x, y);
      this->setValue(x, y, value);
    }
  }
}

template<class T>
Array2D<T>::Array2D(Array2D<T> &&other) noexcept {
  _width = other.getWidth();
  _height = other.getHeight();
  _vals = other._vals;
  other._vals = nullptr;
}

template<class T>
Array2D<T>& Array2D<T>::operator=(const Array2D<T> &other) {
  if(*this == other) {  // self-assignment
    return *this;
  }

  if(getArea() != other.getArea()) {
    delete [] _vals;
    _vals = new T[other.getArea()];
  }

  _width = other.getWidth();
  _height = other.getHeight();
  // memcpy(_vals, other._vals, other.getArea());
  for(int i=0; i<getArea(); ++i) {
    _vals[i] = other._vals[i];
  }

  return *this;
}

template<class T>
Array2D<T>& Array2D<T>::operator=(Array2D<T> &&other) noexcept {
  if(*this == other) {  // self-assignment
    return *this;
  }

  _width = other.getWidth();
  _height = other.getHeight();
  delete [] _vals;
  _vals = other._vals;
  other._vals = nullptr;

  return *this;
}

template<class T>
bool Array2D<T>::operator==(const Array2D<T> &other) {
  if(_width != other._width) {
    return false;
  }
  if(_height != other._height) {
    return false;
  }
  if(_vals != other._vals) {
    return false;
  }
  return true;
}

template<class T>
unsigned int Array2D<T>::getWidth() const {
  return _width;
}

template<class T>
unsigned int Array2D<T>::getHeight() const {
  return _height;
}

template<class T>
unsigned int Array2D<T>::getArea() const {
  return getWidth()*getHeight();
}

template<class T>
T* Array2D<T>::operator[](const unsigned int x) const {
  return _vals + x*getHeight();
}

template<class T>
T* Array2D<T>::operator[](const unsigned int x) {
  return _vals + x*getHeight();
}