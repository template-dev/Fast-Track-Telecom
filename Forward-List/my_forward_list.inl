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
    for(size_t i = 0; i < count; ++i) {
      Node <T> *pNewNode = new Node<T>(value);
      if (!pHead_) {
        pHead_ = pNewNode;
        pLast_ = pNewNode;
      }
      else {
        pLast_->pNext_ = pNewNode;
        pLast_ = pNewNode;
      }
      ++size_;
    }
  }
  
  template<typename T>
  forward_list<T>::forward_list(std::initializer_list <T> init) noexcept
    : pHead_(nullptr)
    , pLast_(nullptr)
    , size_(init.size()) {
    for(auto it = init.begin(); it != init.end(); ++it) {
      Node<T> *pNewNode = new Node<T>(*it);
      if (!pHead_) {
        pHead_ = pNewNode;
        pLast_ = pNewNode;
      } else {
        pLast_->pNext_ = pNewNode;
        pLast_ = pNewNode;
      }
    }
  }
  
  template<typename T>
  forward_list<T>::forward_list(const forward_list <T> &other) noexcept
    : forward_list() {
    if (!other.pHead_) {
      return;
    }
    
    Node <T> *tmp = nullptr;
    for(Node <T> begin = other.pHead_; begin != nullptr; begin = begin->pNext_ ) {
      Node *item = new Node(begin->value_);
      if(!pHead_) {
        pHead_ = item;
      } else {
        tmp->pNext_ = item;
      }
      tmp = item;
      if (!begin->pNext_) {
        pLast_ = item;
      }
    }
    size__ = other.size_;
  }
  
  template<typename T>
  void forward_list<T>::push_back(const T &value) {
    Node <T> *pNewNode = new Node<T>(value);
    if (!pHead_) pHead_ = pNewNode;
    if (pLast_) pLast_->pNext_ = pNewNode;
    pLast_ = pNewNode;
    ++size_;
  }
  
  template<typename T>
  void forward_list<T>::push_front(const T &value) {
    Node <T> *pNewNode = new Node<T>(value);
    pNewNode->pNext_ = pHead_;
    pHead_ = pNewNode;
    if (!pLast_) {
      pLast_ = pNewNode;
    }
    ++size_;
  }
  
  template<typename T>
  void forward_list<T>::pop_back() {
    if (!pHead_) {
      return;
    }
    
    if (pHead_ == pLast_) {
      delete pHead_;
      pHead_ = nullptr;
      pLast_ = nullptr;
      --size_;
      return;
    }
    
    Node <T> *prev = pHead_;
    for (; prev->pNext_ != pLast_; prev = prev->pNext_);
    
    prev->pNext_ = nullptr;
    delete pLast_;
    pLast_ = prev;
    --size_;
  }
  
  template<typename T>
  void forward_list<T>::pop_front() {
    if (!pHead_) {
      return;
    }
    
    Node <T> *pCur = pHead_;
    pHead_ = pCur->pNext_;
    delete pCur;
    --size_;
  }
  
  template<typename T>
  void forward_list<T>::erase(const T &value) {
    if (!pHead_) {
      return;
    }
    
    if (pHead_->value_ == value) {
      pop_front();
      return;
    } else if (pLast_->value_ == value) {
      pop_back();
      return;
    }
    
    Node <T> *slow = pHead_;
    Node <T> *fast = pHead_->pNext_;
    
    while (fast && (fast->value_ != value)) {
      fast = fast->pNext_;
      slow = slow->pNext_;
    }
    
    if (!fast) {
      return;
    }
    
    slow->pNext_ = fast->pNext_;
    delete fast;
  }
  
  template<typename T>
  void forward_list<T>::insert(const size_t &pos, const T &value) {
    if ((!pHead_ && (pos > size_ || pos < 0)) || (pHead_ && (pos > size_ || pos < 0))) {
      return;
    }
    
    if (pos == 0) {
      push_front(value);
    } else {
      Node <T> *prev = pHead_;
      for (int i = 0; i < pos - 1; ++i) {
        prev = prev->pNext_;
      }
      prev->pNext_ = new Node<T>(value, prev->pNext_);
      ++size_;
      
      if (prev == pLast_) {
        pLast_ = prev->pNext_;
      }
    }
  }
  
  template<typename T>
  void forward_list<T>::print() const {
    if (isEmpty()) {
      return;
    }
    
    Node <T> *pCurrent = pHead_;
    while (pCurrent) {
      std::cout << pCurrent->value_ << " ";
      pCurrent = pCurrent->pNext_;
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

  template<typename T>
  std::ostream &operator<<(std::ostream &s, const forward_list<T> &v) {
    s.put('{');
    if (!v.isEmpty()) {
      auto current = v.pHead_;
      while (current) {
        s << current->value_;
        if (current->pNext_) {
          s << ", ";
        }
        current = current->pNext_;
      }
    }
    return s << "}\n";
  }
}