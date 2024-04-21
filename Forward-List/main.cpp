#include "my_forward_list.hpp"

int main(int argc, char* argv[]) {
  my::forward_list<int> lst{1, 2, 3, 4, 5};
  std::cout << lst << std::endl;
  lst.pop_front();
  lst.erase(2);
  std::cout << (lst.find(3) ? "Found" : "Not Found") << std::endl;
  
  std::cout << "[List 1]" << std::endl;
  lst.print();
  std::cout << "Front: " << lst.front() << ", last: " << lst.back() << std::endl;
  std::cout << "Size: " << lst.size() << std::endl;
  std::cout << (lst.isEmpty() ? "Empty" : "Not Empty") << std::endl;
  
  return 0;
}