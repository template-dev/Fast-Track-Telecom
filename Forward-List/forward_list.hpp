#ifndef FAST_TRACK_TELECOM_FORWARD_LIST_HPP
#define FAST_TRACK_TELECOM_FORWARD_LIST_HPP

template<typename T>
class forward_list {
public:
  forward_list();
  explicit forward_list(const T& value) noexcept;
  
  forward_list(const forward_list& other) noexcept;
  forward_list(forward_list&& other) noexcept;
  forward_list& operator=(const forward_list& other) noexcept;
  forward_list& operator=(forward_list&& other) noexcept;
  
  void push_back(const T& value);
  void push_front(const T& value);
  void pop_back();
  void pop_front();
  void insert(size_t pos, const T& value);
  void erase(const T& value);
  void clear();
  bool isEmpty() const { return count_ == 0; };
  void print() const;
  
  ~forward_list() = default;
  
private:
  template<typename N>
  struct Node {
    Node(N data = N{}, Node* next = nullptr)
      : data_{data}
      , next_{next}
    {}
    N data_;
    Node* next_;
  };
  Node<T>* pHead_;
  size_t count_;
};

#include "forward_list.inl"

#endif // FAST_TRACK_TELECOM_FORWARD_LIST_HPP
