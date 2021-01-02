#include "DynArray.h"
#include <stdexcept>

// Kontruktor, Kopierkonstruktor, Destruktor und Zuweisungsoperator
template <class T> DynArray<T>::DynArray(const int &paramLimit) {
  limit = paramLimit;
}
template <class T> DynArray<T>::DynArray(const DynArray<T> &copy) {
  cntElements = copy.cntElements;
  limit = copy.limit;
  resize = copy.resize;
  arrPtr = new T[copy.limit];

  for (int i = 0; i < copy.cntElements; i++) {
    arrPtr[i] = copy.arrPtr[i]; // call assignment operator of T
  }
}
template <class T> DynArray<T>::~DynArray() { Free(); }
template <class T>
DynArray<T> &DynArray<T>::operator=(const DynArray<T> &copy) {
  if (this != &copy) {
    Free();
    limit = copy.limit;
    cntElements = copy.cntElements;
    resize = copy.resize;
    arrPtr = new T[copy.limit];

    for (int i = 0; i < copy.cntElements; i++) {
      arrPtr[i] = copy.arrPtr[i]; // call assignment operator of T
    }
  }

  return *this;
}
// Methoden
template <class T> void DynArray<T>::Add(const T &element) {
  if (arrPtr == nullptr)
    InitArr();
  if (cntElements == limit)
    Resize();

  arrPtr[cntElements] = element; // no need to increment before, because arrays
                                 // start at 0. Assignment operator of T called
  cntElements += 1;
}
template <class T> void DynArray<T>::Set(const T &element, const int &pos) {
  CheckAgainstLimit(pos);
  arrPtr[pos] = element; // assignment operator of T
}
template <class T>
// normally this should also return const, but because of the possibility that
// an exception gets thrown it does not
T DynArray<T>::Get(const int &pos) {
  CheckAgainstLimit(pos);
  return arrPtr[pos];
}

template <class T> void DynArray<T>::Resize() {
  int newSize = limit + resize;
  T *newArrPtr = new T[newSize];

  for (int i = 0; i < cntElements; i++) {
    newArrPtr[i] = arrPtr[i]; // call assignment operator of T
  }
  Free();
  arrPtr = newArrPtr;
  limit = newSize;
}
template <class T> void DynArray<T>::InitArr() { arrPtr = new T[limit]; }
template <class T> void DynArray<T>::Free() {
  delete[] arrPtr;
  arrPtr = nullptr;
}
template <class T> void DynArray<T>::CheckAgainstLimit(const int &pos) {
  if (pos >= limit)
    throw std::out_of_range("invalid index");
}
template <class T> const T *DynArray<T>::GetArrPtr() const { return arrPtr; }

template <class T> int DynArray<T>::GetCntElements() const {
  return cntElements;
}
template <class T> int DynArray<T>::GetLimit() const { return limit; }
