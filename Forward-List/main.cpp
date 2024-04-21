#include "my_forward_list.hpp"

int main(int argc, char* argv[]) {
  my::forward_list<int> lst(5, 1);
  std::cout << "Size: " << lst.size() << std::endl;
  lst.print();
  
  return 0;
}