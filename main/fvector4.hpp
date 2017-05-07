#ifndef __FVECTOR4_H
#define __FVECTOR4_H
#include <stdint.h>
#include "fvector3.hpp"

class fvector4{
public:
  float x;
  float y;
  float z;
  float w;
  fvector4(float x,float y,float z,float w){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  fvector4(float x,float y,float z){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 0;
  }
  fvector4(float x,float y){
    this->x = x;
    this->y = y;
    this->z = 0;
    this->w = 0;
  }
  fvector4(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }

  fvector4& operator*=(float p){
    this->x = (this->x)*p;
    this->y = (this->y)*p;
    this->z = (this->z)*p;
    this->w = (this->w)*p;
    return *this;
  }

  fvector4& operator+=(const fvector4& v){
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    this->w += v.w;
    return *this;
  }
  
  fvector4& operator-=(const fvector4& v){
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    this->w -= v.w;
    return *this;
  }

  fvector4 operator+(){return *this;}
  fvector4 operator-(){
    fvector4 v;
    v.x = -this->x;
    v.y = -this->y;
    v.z = -this->z;
    v.w = -this->w;
    return v;
  }
};

static inline
fvector4 operator+(const fvector4 &v1,const fvector4& v2){
  fvector4 r;

  r.x = v1.x+v2.x;
  r.y = v1.y+v2.y;
  r.z = v1.z+v2.z;
  r.w = v1.w+v2.w;

  return r;
}

static inline
fvector4 operator*(const fvector4 &v1,float n){
  fvector4 w;

  w.x = v1.x*n;
  w.y = v1.y*n;
  w.z = v1.z*n;
  w.w = v1.w*n;
  return w;
}

static inline
fvector4 operator-(const fvector4 &v1,const fvector4& v2){
  fvector4 w;

  w.x = v1.x-v2.x;
  w.y = v1.y-v2.y;
  w.z = v1.z-v2.z;
  w.w = v1.w-v2.w;
  
  return w;
}

inline
fvector3 calc_nv(const fvector4 v[3]){
  fvector3 n;
  fvector4 a,b;
  float rabs;

  a = v[1]-v[0];
  b = v[2]-v[0];

  n = cross(fvector3(a.x,a.y,a.z),fvector3(b.x,b.y,b.z));
  rabs = 1.f/n.abs();

  n.x = n.x * rabs;
  n.y = n.y * rabs;
  n.z = n.z * rabs;

  return n;
}

#endif
