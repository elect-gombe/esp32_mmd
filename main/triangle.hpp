/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/
#ifndef __TRIANGLE_H
#define __TRIANGLE_H
#include "vector3.hpp"
#include "fvector4.hpp"

class triangle{
public:
  uint8_t isexisting;
  // top point.
  float ymiddle;
  float col;
  uint8_t ymin;
  uint16_t ymax;
  uint8_t yno;
  uint8_t phase;
  float delta[2][2];
  int16_t zdelta[2][2]; //
  float pdx[2];
  uint16_t pdz[2];
  // int z_val;
  // int dxdz;
  // int dydz;

  int draw(float *zlinebuf,graphiclib &g);  
  int triangle_set(fvector4 px[3],float col);

  int cymin();
  int cymax();
};
#endif
