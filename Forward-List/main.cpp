#include "my_forward_list.hpp"

int main(int argc, char* argv[]) {
  my::forward_list<int> lst(5, 1);
  std::cout << "LST1: ";
  lst.print();
  std::cout << "Lst1 size: " << lst.size() << std::endl;
  auto lst2 = std::move(lst);
  lst2.pop_front();
  lst2.print();
  
  std::cout << "Size: " << lst2.size() << std::endl;
  std::cout << "Lst1 size: " << lst.size() << std::endl;
  
  return 0;
}