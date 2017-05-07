#include "fvector3.hpp"
// #include <stdio.h>
// #include <iostream>
#include <math.h>

float fvector3::abs(){
  return sqrtf(x*x+y*y+z*z);
}

// void fvector3::print(){
//   std::cout <<"(" << x<<","<<y<<","<<z<<")"<<std::endl;
// }
