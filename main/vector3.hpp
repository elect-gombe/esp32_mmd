#ifndef __VECTOR3_H
#define __VECTOR3_H
#include <stdint.h>

inline
int sqrt2(int f);

struct vector3_t{
  int x;
  int y;
  int z;
};

class vector3{
public:
  int x;
  int y;
  int z;
  vector3(int x,int y,int z){
    this->x = x;
    this->y = y;
    this->z = z;
  }
  vector3(const vector3_t& v){
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
  }
  vector3(int x,int y){
    this->x = x;
    this->y = y;
    this->z = 0;
  }
  vector3(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }

  vector3& operator*=(int p){
    this->x = ((int64_t)this->x)*p;
    this->y = ((int64_t)this->y)*p;
    this->z = ((int64_t)this->z)*p;
    return *this;
  }
  vector3& operator+=(const vector3& v){
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
  }
  
  vector3& operator-=(const vector3& v){
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
  }

  vector3 operator+(){return *this;}
  vector3 operator-(){
    vector3 v;
    v.x = -this->x;
    v.y = -this->y;
    v.z = -this->z;
    return v;
  }
  unsigned int abs();
  void print();
};

static inline
vector3 cross(vector3 p1,vector3 p2){
  vector3 v;

  v.x = int64_t(p1.y)*p2.z/65536 - int64_t(p1.z)*p2.y/65536;
  v.y = int64_t(p1.z)*p2.x/65536 - int64_t(p1.x)*p2.z/65536;
  v.z = int64_t(p1.x)*p2.y/65536 - int64_t(p1.y)*p2.x/65536;
  return v;
}

static inline
vector3 operator+(const vector3 &v1,const vector3& v2){
  vector3 w;

  w.x = v1.x+v2.x;
  w.y = v1.y+v2.y;
  w.z = v1.z+v2.z;
  return w;
}

static inline
int operator*(const vector3 &v1,const vector3 &v2){
  return (int64_t)v1.x*v2.x/65536+(int64_t)v1.y*v2.y/65536+(int64_t)v1.z*v2.z/65536;
}

static inline
vector3 operator*(const vector3 &v1,int n){
  vector3 w;

  w.x = v1.x*n;
  w.y = v1.y*n;
  w.z = v1.z*n;
  return w;
}

static inline
vector3 operator-(const vector3 &v1,const vector3& v2){
  vector3 w;

  w.x = v1.x-v2.x;
  w.y = v1.y-v2.y;
  w.z = v1.z-v2.z;
  return w;
}

inline
vector3 calc_nv(const vector3 v[3]){
  vector3 n;
  int abs_n;
  int rabs;

  n = cross(v[1]-v[0],v[2]-v[0]);
  abs_n = n.abs();

  rabs = 4294967295/abs_n;
  n.x = (int64_t)n.x * rabs / 65536;
  n.y = (int64_t)n.y * rabs / 65536;
  n.z = (int64_t)n.z * rabs / 65536;

  return n;
}

#endif
