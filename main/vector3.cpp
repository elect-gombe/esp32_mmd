#include "vector3.hpp"
// #include <stdio.h>
// #include <iostream>

/*This is fast calculation modules.*/
inline
int sqrt2(int f) {
  int s = f,t;
  
  if(s == 0)return 1;
  do{
    t = s;
    s = (t + f / t) >> 1;
  }while(s < t);
  //  printf("%f:->%f\n",f/65536.,t/256.);

  return t<<8;
}

unsigned int vector3::abs(){
  return sqrt2(int64_t(x)*x/65536+int64_t(y)*y/65536+int64_t(z)*z/65536);
}

// void vector3::print(){
//   std::cout <<"(" << x/65536.<<","<<y/65536.<<","<<z/65536.<<")"<<std::endl;
// }
