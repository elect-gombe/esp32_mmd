#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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

#define MAXPROC_POLYNUM (800)

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


volatile enum vtaskstate{
  VTASK_WAIT,
  VTASK_VERTEX_CALCULATION,
  VTASK_POLY_GENERATE,
  VTASK_POLY_DRAW,
} vtaskstate;

fvector4 obj_transed[POINTNUM];
fvector4 poly_transed[POINTNUM];
Matrix4 m;

extern "C"{
  void vTask(void*);
};

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


uint8_t drawidx[POLYNUM];
static
texturetriangle *t[MAXPROC_POLYNUM];

int draw_y;
fvector3 veye;
int lastbuff = 0;

void vTask(void* prm){
  fvector4 vo[3];
  fvector4 v[3];
  fvector3 n;

  while(1){
    switch(vtaskstate){
    case VTASK_WAIT:
      break;
    case VTASK_VERTEX_CALCULATION:
      for(int j=POINTNUM/2;j<POINTNUM;j++){
	obj_transed[j] = fvector4(pointvec[j].x,pointvec[j].y,pointvec[j].z);
	poly_transed[j] = m.applyit_v4(fvector3(pointvec[j]));
	poly_transed[j].x=poly_transed[j].x*window_width+window_width/2;poly_transed[j].y=poly_transed[j].y*window_height+window_height/2;	
      }
      vtaskstate = VTASK_WAIT;
      break;
    case VTASK_POLY_GENERATE:
      {
	int searchidx = MAXPROC_POLYNUM/2;
	for(int i=1;i<POLYNUM;i+=2){
	  if(drawidx[i])continue;
	  for(int j=0;j<3;j++){
	    v[j] = poly_transed[polyvec[i][j]];
	    vo[j] = obj_transed[polyvec[i][j]];
	  }
	  if((draw_y*DRAW_NLINES+DRAW_NLINES < v[0].y&&draw_y*DRAW_NLINES+DRAW_NLINES < v[1].y&&draw_y*DRAW_NLINES+DRAW_NLINES < v[2].y))continue;
	  drawidx[i] = 1;
	  if((0 > v[0].x&&0>v[1].x&&0 > v[2].x)||(window_width <= v[0].x&&window_width<=v[1].x&&window_width <= v[2].x))continue;
	  //簡易1次クリッピング
	  if(v[0].w < 0)continue;
	  if(v[2].w < 0)continue;
	  if(v[1].w < 0)continue;
	  //クリップ座標系からディスプレイ座標系の変換
	  // v[0].x=v[0].x*window_width+window_width/2;v[0].y=v[0].y*window_height+window_height/2;
	  // v[1].x=v[1].x*window_width+window_width/2;v[1].y=v[1].y*window_height+window_height/2;
	  // v[2].x=v[2].x*window_width+window_width/2;v[2].y=v[2].y*window_height+window_height/2;

	  //光量の計算
	  float light = loadPower(fvector3(),veye,vo);
	  if(light>0){
	    //テクスチャのデータ
	    const texture_t tex={
	      stonetex,vector2(4,4)
	    };
	    fvector2 puv[3];
	    for(int j=0;j<3;j++){
	      puv[j].x = (1.f-point_uv[polyvec[i][j]].x)*SIZE_TEX;
	      puv[j].y = (point_uv[polyvec[i][j]].y)*SIZE_TEX;
	    }
	    while(searchidx < MAXPROC_POLYNUM&&t[searchidx]->isexisting)searchidx++;
	    if(searchidx == MAXPROC_POLYNUM){
	      printf("over flowed\n");
	      continue;
	    }
	    if(t[searchidx]->triangle_set(v,light,&tex,puv)==-1)t[searchidx]->isexisting = 0;
	  }
	}
	vtaskstate = VTASK_WAIT;      
	break;
      }

    case VTASK_POLY_DRAW:
    // case VTASK_DRAW:
      for(int i=1;i<MAXPROC_POLYNUM;i+=2){
	// #if ZSORT
	// if(t[draworder[i].draworder]->ymin < &&t[draworder[i].draworder]->ymax >= y){
	//   t[draworder[i].draworder]->draw(zlinebuf,drawbuff[lastbuff],y*DRAW_NLINES);
	// }
	// #else

	if(t[i]->isexisting// ymin < (y*DRAW_NLINES)+DRAW_NLINES&&t[i]->ymax >= y*DRAW_NLINES
	   ){
  	  if(t[i]->draw(zlinebuf,drawbuff[lastbuff],draw_y*DRAW_NLINES)){
	    t[i]->isexisting = 0;
	  }
	}
	// #endif
      }
      vtaskstate = VTASK_WAIT;      
      break;
    }
  }
}

int main3d(void){
  Matrix4 projection;
  Matrix4 obj;
  

  drawbuff[0] = new uint16_t[window_width*DRAW_NLINES];
  drawbuff[1] = new uint16_t[window_width*DRAW_NLINES];
  zlinebuf = new float[window_width*DRAW_NLINES];
  

  for(int i=0;i<MAXPROC_POLYNUM;i++){
    printf("%d\n",i);
    fflush(stdout);
    t[i] = new texturetriangle;
    t[i]->isexisting = 0;
  }

  //透視投影行列
  projection=loadPerspective(0.25f,float(window_height)/window_width,0.0001f,3.f,0,0)*projection;

  fvector3 viewdir;
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
      // esp_task_wdt_feed();      // if(hogec!=0)
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
    vtaskstate = VTASK_VERTEX_CALCULATION;
    for(int j=0;j<POINTNUM/2;j++){
      obj_transed[j] = fvector4(pointvec[j].x,pointvec[j].y,pointvec[j].z);
      poly_transed[j] = m.applyit_v4(fvector3(pointvec[j]));
      poly_transed[j].x=poly_transed[j].x*window_width+window_width/2;poly_transed[j].y=poly_transed[j].y*window_height+window_height/2;
      //std::cout<<"poly"<<poly_transed[j].x<<","<<poly_transed[j].y<<","<<poly_transed[j].z<<std::endl;
    }
    while(vtaskstate == VTASK_VERTEX_CALCULATION);
    //ポリゴンデータの生成
    
// #if ZSORT
    // for(int i=0;i<tnum;i++){
    //   draworder[i].draworder = i;
    //   draworder[i].zdata = t[i]->pdz[0]*65536.;
    // }
    // std::sort(draworder,draworder+tnum, [](draworder_t& x, draworder_t& y){return x.zdata < y.zdata    ;});
    
// #endif
    for(int i=0;i<MAXPROC_POLYNUM;i++){
      t[i]->isexisting = 0;
    }
    for(int i=0;i<POLYNUM;i++){
      drawidx[i] = 0;
    }      
    //ラインごとに描画しLCDに転送
    for(draw_y=0;draw_y<window_height/DRAW_NLINES;draw_y++){
      int searchidx = 0;
      for(int i=0;i<window_width*DRAW_NLINES;i++){
	zlinebuf[i]=1.f;
	drawbuff[lastbuff][i]=0x0020;/*RGB*/
      }

      vtaskstate = VTASK_POLY_GENERATE;
      for(int i=0;i<POLYNUM;i+=2){
	if(drawidx[i])continue;
	for(int j=0;j<3;j++){
	  v[j] = poly_transed[polyvec[i][j]];
	  vo[j] = obj_transed[polyvec[i][j]];
	}
	if((draw_y*DRAW_NLINES+DRAW_NLINES < v[0].y&&draw_y*DRAW_NLINES+DRAW_NLINES < v[1].y&&draw_y*DRAW_NLINES+DRAW_NLINES < v[2].y))continue;
	drawidx[i] = 1;
	if((0 > v[0].x&&0>v[1].x&&0 > v[2].x)||(window_width <= v[0].x&&window_width<=v[1].x&&window_width <= v[2].x))continue;
	//簡易1次クリッピング
	if(v[0].w < 0)continue;
	if(v[2].w < 0)continue;
	if(v[1].w < 0)continue;
	//クリップ座標系からディスプレイ座標系の変換
	// v[0].x=v[0].x*window_width+window_width/2;v[0].y=v[0].y*window_height+window_height/2;
	// v[1].x=v[1].x*window_width+window_width/2;v[1].y=v[1].y*window_height+window_height/2;
	// v[2].x=v[2].x*window_width+window_width/2;v[2].y=v[2].y*window_height+window_height/2;

	//光量の計算
	float light = loadPower(fvector3(),veye,vo);
	if(light>0){
	  //テクスチャのデータ
	  const texture_t tex={
	    stonetex,vector2(4,4)
	  };
	  fvector2 puv[3];
	  for(int j=0;j<3;j++){
	    puv[j].x = (1.f-point_uv[polyvec[i][j]].x)*SIZE_TEX;
	    puv[j].y = (point_uv[polyvec[i][j]].y)*SIZE_TEX;
	  }
	  while(searchidx < MAXPROC_POLYNUM&&t[searchidx]->isexisting)searchidx++;
	  if(searchidx == MAXPROC_POLYNUM){
	    printf("over flowed\n");
	    continue;
	  }
	  if(t[searchidx]->triangle_set(v,light,&tex,puv)==-1)t[searchidx]->isexisting = 0;
	}
      }
      while(vtaskstate == VTASK_POLY_GENERATE);
      vtaskstate = VTASK_POLY_DRAW;
      for(int i=0;i<MAXPROC_POLYNUM;i+=2){
// #if ZSORT
	// if(t[draworder[i].draworder]->ymin < &&t[draworder[i].draworder]->ymax >= y){
	//   t[draworder[i].draworder]->draw(zlinebuf,drawbuff[lastbuff],y*DRAW_NLINES);
	// }
// #else

	if(t[i]->isexisting// ymin < (y*DRAW_NLINES)+DRAW_NLINES&&t[i]->ymax >= y*DRAW_NLINES
	   ){
  	  if(t[i]->draw(zlinebuf,drawbuff[lastbuff],draw_y*DRAW_NLINES)){
	    t[i]->isexisting = 0;
	  }
	}
// #endif
      }
      while(vtaskstate == VTASK_POLY_DRAW);
      send_line(draw_y*DRAW_NLINES,drawbuff[lastbuff]);
      lastbuff = 1-lastbuff;
    }
  }
  for(int i=0;i<MAXPROC_POLYNUM;i++){
    delete[] t[i];
  }
}
