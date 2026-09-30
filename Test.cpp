// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "include/rigcontroller.hpp"

int main() {
  std::cout << "Starting program" << std::endl;

  RigController controller;

  if (controller.OpenStack()== -1)return 1;
  
  int current_state = controller.getState();

  for (int i = 0; i < 12; i++){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    current_state = controller.nextPosition();
  }

   
  controller.CloseStack();
  
  return 0; // success
}