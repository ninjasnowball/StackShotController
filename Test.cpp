// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "include/rigcontroller.hpp"

int main() {
  std::cout << "Starting program" << std::endl;

  RigController controller;

  controller.OpenStack();
  
  int current_state = getState();

  for (int i = 0; i < 12; i++){
    current_state = controller.nextPosition();
    std::this_thread::sleep_for(std::chrono::seconds(5))
  }

   
  controller.CloseStack();
  
  return 0; // success
}