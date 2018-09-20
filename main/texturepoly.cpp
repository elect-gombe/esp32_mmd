/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/
#include "texturepoly.hpp"
#include "3dconfig.hpp"

int texturetriangle::draw(uint16_t *zlinebuf,uint16_t *buff,int dry){
  int sx,ex,i;
  int sz,ez;
  int zv;
  int deltaz;
  fvector2 suv,euv;
  fvector2 uv;
  fvector2 deltauv;
  fvector2 cuv;
  float sw,ew,wv,deltaw;

  i=0;
  if(ymin+yno > dry){
    i = ymin-dry;
    zlinebuf += window_width*(i);
    buff += window_width*(i);
  }

  while(i<DRAW_NLINES){
    //開始地点と終了地点の各成分の算出
    // if(pdx[0] >= pdx[1]){
      sx = (pdx[1]-0.01f);
      ex = (pdx[0]+0.01f);
      sz = pdz[1];
      ez = pdz[0];
      sw = pdw[1];
      ew = pdw[0];
      suv = pdt[1];
      euv = pdt[0];
    //    } else{
    //   sx = (pdx[0]-0.01f);
    //   ex = (pdx[1]+0.01f);
    //   sz = pdz[0];
    //   ez = pdz[1];
    //   sw = pdw[0];
    //   ew = pdw[1];
    //   suv = pdt[0];
    //   euv = pdt[1];
    // }

    //各成分の増加量の計算
    float d_x;
    if(sx!=ex){
      d_x = 1.f/(ex-sx);
      deltaz = (ez-sz)*d_x;
      deltaw = (ew-sw)*d_x;
      deltauv = (euv-suv)*d_x;
    }else{
      deltaz = 0;
      deltaw = 0;
    }
    zv = sz - (deltaz>>1);
    wv = sw - deltaw*0.5f;
    uv = suv - deltauv*0.5f;

    //xのクリップ
    if(sx >= window_width)goto distnext;
    if(sx < 0){
      zv -= sx * deltaz;
      wv -= sx * deltaw;
      uv -= deltauv*sx;
      sx=0;
    }
    if(ex < 0)goto distnext;
    if(ex >= window_width){
      ex = window_width-1;
    }
    {
      //描画処理
      int shift = 8;
      int mask = 0xFF;
      int cr,cg,cb;
      float bri;
      // float smoke;
      for(int i=sx;i<ex;i++){
	//各成分の増加計算
	zv += deltaz;
	wv += deltaw;
	uv += deltauv;
	//z test
	if(zv < zlinebuf[i]){
	  zlinebuf[i] = zv;
	  //テクスチャ座標の算出
	  cuv = uv *(1.f/wv);
	  //ｚデータの書き込み
	  uint16_t dtx;
	  //テクスチャの取得
	  dtx = tx[65535-
		   (((int)(cuv.x)&mask)+(((int)(cuv.y)&mask)<<shift))];

	  // Checkered pattern
	  //	  dtx = (((int)(cuv.x/16.f)+(int)(cuv.y/16.f))&1)*65535;

	  // cr = (dtx) >> 11;
	  // cg = ((dtx) >> 5)&0x3F;
	  // cb = (dtx) &0x1F;
	  // // //遠くのに対して黒く補正する。
	  // // smoke = min(1.f-zv,0.2f)*5.f;
	  // bri = 1.f;
	  // // // //色の計算
	  // cr=(cr*bri);
	  // cg=(cg*bri);
	  // cb=(cb*bri);
	  //色の書き込み
#ifdef ENDIAN_LITTLE
	  ((uint8_t*)buff)[i*2] = dtx>>8;
	  ((uint8_t*)buff)[i*2+1] = dtx;
// 	  ((uint8_t*)buff)[i*2+1] = ((cg<<5)|(cr<<11))>>8;
// 	  ((uint8_t*)buff)[i*2] = (cb|(cg<<5));
#else
	  ((uint8_t*)buff)[i*2+1] = dtx>>8;
	  ((uint8_t*)buff)[i*2] = dtx;
// 	  ((uint8_t*)buff)[i*2] = ((cg<<5)|(cr<<11))>>8;
// 	  ((uint8_t*)buff)[i*2+1] = (cb|(cg<<5));
#endif
	  // ((uint8_t*)buff)[i*2] = 0xFF;
	  // ((uint8_t*)buff)[i*2+1] = 0xFF;
	}
      }
    }
  distnext:
    //三角型の真ん中のところに達したか？
    if(yno+ymin >= ymiddle){
      //増加量の更新
      phase=1;
    }
    yno++;

    //各成分のdyに対する増加分の計算
    for(int i=0;i<2;i++){
      pdx[i]+=delta[phase][i];
      pdz[i]+=zdelta[phase][i];
      pdw[i]+=wdelta[phase][i];
      pdt[i]+=uvdelta[phase][i];
    }

    if(yno+ymin > ymax)return 1;
    zlinebuf += window_width;
    buff += window_width;
    i++;
  }
  //to be continued.
  return 0;
}

// int texturetriangle::draw(float *zlinebuf,uint16_t *buff){
//   int sx,ex;
//   float sz,ez;
//   float zv;
//   float deltaz;
//   fvector2 suv,euv;
//   fvector2 uv;
//   fvector2 deltauv;
//   fvector2 cuv;
//   float sw,ew,wv,deltaw;

//   //開始地点と終了地点の各成分の算出
//   if(pdx[0] >= pdx[1]){
//     sx = (pdx[1]-0.01f);
//     ex = (pdx[0]+0.01f);
//     sz = pdz[1];
//     ez = pdz[0];
//     sw = pdw[1];
//     ew = pdw[0];
//     suv = pdt[1];
//     euv = pdt[0];
//   }else{
//     sx = (pdx[0]-0.01f);
//     ex = (pdx[1]+0.01f);
//     sz = pdz[0];
//     ez = pdz[1];
//     sw = pdw[0];
//     ew = pdw[1];
//     suv = pdt[0];
//     euv = pdt[1];
//   }

//   //各成分の増加量の計算
//   float d_x;
//   if(sx!=ex){
//     d_x = 1.f/(ex-sx);
//     deltaz = (ez-sz)*d_x;
//     deltaw = (ew-sw)*d_x;
//     deltauv = (euv-suv)*d_x;
//   }else{
//     deltaz = 0;
//     deltaw = 0;
//   }
//   zv = sz - deltaz*0.5f;
//   wv = sw - deltaw*0.5f;
//   uv = suv - deltauv*0.5f;

//   //xのクリップ
//   if(sx >= window_width)goto distnext;
//   if(sx < 0){
//     zv -= sx * deltaz;
//     wv -= sx * deltaw;
//     uv -= deltauv*sx;
//     sx=0;
//   }
//   if(ex < 0)goto distnext;
//   if(ex >= window_width){
//     ex = window_width-1;
//   }

//   //描画処理
//   {
//     int shift = 8;
//     int mask = 0xFF;
//     int cr,cg,cb;
//     float bri;
//     float smoke;
//     for(int i=sx;i<ex;i++){
//       //各成分の増加計算
//       zv += deltaz;
//       wv += deltaw;
//       uv += deltauv;
//       //z test
//       if(zv < zlinebuf[i]){
// 	if(zv > 0){
// 	  //テクスチャ座標の算出
// 	  cuv = uv *(1.f/wv);
// 	  //ｚデータの書き込み
// 	  zlinebuf[i] = zv;
// 	  uint16_t dtx;
// 	  //テクスチャの取得
// 	  dtx = tx[65535-
// 		   (((int)(cuv.x)&mask)+(((int)(cuv.y)&mask)<<shift))];
// 	  cr = (dtx) >> 11;
// 	  cg = ((dtx) >> 5)&0x3F;
// 	  cb = (dtx) &0x1F;
// 	  // //遠くのに対して黒く補正する。
// 	  // smoke = min(1.f-zv,0.2f)*5.f;
// 	  bri = col;
// 	  // //色の計算
// 	  cr=(cr*bri);
// 	  cg=(cg*bri);
// 	  cb=(cb*bri);
// 	  //色の書き込み
// 	  ((uint8_t*)buff)[i*2] = ((cg<<5)|(cr<<11))>>8;
// 	  ((uint8_t*)buff)[i*2+1] = (cb|(cg<<5));

// 	  // ((uint8_t*)buff)[i*2] = 0xFF;
// 	  // ((uint8_t*)buff)[i*2+1] = 0xFF;
// 	}
//       }
//     }
//  distnext:
//   //三角型の真ん中のところに達したか？
//   if(yno+ymin >= ymiddle){
//     //増加量の更新
//     phase=1;
//   }
//   yno++;

//   //各成分のdyに対する増加分の計算
//   for(int i=0;i<2;i++){
//     pdx[i]+=delta[phase][i];
//     pdz[i]+=zdelta[phase][i];
//     pdw[i]+=wdelta[phase][i];
//     pdt[i]+=uvdelta[phase][i];
//   }

//   if(yno+ymin > ymax)return 1;
//   //to be continued.
//   return 0;
// }

int texturetriangle::triangle_set(fvector4 px[3],const float col,const texture_t *tex,const fvector2 puv[3]){
  fvector4 p[3];
  float delta_top_mid;
  float delta_top_btm;
  float delta_mid_btm;
  float top_mid_x;
  float top_btm_x;
  float top_mid_z;
  float top_btm_z;
  float top_mid_w;
  float top_btm_w;
  fvector2 top_mid_uv;
  fvector2 top_btm_uv;

  fvector4 t;
  fvector2 t2;
  fvector2 uv[3];

  tx = tex->tx;
  yno=0;
  phase = 0;
  this->col = col;

  px[0].x = (int)(px[0].x+0.5f);
  px[0].y = (int)(px[0].y+0.5f);
  px[1].x = (int)(px[1].x+0.5f);
  px[1].y = (int)(px[1].y+0.5f);
  px[2].x = (int)(px[2].x+0.5f);
  px[2].y = (int)(px[2].y+0.5f);

  for(int i=0;i<3;i++){
    p[i]=px[i];
    p[i].w = 1.f/p[i].w;
    uv[i]=puv[i]*(p[i].w);
  }
  if(p[0].y > p[1].y) {
    t=p[0];
    t2=uv[0];
    p[0]=p[1];
    uv[0]=uv[1];
    p[1]=t;
    uv[1]=t2;
  }
  if(p[0].y > p[2].y){
    t=p[0];
    t2=uv[0];
    p[0]=p[2];
    uv[0]=uv[2];
    p[2]=t;
    uv[2]=t2;
  }
  if(p[1].y > p[2].y){
    t=p[1];
    t2=uv[1];
    p[1]=p[2];
    uv[1]=uv[2];
    p[2]=t;
    uv[2]=t2;
  }

  ymin = p[0].y;
  if(p[2].y>=window_height)ymax=window_height-1;
  else ymax = p[2].y;

  top_btm_x = p[0].x;
  top_mid_x = p[0].x;
  top_btm_z = p[0].z;
  top_mid_z = p[0].z;
  top_btm_w = p[0].w;
  top_mid_w = p[0].w;
  top_mid_uv = uv[0];
  top_btm_uv = uv[0];

  if(p[0].y==p[1].y){
    phase=1;
    top_mid_x = p[1].x;
    top_mid_z = p[1].z;
    top_mid_w = p[1].w;
    top_mid_uv = uv[1];
    top_btm_uv = uv[0];
  }

  if(p[0].y>=window_height-1) return -1; 
  if(p[2].y <= 0) return -1;
  if (p[0].y < 0) {
    ymin = 0;
    if (p[1].y >= 0) {
      if(p[0].y != p[1].y){
	float d_y;
	d_y = p[1].y / (p[0].y - p[1].y);
	top_mid_x = (p[1].x - p[0].x) * d_y + p[1].x;
	top_mid_z = (p[1].z - p[0].z) * d_y + p[1].z;
	top_mid_w = (p[1].w - p[0].w) * d_y + p[1].w;
	top_mid_uv = (uv[1]-uv[0]) * d_y + uv[1];
      }
    }else{
      if(p[1].y!=p[2].y){
	float d_y;
	d_y = p[2].y / (p[1].y - p[2].y);
	top_mid_x = (p[2].x - p[1].x) * d_y + p[2].x;
	top_mid_z = (p[2].z - p[1].z) * d_y + p[2].z;
	top_mid_w = (p[2].w - p[1].w) * d_y + p[2].w;
	top_mid_uv = (uv[2]-uv[1]) * d_y + uv[2];
      }
    }
    if(p[0].y != p[2].y){
      float d_y;
      d_y = p[2].y / (p[0].y - p[2].y );
      top_btm_x = (p[2].x - p[0].x ) * d_y + p[2].x;
      top_btm_z = (p[2].z - p[0].z ) * d_y + p[2].z;
      top_btm_w = (p[2].w - p[0].w ) * d_y + p[2].w;
      top_btm_uv = (uv[2]-uv[0]) * d_y + uv[2];
    }
  }

  float zdelta_top_mid,wdelta_top_mid;
  float zdelta_top_btm,wdelta_top_btm;
  float zdelta_mid_btm,wdelta_mid_btm;
  fvector2 uvdelta_top_mid;
  fvector2 uvdelta_top_btm;
  fvector2 uvdelta_mid_btm;

  if(p[2].y!=p[1].y){
    float iy;
    iy = 1.f/(p[1].y-p[2].y);
    delta_top_mid = (p[1].x-p[2].x)*iy;
    zdelta_top_mid = (p[1].z-p[2].z)*iy;
    wdelta_top_mid = (p[1].w-p[2].w)*iy;
    uvdelta_top_mid = (uv[1]-uv[2])*(1.f/(p[1].y-p[2].y));
  }else{
    delta_top_mid = 0;
    zdelta_top_mid = 0;
    wdelta_top_mid = 0;
  }
  
  if(p[1].y!=p[0].y){
    float iy;
    iy = 1.f/(p[1].y-p[0].y);
    delta_mid_btm = (p[1].x-p[0].x)*iy;
    zdelta_mid_btm = (p[1].z-p[0].z)*iy;
    wdelta_mid_btm = (p[1].w-p[0].w)*iy;
    uvdelta_mid_btm = (uv[1]-uv[0])*(1.f/(p[1].y-p[0].y));
  }else{
    delta_mid_btm = 0;
    zdelta_mid_btm = 0;
    wdelta_mid_btm = 0;
  }

  if(p[2].y!=p[0].y){
    float iy;
    iy = 1.f/(p[2].y-p[0].y);
    delta_top_btm = (p[2].x-p[0].x)*iy;
    zdelta_top_btm = (p[2].z-p[0].z)*iy;
    wdelta_top_btm = (p[2].w-p[0].w)*iy;
    uvdelta_top_btm = (uv[2]-uv[0])*(1.f/(p[2].y-p[0].y));
  }else{
    delta_top_btm = 0;
    zdelta_top_btm = 0;
    wdelta_top_btm = 0;
  }
    
  float split_x;
  if(p[0].y != p[2].y){
    split_x = (((p[0].x-p[2].x)*(p[1].y-p[0].y))/(p[0].y-p[2].y)+p[0].x);
  }else{
    split_x = p[2].x;
  }
    
  if(p[1].x >= split_x){
    pdx[0] = top_mid_x;
    pdx[1] = top_btm_x;
    pdz[0] = top_mid_z*65536;
    pdz[1] = top_btm_z*65536;
    pdw[0] = top_mid_w;
    pdw[1] = top_btm_w;
    pdt[0] = top_mid_uv;
    pdt[1] = top_btm_uv;
    delta[1][0]=delta_top_mid;
    delta[1][1]=delta_top_btm;
    delta[0][0]=delta_mid_btm;
    delta[0][1]=delta_top_btm;
    zdelta[1][0]=zdelta_top_mid*65536;
    zdelta[1][1]=zdelta_top_btm*65536;
    zdelta[0][0]=zdelta_mid_btm*65536;
    zdelta[0][1]=zdelta_top_btm*65536;
    wdelta[1][0]=wdelta_top_mid;
    wdelta[1][1]=wdelta_top_btm;
    wdelta[0][0]=wdelta_mid_btm;
    wdelta[0][1]=wdelta_top_btm;
    uvdelta[1][0]=uvdelta_top_mid;
    uvdelta[1][1]=uvdelta_top_btm;
    uvdelta[0][0]=uvdelta_mid_btm;
    uvdelta[0][1]=uvdelta_top_btm;
  }else{
    pdx[0] = top_btm_x;
    pdx[1] = top_mid_x;
    pdz[0] = top_btm_z*65536;
    pdz[1] = top_mid_z*65536;
    pdw[0] = top_btm_w;
    pdw[1] = top_mid_w;
    pdt[0] = top_btm_uv;
    pdt[1] = top_mid_uv;
    delta[1][0]=delta_top_btm;
    delta[1][1]=delta_top_mid;
    delta[0][0]=delta_top_btm;
    delta[0][1]=delta_mid_btm;
    zdelta[1][0]=zdelta_top_btm*65536;
    zdelta[1][1]=zdelta_top_mid*65536;
    zdelta[0][0]=zdelta_top_btm*65536;
    zdelta[0][1]=zdelta_mid_btm*65536;
    wdelta[1][0]=wdelta_top_btm;
    wdelta[1][1]=wdelta_top_mid;
    wdelta[0][0]=wdelta_top_btm;
    wdelta[0][1]=wdelta_mid_btm;
    uvdelta[1][0]=uvdelta_top_btm;
    uvdelta[1][1]=uvdelta_top_mid;
    uvdelta[0][0]=uvdelta_top_btm;
    uvdelta[0][1]=uvdelta_mid_btm;
  }
  ymiddle = p[1].y;
  isexisting = 1;
  
  return 0;
}
