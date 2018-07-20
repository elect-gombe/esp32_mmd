#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "fvector4.hpp"
#include "fvector2.hpp"
#include "matrix4.hpp"
#include "graphiclib.hpp"
#include "triangle.hpp"
#include "texturepoly.hpp"
#include "lcd.h"
#include <algorithm>
#include "3dconfig.hpp"

#define ZSORT 0

extern "C"{
  void toggleLED(void);
  void send_first(int ypos) ;
  void send_line(int ypos, uint16_t *line) ;
  void send_aline_finish();
  int main3d();
  void esp_task_wdt_feed();
}

#include "poly.h"

#define POLYNUM int(sizeof(polyvec)/sizeof(polyvec[0]))
#define POINTNUM int(sizeof(pointvec)/sizeof(pointvec[0]))
#define WIRENUM int(sizeof(wireframe)/sizeof(wireframe[0]))

#define MAXPROC_POLYNUM (POLYNUM*3/5)

vector3 pv[12][3];


//-----------------------------------------------------------------------------
// Read CCOUNT register.
//-----------------------------------------------------------------------------
static __inline__ uint32_t xos_get_ccount(void)
{
#if XCHAL_HAVE_CCOUNT

#if defined (__XCC__)
  return XT_RSR_CCOUNT();
#else
  uint32_t ccount;

  __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (ccount) );
  return ccount;
#endif

#else

  return 0;

#endif
}

float loadPower(const fvector3 &light_pos,const fvector3 &light_n,const fvector4 obj[3]){
  fvector3 light_obj;
  fvector3 n;
  fvector4 obj_pos;
  float cos_cross;

  obj_pos = obj[0]+obj[1]+obj[2];
  obj_pos.x = obj_pos.x / 3;
  obj_pos.y = obj_pos.y / 3;
  obj_pos.z = obj_pos.z / 3;
  
  n = calc_nv(obj);
  cos_cross = light_n * n;

  return cos_cross;// * light_obj_dist/65536;
}

const int SIZE_TEX = 256;

fvector4 obj_transed[POINTNUM];
fvector4 poly_transed[POINTNUM];


#if ZSORT
//ソート用のデータの作成
struct draworder_t{
  int draworder;
  int zdata;
};
draworder_t draworder[MAXPROC_POLYNUM];
#endif

uint16_t *drawbuff[2];

float *zlinebuf;


int main3d(void){
  Matrix4 m;
  Matrix4 projection;
  Matrix4 obj;
  int lastbuff = 0;

  drawbuff[0] = new uint16_t[window_width*DRAW_NLINES];
  drawbuff[1] = new uint16_t[window_width*DRAW_NLINES];
  zlinebuf = new float[window_width*DRAW_NLINES];
  
  texturetriangle *t[MAXPROC_POLYNUM];

  for(int i=0;i<MAXPROC_POLYNUM;i++){
    printf("%d\n",i);
    fflush(stdout);
    t[i] = new texturetriangle;
  }

  //透視投影行列
  projection=loadPerspective(0.25f,float(window_height)/window_width,0.0001f,3.f,0,0)*projection;

  fvector3 viewdir;
  fvector3 veye;
  float dist = 3.f;
  vector2 mouse;
  vector2 pmouse;
  fvector2 np = fvector2(440.f,0);
  vector2 pnp;
  bool clicking = false;
  float average = 0.f;
    
  fvector4 vo[3];
  fvector4 v[3];
  fvector3 n;
  int tnum=0;

  int hogec=0;
  uint32_t prev = 0;  

  veye = fvector3(0,0,-15.5f);
  obj = obj*magnify(0.995);
  while(1){
    {
      esp_task_wdt_feed();      // if(hogec!=0)
      // 	send_aline_finish();
      if(1){
	if(prev){
	  if(0&&average){
	    average = average *0.9+0.1*240000000.f/(xos_get_ccount()-prev);
	  }else{
	    average = 240000000.f/(xos_get_ccount()-prev);
	  }
	  printf("%.2ffps tri:%d\n",average,tnum);
	}
	prev = xos_get_ccount();
      }
      tnum=0;
      hogec++;
    }
    np.x+=2.f;    //camera rotation
    //視点計算
#if MODEL == 1
    dist = 2.0f;// + 1.4f*cosf(np.x/150.f*3.14159265358979324f);
#else
    dist = 3.5f + 1.5f*cosf(np.x/150.f*3.14159265358979324f);
#endif
    veye = -fvector3(cosf(np.x/300.f*3.14159265f)*cosf(np.y/300.f*3.14159265f),sinf(np.y/300.f*3.14159265f),sinf(np.x/300.f*3.14159265f)*cosf(np.y/300.f*3.14159265f));
    //透視投影行列とカメラ行列の合成
#if MODEL <= 3
    m=projection*lookat(fvector3(0,0,0),veye*dist)*obj*translation(fvector3(0,-0.3,0));
#else
    m=projection*lookat(fvector3(0,0,0),veye*dist)*obj*translation(fvector3(0,0.3,-0.7));
#endif
    //頂点データを変換
    for(int j=0;j<POINTNUM;j++){
      obj_transed[j] = fvector4(pointvec[j].x,pointvec[j].y,pointvec[j].z);
      poly_transed[j] = m.applyit_v4(fvector3(pointvec[j]));
      //std::cout<<"poly"<<poly_transed[j].x<<","<<poly_transed[j].y<<","<<poly_transed[j].z<<std::endl;
    }
    //ポリゴンデータの生成
    for(int i=0;i<POLYNUM;i++){
      for(int j=0;j<3;j++){
	v[j] = poly_transed[polyvec[i][j]];
	vo[j] = obj_transed[polyvec[i][j]];
      }

      //簡易1次クリッピング
      if(v[0].w < 0)continue;
      if(v[2].w < 0)continue;
      if(v[1].w < 0)continue;
      if(!(
	   !((abs(v[0].x) > 1.f)||(abs(v[0].y) > 1.f)||(abs(v[0].z) < 0.f))||
	   !((abs(v[1].x) > 1.f)||(abs(v[1].y) > 1.f)||(abs(v[1].z) < 0.f))||
	   !((abs(v[2].x) > 1.f)||(abs(v[2].y) > 1.f)||(abs(v[2].z) < 0.f))))
	continue;
      //クリップ座標系からディスプレイ座標系の変換
      v[0].x=v[0].x*window_width+window_width/2;v[0].y=v[0].y*window_height+window_height/2;
      v[1].x=v[1].x*window_width+window_width/2;v[1].y=v[1].y*window_height+window_height/2;
      v[2].x=v[2].x*window_width+window_width/2;v[2].y=v[2].y*window_height+window_height/2;

      //テクスチャのデータ
      const texture_t tex={
	stonetex,vector2(4,4)
      };
      fvector2 puv[3];
      for(int j=0;j<3;j++){
	puv[j].x = (1.f-point_uv[polyvec[i][j]].x)*SIZE_TEX;
	puv[j].y = (point_uv[polyvec[i][j]].y)*SIZE_TEX;
      }
      //光量の計算
      float light = loadPower(fvector3(),veye,vo);
      if(light>0){
	if(t[tnum++]->triangle_set(v,light,&tex,puv)==-1)tnum--;
      }
    }
    
#if ZSORT
    for(int i=0;i<tnum;i++){
      draworder[i].draworder = i;
      draworder[i].zdata = t[i]->pdz[0]*65536.;
    }
    std::sort(draworder,draworder+tnum, [](draworder_t& x, draworder_t& y){return x.zdata < y.zdata;});
    
#endif
    //ラインごとに描画しLCDに転送
    for(int y=0;y<window_height/DRAW_NLINES;y++){
      for(int i=0;i<window_width*DRAW_NLINES;i++){
	zlinebuf[i]=1.f;
	drawbuff[lastbuff][i]=0x0020;/*RGB*/
      }
      for(int i=0;i<tnum;i++){
#if ZSORT
	if(t[draworder[i].draworder]->ymin < y&&t[draworder[i].draworder]->ymax >= y){
	  t[draworder[i].draworder]->draw(zlinebuf,drawbuff[lastbuff],y*DRAW_NLINES);
	}
#else

	if(t[i]->ymin < (y*DRAW_NLINES)+DRAW_NLINES&&t[i]->ymax >= y*DRAW_NLINES){
  	  t[i]->draw(zlinebuf,drawbuff[lastbuff],y*DRAW_NLINES);
	}
#endif
      }
      send_line(y*DRAW_NLINES,drawbuff[lastbuff]);
      lastbuff = 1-lastbuff;
    }
  }
  for(int i=0;i<MAXPROC_POLYNUM;i++){
    delete[] t[i];
  }
}
