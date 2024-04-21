#include "my_forward_list.hpp"

int main(int argc, char* argv[]) {
  my::forward_list<int> lst(5, 1);
  my::forward_list<int> lst2;
  lst2 = std::move(lst);
  std::cout << "lst1: ";
  lst.print();
  std::cout << "lst2: ";
  lst2.print();
  
  return 0;
}