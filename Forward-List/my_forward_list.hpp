#ifndef FAST_TRACK_TELECOM_FORWARD_LIST_HPP
#define FAST_TRACK_TELECOM_FORWARD_LIST_HPP

#include <initializer_list>
#include <iostream>

namespace my {
  template<typename T>
  class forward_list {
  public:
    forward_list() noexcept;
    explicit forward_list(const size_t &count) noexcept;
    explicit forward_list(const size_t &count, const T& value) noexcept;
    forward_list(std::initializer_list <T> init) noexcept;
    
    forward_list(const forward_list &other) noexcept; // -
    forward_list(forward_list &&other) noexcept; // -
    
    forward_list &operator=(const forward_list &other) noexcept; // -
    forward_list &operator=(forward_list &&other) noexcept; // -
    
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
    
    friend std::ostream &operator<<(std::ostream &s, const forward_list<T> &v);
    
    ~forward_list() noexcept;
  
  private:
    template<typename N>
    struct Node {
      Node(N value = N{}, Node *next = nullptr) noexcept
        : value_{value}
        , pNext_{next}
      {}
      N value_;
      Node *pNext_;
    };
    Node<T> *pHead_;
    Node<T> *pLast_;
    size_t size_;
  };
}

#include "my_forward_list.inl"

#endif // FAST_TRACK_TELECOM_FORWARD_LIST_HPP
