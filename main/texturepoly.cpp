#include "texturepoly.hpp"

int texturetriangle::draw(float *zlinebuf,uint16_t *buff){
  int sx,ex;
  float sz,ez;
  float zv;
  float deltaz;
  fvector2 suv,euv;
  fvector2 uv;
  fvector2 deltauv;
  fvector2 cuv;
  float sw,ew,wv,deltaw;
  //    std::cout <<"x[0]:"<<pdx[0]/65536<<",x[1]:"<<pdx[1]/65536<<std::endl;

  if(pdx[0] >= pdx[1]){
    sx = (pdx[1]-0.01f);
    ex = (pdx[0]+0.01f);
    sz = pdz[1];
    ez = pdz[0];
    sw = pdw[1];
    ew = pdw[0];
    suv = pdt[1];
    euv = pdt[0];
  }else{
    // volatile int a = 0/0;
    sx = (pdx[0]-0.01f);
    ex = (pdx[1]+0.01f);
    sz = pdz[0];
    ez = pdz[1];
    sw = pdw[0];
    ew = pdw[1];
    suv = pdt[0];
    euv = pdt[1];
  }
  // printf("%.2f,%.2f",sz/65536.,ez/65536.);
  
  /*sx,ez must be 0-65535.*/
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
  zv = sz - deltaz*0.5f;
  wv = sw - deltaw*0.5f;
  uv = suv - deltauv*0.5f;
    
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
  //      volatile int x = 0/0;

  {
    int shift = 8;
    int mask = (1<<shift)-1;
    int c=sx+yno+ymin+frame;
    int cr,cg,cb;
    float bri;
    for(int i=sx;i<ex;i++){
      zv += deltaz;
      wv += deltaw;
      uv += deltauv;
      if(zv < zlinebuf[i]){
       if(zv > 0){
	 cuv = uv *(1.f/wv);
	 // cuv.print();
	 // printf("%f\n",wv);
	 uint16_t dtx;
	 dtx = tx[65535-((int(cuv.x)&mask)+((int(cuv.y)&mask)<<shift))];
	 cr = (dtx) >> 11;
	 cg = ((dtx) >> 5)&0x3F;
	 cb = (dtx) &0x1F;
	 smoke = min(1.f-zv,0.2f)*5.f;
	 bri = smoke*col;
	 cr=(cr*bri);
	 cg=(cg*bri);
	 cb=(cb*bri);
	 ((uint8_t*)buff)[i*2] = (cr|(cg<<5)|(cb<<11))>>8;
	 ((uint8_t*)buff)[i*2+1] = (cr|(cg<<5)|(cb<<11));
	 zlinebuf[i] = zv;
       }
      }
      c++;
    }
  }
 distnext:
  if(yno+ymin >= p[1].y){
    phase=1;
  }
  yno++;

  for(int i=0;i<2;i++){
    pdx[i]+=delta[phase][i];
    pdz[i]+=zdelta[phase][i];
    pdw[i]+=wdelta[phase][i];
    pdt[i]+=uvdelta[phase][i];
  }

  if(yno+ymin > ymax)return 1;
  //to be continued.
  return 0;
}

int texturetriangle::triangle_set(fvector4 px[3],const float col,const texture_t *tex,const fvector2 puv[3]){
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

  // int delta_zx;
  // int delta_zy;
  fvector4 t;
  vector3 cr;
  fvector2 t2;
  fvector2 uv[3];

  tx = tex->tx;
  txsize = tex->size;
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

  this->col = col;
  ymin = cymin();
  ymax = cymax();
  if(ymax>=window_height)ymax=window_height-1;

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

  if(ymin>=window_height-1) return -1; 
  if(ymax <= 0) return -1;
  if (ymin < 0) {
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
  if(ymin < 0)ymin=0;
  //z_val = p[2].z*256;

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
    // printf("\n\n");
  }else{
    split_x = p[2].x;
  }

  // printf("sprit%.2f",split_x/65536.);
    
  if(p[1].x >= split_x){
    pdx[0] = top_mid_x;
    pdx[1] = top_btm_x;
    pdz[0] = top_mid_z;
    pdz[1] = top_btm_z;
    pdw[0] = top_mid_w;
    pdw[1] = top_btm_w;
    pdt[0] = top_mid_uv;
    pdt[1] = top_btm_uv;
    delta[1][0]=delta_top_mid;
    delta[1][1]=delta_top_btm;
    delta[0][0]=delta_mid_btm;
    delta[0][1]=delta_top_btm;
    zdelta[1][0]=zdelta_top_mid;
    zdelta[1][1]=zdelta_top_btm;
    zdelta[0][0]=zdelta_mid_btm;
    zdelta[0][1]=zdelta_top_btm;
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
    pdz[0] = top_btm_z;
    pdz[1] = top_mid_z;
    pdw[0] = top_btm_w;
    pdw[1] = top_mid_w;
    pdt[0] = top_btm_uv;
    pdt[1] = top_mid_uv;
    delta[1][0]=delta_top_btm;
    delta[1][1]=delta_top_mid;
    delta[0][0]=delta_top_btm;
    delta[0][1]=delta_mid_btm;
    zdelta[1][0]=zdelta_top_btm;
    zdelta[1][1]=zdelta_top_mid;
    zdelta[0][0]=zdelta_top_btm;
    zdelta[0][1]=zdelta_mid_btm;
    wdelta[1][0]=wdelta_top_btm;
    wdelta[1][1]=wdelta_top_mid;
    wdelta[0][0]=wdelta_top_btm;
    wdelta[0][1]=wdelta_mid_btm;
    uvdelta[1][0]=uvdelta_top_btm;
    uvdelta[1][1]=uvdelta_top_mid;
    uvdelta[0][0]=uvdelta_top_btm;
    uvdelta[0][1]=uvdelta_mid_btm;
  }
  // printf("start @%f @%f\n",pdx[0]/65536.,pdx[1]/65536.);
  // printf("delta %f %f %f %f\n",delta[0][0]/65536.,delta[0][1]/65536.,delta[1][0]/65536.,delta[1][1]/6
  
  return 0;
}

// int texturetriangle::triangle_set(fvector3 px[3],color_t *itx,vector2 iuv[3]){
//   int delta_top_mid;
//   int delta_top_btm;
//   int delta_mid_btm;
//   int top_mid_x;
//   int top_btm_x;
//   int top_mid_z;
//   int top_btm_z;
//   // int delta_zx;
//   // int delta_zy;
//   vector3 t;
//   vector3 cr;

//   yno=0;
//   phase = 0;

//   for(int i=0;i<3;i++){
//     p[i]=px[i];
//   }
//   if(p[0].y > p[1].y) {
//     t=p[0];
//     p[0]=p[1];
//     p[1]=t;
//   }
//   if(p[0].y > p[2].y){
//     t=p[0];
//     p[0]=p[2];
//     p[2]=t;
//   }
//   if(p[1].y > p[2].y){
//     t=p[1];
//     p[1]=p[2];
//     p[2]=t;
//   }

//   // std::cout << "("<<p[0].x<<","<<p[0].y<<","<<p[0].z<<")"<<std::endl;
//   // std::cout << "("<<p[1].x<<","<<p[1].y<<","<<p[1].z<<")"<<std::endl;
//   // std::cout << "("<<p[2].x<<","<<p[2].y<<","<<p[2].z<<")"<<std::endl;

//   // {
//   //   vector3 a = p[2]-p[0];
//   //   vector3 b = p[1]-p[0];
      
//   //   printf("a=(%d,%d,%d)\n",a.x,a.y,a.z);
//   //   printf("b=(%d,%d,%d)\n",b.x,b.y,b.z);

//   //   a.x*=65536; b.x*=65536;
//   //   a.y*=65536; b.y*=65536;
//   // {
//   // 	int aabs;
//   // 	aabs = (unsigned int)0xFFFFFFFF/a.abs();
//   // 	// printf("aabs.....................%f",aabs/65536.);
//   // 	a.x = (int64_t)a.x * aabs/65536;
//   // 	a.y = (int64_t)a.y * aabs/65536;
//   // 	a.z = (int64_t)a.z * aabs/65536;
//   // 	printf("a(%.2f,%.2f,%.2f)\n",a.x/65536.,a.y/65536.,a.z/65536.);
//   // }
//   // {
//   // 	int babs;
//   // 	babs = (unsigned int)0xFFFFFFFF/a.abs();
//   // 	b.x = (int64_t)b.x * babs/65536;
//   // 	b.y = (	int64_t)b.y * babs/65536;
//   // 	b.z = (int64_t)b.z * babs/65536;
//   // 	printf("b(%.2f,%.2f,%.2f)\n",b.x/65536.,b.y/65536.,b.z/65536.);
//   // }

//   // cr = cross(b,a);
//   //	printf("cr.z is zero(%d,%d,%d)\n",cr.x,cr.y,cr.z);
 
//   //   if(cr.z&&cr.y!=0){
//   // 	{
//   // 	  int crabs;
//   // 	  crabs = cr.abs();
//   // 	  delta_zx = int64_t(1<<8)*cr.z/cr.x;
//   // 	  delta_zy = int64_t(1<<8)*cr.z/cr.y;
//   // 	}
//   // 	dxdz = delta_zx;
//   // 	dydz = delta_zy;
//   // 	printf("cr=(%.5f,%.5f,%.5f)\n",cr.x/65536.,cr.y/65536.,cr.z/65536.);
//   // 	printf("(dz/dx,dz/dy)=(%.8f,%.8f)\n",delta_zx/256.,delta_zy/256.);
//   //   }else{
//   // 	printf("cr.z is zero(%d,%d,%d)\n",cr.x,cr.y,cr.z);
//   //   }
//   // }

//   this->col = col;
//   ymin = cymin();
//   ymax = cymax();
//   if(ymax>=window_height)ymax=window_height-1;

//   top_btm_x = p[0].x;
//   top_mid_x = p[0].x;
//   top_btm_z = p[0].z;
//   top_mid_z = p[0].z;

//   if(p[0].y==p[1].y){
//     phase=1;
//     top_mid_x = p[1].x;
//     top_mid_z = p[1].z;
//   }

//   if(ymin>=window_height-1) return -1; 
//   if(ymax <= 0) return -1;
//   if (ymin < 0) {
//     ymin = 0;
//     if (p[1].y >= 0) {
//       if(p[0].y != p[1].y){
// 	top_mid_x = (p[1].x - p[0].x) * int64_t(p[1].y) / (p[0].y - p[1].y) + p[1].x;
// 	top_mid_z = (p[1].z - p[0].z) * int64_t(p[1].y) / (p[0].y - p[1].y) + p[1].z;
//       }
//     }else{
//       if(p[1].y!=p[2].y){
// 	top_mid_x = (p[2].x - p[1].x) * int64_t(p[2].y) / (p[1].y - p[2].y) + p[2].x;
// 	top_mid_z = (p[2].z - p[1].z) * int64_t(p[2].y) / (p[1].y - p[2].y) + p[2].z;
//       }
//     }
//     if(p[0].y != p[2].y){
//       top_btm_x = (p[2].x - p[0].x ) * int64_t(p[2].y) / (p[0].y - p[2].y ) + p[2].x;
//       top_btm_z = (p[2].z - p[0].z ) * int64_t(p[2].y) / (p[0].y - p[2].y ) + p[2].z;
//     }
//   }
//   if(ymin < 0)ymin=0;
//   //z_val = p[2].z*256;
  
//   if(p[2].y!=p[1].y){
//     delta_top_mid = int64_t(65536)*(p[1].x-p[2].x)/(p[1].y-p[2].y);
//   }else{
//     delta_top_mid = 0;
//   }
  
//   if(p[1].y!=p[0].y){
//     delta_mid_btm = int64_t(65536)*(p[1].x-p[0].x)/(p[1].y-p[0].y);
//   }else{
//     delta_mid_btm = 0;
//   }

//   if(p[2].y!=p[0].y){
//     delta_top_btm = int64_t(65536)*(p[2].x-p[0].x)/(p[2].y-p[0].y);
//   }else{
//     delta_top_btm = 0;
//   }

//   int zdelta_top_mid;
//   int zdelta_top_btm;
//   int zdelta_mid_btm;
//   if(p[2].y!=p[1].y){
//     zdelta_top_mid = int64_t(256)*(p[1].z-p[2].z)/(p[1].y-p[2].y);
//   }else{
//     zdelta_top_mid = 0;
//   }

//   if(p[1].y!=p[0].y){
//     zdelta_mid_btm = int64_t(256)*(p[1].z-p[0].z)/(p[1].y-p[0].y);
//   }else{
//     zdelta_mid_btm = 0;
//   }

//   if(p[2].y!=p[0].y){
//     zdelta_top_btm = int64_t(256)*(p[2].z-p[0].z)/(p[2].y-p[0].y);
//   }else{
//     zdelta_top_btm = 0;
//   }
//   // printf("zdelta %f %f %f\n",zdelta_top_btm/65536.,zdelta_top_mid/65536.,zdelta_mid_btm/65536.);
    
//   int split_x;
//   if(p[0].y != p[2].y){
//     split_x = ((int64_t(p[0].x-p[2].x)*(p[1].y-p[0].y)*65536)/(p[0].y-p[2].y)+p[0].x*65536);
//     // printf("\n\n");
//   }else{
//     split_x = p[2].x*65536;
//   }

//   // printf("sprit%.2f",split_x/65536.);
    
//   if(p[1].x*65536 >= split_x){
//     pdx[0] = top_mid_x*65536;
//     pdx[1] = top_btm_x*65536;
//     pdz[0] = top_mid_z*256;
//     pdz[1] = top_btm_z*256;
//     delta[1][0]=delta_top_mid;
//     delta[1][1]=delta_top_btm;
//     delta[0][0]=delta_mid_btm;
//     delta[0][1]=delta_top_btm;
//     zdelta[1][0]=zdelta_top_mid;
//     zdelta[1][1]=zdelta_top_btm;
//     zdelta[0][0]=zdelta_mid_btm;
//     zdelta[0][1]=zdelta_top_btm;
//   }else{
//     pdx[0] = top_btm_x*65536;
//     pdx[1] = top_mid_x*65536;
//     pdz[0] = top_btm_z*256;
//     pdz[1] = top_mid_z*256;
//     delta[1][0]=delta_top_btm;
//     delta[1][1]=delta_top_mid;
//     delta[0][0]=delta_top_btm;
//     delta[0][1]=delta_mid_btm;
//     zdelta[1][0]=zdelta_top_btm;
//     zdelta[1][1]=zdelta_top_mid;
//     zdelta[0][0]=zdelta_top_btm;
//     zdelta[0][1]=zdelta_mid_btm;
//   }
//   // printf("start @%f @%f\n",pdx[0]/65536.,pdx[1]/65536.);
//   // printf("delta %f %f %f %f\n",delta[0][0]/65536.,delta[0][1]/65536.,delta[1][0]/65536.,delta[1][1]/6
//   return 0;
// }
  

// int draw(int *zlinebuf,graphiclib &g){
  
// }
