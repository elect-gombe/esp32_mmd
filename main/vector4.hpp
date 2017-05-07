#ifndef __VECTOR4_H
#define __VECTOR4_H
#include <stdint.h>

class vector4{
public:
  int x;
  int y;
  int z;
  int w;
  vector4(int x,int y,int z,int w){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  vector4(int x,int y,int z){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 0;
  }
  vector4(int x,int y){
    this->x = x;
    this->y = y;
    this->z = 0;
    this->w = 0;
  }
  vector4(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }

  vector4& operator*=(int p){
    this->x = ((int64_t)this->x)*p;
    this->y = ((int64_t)this->y)*p;
    this->z = ((int64_t)this->z)*p;
    this->w = ((int64_t)this->w)*p;
    return *this;
  }

  vector4& operator+=(const vector4& v){
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    this->w += v.w;
    return *this;
  }
  
  vector4& operator-=(const vector4& v){
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    this->w -= v.w;
    return *this;
  }

  vector4 operator+(){return *this;}
  vector4 operator-(){
    vector4 v;
    v.x = -this->x;
    v.y = -this->y;
    v.z = -this->z;
    v.w = -this->w;
    return v;
  }
};

static inline
vector4 operator+(const vector4 &v1,const vector4& v2){
  vector4 r;

  r.x = v1.x+v2.x;
  r.y = v1.y+v2.y;
  r.z = v1.z+v2.z;
  r.w = v1.w+v2.w;

  return r;
}

static inline
vector4 operator*(const vector4 &v1,int n){
  vector4 w;

  w.x = v1.x*n;
  w.y = v1.y*n;
  w.z = v1.z*n;
  w.w = v1.w*n;
  return w;
}

static inline
vector4 operator-(const vector4 &v1,const vector4& v2){
  vector4 w;

  w.x = v1.x-v2.x;
  w.y = v1.y-v2.y;
  w.z = v1.z-v2.z;
  w.w = v1.w-v2.w;
  
  return w;
}

#endif
