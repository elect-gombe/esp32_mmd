#ifndef __FVECTOR2
#define __FVECTOR2

struct fvector2_t{
  float x;
  float y;
};

struct fvector2{
  float x;
  float y;

  fvector2(){
    x=0;
    y=0;
  }

  fvector2(float x,float y){
    this->x = x;
    this->y = y;
  }

  fvector2(const fvector2& v){
    this->x = v.x;
    this->y = v.y;
  }

  fvector2& operator=(const fvector2& v){
    this->x = v.x;
    this->y = v.y;
    return *this;
  }

  fvector2 operator+(){
    return *this;
  }
  fvector2 operator-(){
    return fvector2(-x,-y);
  }

  fvector2& operator+=(const fvector2& v){
    this->x += v.x;
    this->y += v.y;
    return *this;
  }

  fvector2& operator-=(const fvector2& v){
    this->x -= v.x;
    this->y -= v.y;
    return *this;
  }

  /* warning this is mul by 256 */
  fvector2& operator*=(float m){
    this->x *= m;
    this->x /= 256;
    this->y *= m;
    this->y /= 256;

    return *this;
  }
  void print(void);
};

inline
fvector2 operator+(const fvector2& v1,const fvector2& v2){
  fvector2 w;

  w.x = v1.x+v2.x;
  w.y = v1.y+v2.y;

  return w;
}

inline
fvector2 operator*(const fvector2& v1,float n){
  fvector2 w;

  w.x = v1.x*n;
  w.y = v1.y*n;

  return w;
}

inline
fvector2 operator-(const fvector2& v1,const fvector2& v2){
  fvector2 w;

  w.x = v1.x-v2.x;
  w.y = v1.y-v2.y;

  return w;
}

fvector2 operator+(const fvector2& v1,const fvector2& v2);
fvector2 operator*(const fvector2& v1,float n);
fvector2 operator-(const fvector2& v1,const fvector2& v2);

#endif
