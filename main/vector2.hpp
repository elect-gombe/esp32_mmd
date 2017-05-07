#ifndef __VECTOR2
#define __VECTOR2

class vector2{
public:
  int x;
  int y;

  vector2(int x,int y);
  vector2(const vector2& v);
  vector2();
  vector2& operator=(const vector2& v);
  vector2 operator+();
  vector2 operator-();
  vector2& operator+=(const vector2& v);
  vector2& operator-=(const vector2& v);
  vector2& operator*=(int m);
  void print(void);
};

vector2 operator+(const vector2& v1,const vector2& v2);
vector2 operator*(const vector2& v1,int n);
vector2 operator-(const vector2& v1,const vector2& v2);

#endif
