/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/
#ifndef __FVECTOR3_H
#define __FVECTOR3_H
#include <stdint.h>
#include <math.h>

inline
int sqrt2(int f);

struct fvector3_t{
  float x;
  float y;
  float z;
};

class fvector3{
public:
  float x;
  float y;
  float z;
  fvector3(const float x,const float y,const float z){
    this->x = x;
    this->y = y;
    this->z = z;
  }
  fvector3(const fvector3_t& v){
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
  }
  fvector3(const float x,const float y){
    this->x = x;
    this->y = y;
    this->z = 0;
  }
  fvector3(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }

  fvector3& operator*=(const float p){
    this->x = this->x*p;
    this->y = this->y*p;
    this->z = this->z*p;
    return *this;
  }
  fvector3& operator+=(const fvector3& v){
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
  }
  
  fvector3& operator-=(const fvector3& v){
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
  }

  fvector3 operator+(){return *this;}
  fvector3 operator-(){
    fvector3 v;
    v.x = -this->x;
    v.y = -this->y;
    v.z = -this->z;
    return v;
  }


  float abs(){
    return sqrtf(x*x+y*y+z*z);
  }

  float sqabs(){
    return (x*x+y*y+z*z);
  }

  fvector3& normalize(){
    float r = 1.f/abs();
    x *= r;
    y *= r;
    z *= r;
    return *this;
  }
  
  void print();
  fvector3& operator=(const fvector3& v){
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    return *this;
  }
};


static inline
fvector3 cross(fvector3 p1,fvector3 p2){
  fvector3 v;

  v.x = p1.y*p2.z - p1.z*p2.y;
  v.y = p1.z*p2.x - p1.x*p2.z;
  v.z = p1.x*p2.y - p1.y*p2.x;
  return v;
}

static inline
fvector3 operator+(const fvector3 &v1,const fvector3& v2){
  fvector3 w;

  w.x = v1.x+v2.x;
  w.y = v1.y+v2.y;
  w.z = v1.z+v2.z;
  return w;
}

static inline
float operator*(const fvector3 &v1,const fvector3 &v2){
  return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

static inline
fvector3 operator*(const fvector3 &v1,const float n){
  fvector3 w;

  w.x = v1.x*n;
  w.y = v1.y*n;
  w.z = v1.z*n;
  return w;
}

static inline
fvector3 operator-(const fvector3 &v1,const fvector3& v2){
  fvector3 w;

  w.x = v1.x-v2.x;
  w.y = v1.y-v2.y;
  w.z = v1.z-v2.z;
  return w;
}

inline
fvector3 calc_nv(const fvector3 v[3]){
  fvector3 n;
  float rabs;

  n = cross(v[1]-v[0],v[2]-v[0]);
  rabs = 1.f/n.abs();

  n.x = n.x * rabs;
  n.y = n.y * rabs;
  n.z = n.z * rabs;

  return n;
}

#endif
