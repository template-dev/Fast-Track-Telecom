#include "forward_list.hpp"

template<typename T>
forward_list<T>::forward_list(const T& value /*= T{}*/) noexcept
  : pHead_{nullptr}
  , count_{0}
{
  pHead_ = new Node<T>(value);
  ++count_;
}

template<typename T>
void forward_list<T>::push_back(const T& value) {
  if(!pHead_) {
    pHead_ = new Node<T>(value);
  } else {
    Node<T>* pCurrent = pHead_;
    while(pCurrent->next_) {
      pCurrent = pCurrent->next_;
    }
    pCurrent->next_ = new Node<T>(value);
  }
  ++count_;
}

template<typename T>
void forward_list<T>::push_front(const T& value) {
  pHead_ = new Node<T>(value, pHead_);
  ++count_;
}

template<typename T>
void forward_list<T>::print() const {
  Node<T>* pCurrent = pHead_;
  while(pCurrent) {
    std::cout << pCurrent->data_ << " ";
    pCurrent = pCurrent->next_;
  }
  std::cout << std::endl;
}