/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/
#ifndef __TEXTURE_POLY_H
#define __TEXTURE_POLY_H
#include <stdint.h>
#include "fvector2.hpp"
#include "vector2.hpp"
#include "fvector4.hpp"
#include "projpoint.hpp"

//テクスチャ型
struct texture_t{
  const uint16_t *tx;
  // vector2 size;//未実装
};


//テクスチャのついた三角形描画はtriangleを継承しますが、ほとんどオーバーライドしています。速度の問題とかあるんで
class texturetriangle{
public:
  uint8_t isexisting;
  // top point.
  uint16_t ymin;
  uint16_t ymax;
  uint16_t yno;
  uint16_t phase;
  uint16_t ymiddle;
  float col;
  float delta[2][2];
  int32_t zdelta[2][2]; //
  float pdx[2];
  uint32_t pdz[2];
  //テクスチャデータ
  const uint16_t *tx;
  //テクスチャ内座標
  vector2 pdt[2];
  vector2 uvdelta[2][2];
  //w値の補間用
  int32_t pdw[2];
  int32_t wdelta[2][2];
public:
  texturetriangle(){}
  int triangle_set(fvector4 px[3],float c,const texture_t *t,const fvector2 puv[3]);
  int draw(uint16_t *zlinebuf,uint16_t *buff,int dry);
  static bool LessZ(const texturetriangle& rLeft, const texturetriangle& rRight) ;
};

#endif
