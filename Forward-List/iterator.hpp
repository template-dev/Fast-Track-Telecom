#ifndef FAST_TRACK_TELECOM_ITERATOR_HPP
#define FAST_TRACK_TELECOM_ITERATOR_HPP

namespace my {
  template<typename T>
  class Iterator {
  public:
    Iterator(T *node = nullptr) : current_(node) {}
    
    T& operator*() { return *current_; }
    
    Iterator& operator++() {
      ++current_;
      return *this;
    }
    
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    
    bool operator==(const Iterator& other) const {
      return current == other.current;
    }
    
    bool operator!=(const Iterator& other) const {
      return !(*this == other);
    }
  
  private:
    T *current_;
  };
}

#endif //FAST_TRACK_TELECOM_ITERATOR_HPP
