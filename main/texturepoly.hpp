#ifndef __TEXTURE_POLY_H
#define __TEXTURE_POLY_H

#include "graphiclib.hpp"
#include "triangle.hpp"
#include "vector3.hpp"
#include "fvector2.hpp"
#include "fvector4.hpp"
typedef struct{
  const uint16_t *tx;
  vector2 size;
} texture_t;

class texturetriangle:public triangle{
private:
  const uint16_t *tx;
  vector2 txsize;
  fvector2 pdt[2];
  fvector2 uvdelta[2][2];
  float pdw[2];
  float wdelta[2][2];
public:
  int triangle_set(fvector4 px[3],float c,const texture_t *t,const fvector2 puv[3]);
  int draw(float *zlinebuf,uint16_t *buff);
};


#endif
