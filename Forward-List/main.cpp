#include "my_forward_list.hpp"

int main(int argc, char* argv[]) {
  my::forward_list<int> lst;
  lst.push_front(5);
  lst.push_front(0);
  lst.push_back(10);
  
  lst.pop_front();
  lst.pop_back();
  
  lst.print();
  std::cout << "Size: " << lst.size() << std::endl;
  
  return 0;
}