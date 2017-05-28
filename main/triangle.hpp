#ifndef __TRIANGLE_H
#define __TRIANGLE_H
#include "vector3.hpp"
#include "fvector4.hpp"

class triangle{
public:
  // top point.
  float ymiddle;
  float col;
  uint8_t ymin;
  uint8_t ymax;
  uint8_t yno;
  uint8_t phase;
  float delta[2][2];
  float zdelta[2][2];
  float pdx[2];
  float pdz[2];
  // int z_val;
  // int dxdz;
  // int dydz;

  int draw(float *zlinebuf,graphiclib &g);  
  int triangle_set(fvector4 px[3],float col);

  int cymin();
  int cymax();
};
#endif
