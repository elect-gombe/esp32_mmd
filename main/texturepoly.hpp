/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/
#ifndef __TEXTURE_POLY_H
#define __TEXTURE_POLY_H

#include "triangle.hpp"
#include "fvector2.hpp"
#include "fvector4.hpp"

#define IRAM_ATTR __attribute__((section(".iram1")))

//テクスチャ型
struct texture_t{
  const uint16_t *tx;
  // vector2 size;//未実装
};


//テクスチャのついた三角形描画はtriangleを継承しますが、ほとんどオーバーライドしています。速度の問題とかあるんで
class texturetriangle:public triangle{
private:
  //テクスチャデータ
  const uint16_t *tx;
  //テクスチャ内座標
  fvector2 pdt[2];
  fvector2 uvdelta[2][2];
  //w値の補間用
  float pdw[2];
  float wdelta[2][2];
public:
  texturetriangle(){}
  int triangle_set(fvector4 px[3],float c,const texture_t *t,const fvector2 puv[3]);
  int draw(uint16_t *zlinebuf,uint16_t *buff,int dry);
  static bool LessZ(const texturetriangle& rLeft, const texturetriangle& rRight) ;
};

#endif
