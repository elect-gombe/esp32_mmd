#include "vector2.hpp"
vector2::vector2(){
  x=0;
  y=0;
}

vector2::vector2(int x,int y){
  this->x = x;
  this->y = y;
}

vector2::vector2(const vector2& v){
  this->x = v.x;
  this->y = v.y;
}

vector2& vector2::operator=(const vector2& v){
  this->x = v.x;
  this->y = v.y;
  return *this;
}

vector2 vector2::operator+(){
  return *this;
}
vector2 vector2::operator-(){
  return vector2(-x,-y);
}

vector2& vector2::operator+=(const vector2& v){
  this->x += v.x;
  this->y += v.y;
  return *this;
}

vector2& vector2::operator-=(const vector2& v){
  this->x -= v.x;
  this->y -= v.y;
  return *this;
}

vector2 operator+(const vector2& v1,const vector2& v2){
  vector2 w;

  w.x = v1.x+v2.x;
  w.y = v1.y+v2.y;

  return w;
}

vector2 operator*(const vector2& v1,int n){
  vector2 w;

  w.x = v1.x*n;
  w.y = v1.y*n;

  return w;
}

vector2 operator-(const vector2& v1,const vector2& v2){
  vector2 w;

  w.x = v1.x-v2.x;
  w.y = v1.y-v2.y;

  return w;
}

/* warning this is mul by 256 */
vector2& vector2::operator*=(int m){
  this->x *= m;
  this->x /= 256;
  this->y *= m;
  this->y /= 256;

  return *this;
}

// void vector2::print(void){
// #ifdef _STRFUNC
//   xprintf("(%d,%d)\n",x,y);
// #else
//   printf("(%d,%d)\n",x,y);
// #endif
// }
