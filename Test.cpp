// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "include/stackcomm.hpp"


int main() {
  std::cout << "Starting program" << std::endl;
  
  int serial_port = stackOpen();

  if (serial_port < 0) return 1;

  int move = stackMoveDiagonal(serial_port, 2.0, 0.4, 0, 3.0, 1.0, 1);
  if (move < 0) return 1;
   
  stackClose(serial_port);
  
  return 0; // success
}