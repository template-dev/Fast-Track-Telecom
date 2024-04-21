#ifndef FAST_TRACK_TELECOM_FORWARD_LIST_HPP
#define FAST_TRACK_TELECOM_FORWARD_LIST_HPP

#include <initializer_list>
#include <iostream>
#include <memory>

namespace my {
  template<typename T>
  class forward_list {
  public:
    forward_list() noexcept;
    explicit forward_list(const size_t &count) noexcept;
    forward_list(const size_t &count, const T &value) noexcept;
    forward_list(const std::initializer_list <T> &init) noexcept;
    forward_list(const forward_list &other) noexcept;
    forward_list(forward_list &&other) noexcept;
    forward_list &operator=(const forward_list &other) noexcept;
    forward_list &operator=(forward_list &&other) noexcept;
    
    void push_back(const T &value);
    void push_front(const T &value);
    void insert(const size_t &pos, const T &value);
    
    void pop_back();
    void pop_front();
    void erase(const T &value);
    void clear();
    
    size_t size() const { return size_; }
    bool isEmpty() const { return size_ == 0; };
    
    void print() const;
    
    T front() const { return pHead_->value_; }
    T back() const { return pLast_->value_; }
    
    T& operator*() const {
      return pNode_->value_;
    }
    
    ~forward_list() noexcept;
    
  private:
    struct Node {
      Node(T value = T{}, Node *next = nullptr) noexcept
        : value_{value}
        , pNext_{next}
      {}
      T value_;
      std::unique_ptr <Node> pNext_;
    };
    std::unique_ptr<Node> pHead_;
    Node *pLast_;
    size_t size_;
  };
}
  
#include "my_forward_list.inl"

#endif // FAST_TRACK_TELECOM_FORWARD_LIST_HPP
