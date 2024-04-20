#include "forward_list.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  forward_list<int> lst;
  lst.push_back(5);
  lst.push_back(10);
  //lst.push_front(0);
  lst.print();
  
  return 0;
}