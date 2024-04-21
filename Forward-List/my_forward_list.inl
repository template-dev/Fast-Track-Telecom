#include "my_forward_list.hpp"

namespace my {
  template<typename T>
  forward_list<T>::forward_list() noexcept
    : pHead_(nullptr)
    , pLast_(nullptr)
    , size_(0)
  {}

  template<typename T>
  forward_list<T>::forward_list(const size_t &count) noexcept
    : forward_list(count, T())
  {}

  template<typename T>
  forward_list<T>::forward_list(const size_t &count, const T& value) noexcept
    : forward_list() {
      for (size_t i = 0; i < count; ++i) {
        push_back(value);
      }
  }

  template<typename T>
  forward_list<T>::forward_list(const std::initializer_list<T>& init) noexcept
    : forward_list() {
    for (auto it = init.begin(); it != init.end(); ++it) {
      push_back(*it);
    }
    size_ = init.size();
  }

  template<typename T>
  forward_list<T>::forward_list(const forward_list &other) noexcept
    : forward_list() {
    if (&other == this || !other.pHead_) {
      return;
    }
    Node* current = other.pHead_.get();
    while(current) {
      push_back(current->value_);
      current = current->pNext_.get();
    }
    size_ = other.size_;
  }

  template<typename T>
  forward_list<T>::forward_list(forward_list &&other) noexcept
    : forward_list() {
    if (&other == this || !other.pHead_) {
      return;
    }
    std::swap(pHead_, other.pHead_);
    std::swap(pLast_, other.pLast_);
    std::swap(size_, other.size_);
  }

  template<typename T>
  forward_list<T> &forward_list<T>::operator=(const forward_list &other) noexcept {
    if (this == &other) {
      return *this;
    }
    clear();
    forward_list<T> tempList(other);
    Node<T>* tempHead = tempList.pHead_;
    while (tempHead) {
      push_back(tempHead->value_);
      tempHead = tempHead->pNext_;
    }
    pLast_ = tempHead.pLast_;
    return *this;
  }

  template<typename T>
  forward_list<T> & forward_list<T>::operator=(forward_list &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    forward_list<T> temp(std::move(other));
    std::swap(pHead_, temp.pHead_);
    std::swap(pLast_, temp.pLast_);
    std::swap(size_, temp.size_);
    return *this;
  }

  template<typename T>
  void forward_list<T>::push_back(const T &value) {
    std::unique_ptr<Node> pNewNode = std::make_unique<Node>(value);
    if (!pHead_) {
      pHead_ = std::move(pNewNode);
      pLast_ = pHead_.get();
    } else {
      pLast_->pNext_ = std::move(pNewNode);
      pLast_ = pLast_->pNext_.get();
    }
    ++size_;
  }

  template<typename T>
  void forward_list<T>::push_front(const T &value) {
    std::unique_ptr<Node> newNode = std::make_unique<Node>(value);
    if (!pHead_) {
      pHead_ = std::move(newNode);
      pLast_ = pHead_.get();
    } else {
      newNode->pNext_ = std::move(pHead_);
      pHead_ = std::move(newNode);
    }
    ++size_;
  }

  template<typename T>
  void forward_list<T>::pop_back() {
    if (!pHead_) {
      return;
    }

    if (pHead_.get() == pLast_) {
      pHead.reset();
      pLast_ = nullptr;
      --size_;
      return;
    }

    Node *prev = pHead_.get();
    for (; prev->pNext_.get() != pLast_; prev = prev->pNext_.get());
    prev->pNext_.reset();
    pLast_ = prev;
    --size_;
  }

  template<typename T>
  void forward_list<T>::pop_front() {
    if (!pHead_) {
      return;
    }
    std::unique_ptr<Node> tempHead = std::move(pHead_);
    pHead_ = std::move(tempHead->pNext_);
    --size_;
    tempHead.reset();
  }

  template<typename T>
  void forward_list<T>::erase(const T &value) {
    if (!pHead_) {
      return;
    }
    
    Node* current = pHead_.get();
    Node* prev = nullptr;
    
    while (current) {
      if (current->value_ == value) {
        if (current == pHead_.get()) {
          pop_front();
        } else {
          prev->pNext_ = std::move(current->pNext_);
          
          delete current;
        }
        --size_;
        return;
      }
      prev = current;
      current = current->pNext_.get();
    }
  }

  template<typename T>
  void forward_list<T>::insert(const size_t &pos, const T &value) {
    if ((!pHead_ && (pos > size_ || pos < 0)) || (pHead_ && (pos > size_ || pos < 0))) {
      return;
    }
    
    std::unique_ptr<Node> newNode = std::make_unique<Node>(value);
    
    if (pos == 0 || !pHead_) {
      newNode->pNext_ = std::move(pHead_);
      pHead_ = std::move(newNode);
      if (!pLast_) {
        pLast_ = pHead_.get();
      }
    } else {
      Node* current = pHead_.get();
      for (size_t i = 0; i < pos - 1; ++i) {
        current = current->pNext_.get();
      }
      
      newNode->pNext_ = std::move(current->pNext_);
      current->pNext_ = std::move(newNode);
      
      if (!newNode->pNext_) {
        pLast_ = newNode.get();
      }
    }
    ++size_;
  }

  template<typename T>
  void forward_list<T>::print() const {
    if (isEmpty()) {
      return;
    }

    Node* pCurrent = pHead_.get();
    while (pCurrent) {
      std::cout << pCurrent->value_ << " ";
      pCurrent = pCurrent->pNext_.get();
    }
    std::cout << std::endl;
  }

  template<typename T>
  void forward_list<T>::clear() {
    while (size_) {
      pop_front();
    }
  }

  template<typename T>
  forward_list<T>::~forward_list() noexcept {
    clear();
  }
}
