/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include "fvector4.hpp"
#include "matrix4.hpp"
#include "quaternion.hpp"
#include "texturepoly.hpp"
#include "3dconfig.hpp"
#include <unistd.h>
#include "skeletal-animation.hpp"

#ifdef USE_SDL
#include <SDL2/SDL.h>
#endif

#include "poly.h"

#define POLYNUM int(sizeof(polyvec)/sizeof(polyvec[0]))
#define POINTNUM int(sizeof(pointvec)/sizeof(pointvec[0]))

extern "C"{
  int main3d();
  void *vTask(void*);
  int kbhit(void);
  void send_line(int ypos, uint8_t *line) ;
}

enum vtaskstate{
  VTASK_WAIT,
  VTASK_VERTEX_CALCULATION,
  VTASK_POLY_GENERATE,
  VTASK_POLY_DRAW,
};

struct{
  volatile enum vtaskstate vtaskstate;
#ifndef OMIT_ZBUFFER_CONFLICT
  uint16_t *drawbuff;
  unsigned short *zbuff;
#endif
} clientps[PROCESSNUM-1];

uint16_t *g_drawbuff[2];
uint16_t *g_zbuff;

fvector4 poly_transed[POINTNUM];
Matrix4 m;

#define ZNEAR 0.1f

float loadPower(// const fvector3 &light_pos,
		const fvector3 &light_n,const fvector4 obj[3]){
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

uint8_t drawidx[POLYNUM];

#define TRI_MULTI 4

static
texturetriangle *tri[(MAXPROC_POLYNUM+TRI_MULTI-1)/TRI_MULTI];

int draw_y;
fvector3 veye;
int lastbuff = 0;

bonelist bl;

static inline
bool unvisible(fvector4 *v){
  if((0 > v[0].x&&0>v[1].x&&0 > v[2].x)||(window_width <= v[0].x&&window_width<=v[1].x&&window_width <= v[2].x))return 1;

  return culling(v);
}

void calc_vertices(unsigned int begin,unsigned int end){
  for(unsigned int j=begin;j<end;j++){
    // obj_transed[j] = fvector4(pointvec[j].x,pointvec[j].y,pointvec[j].z);
#ifdef DISABLE_ANIMATION
    poly_transed[j] =  m.mul_fv4(fvector3(pointvec[j]));
#else
    poly_transed[j] =  bl.boneworld[bone_index[j]].mul_fv4(fvector3(pointvec[j]));
#endif
    poly_transed[j].x=poly_transed[j].x*window_width+window_width/2;poly_transed[j].y=poly_transed[j].y*window_height+window_height/2;
  }
}

void initialize_g_draw_buffer(void){
  for(int i=0;i<window_width*DRAW_NLINES/2;i++){
    ((uint32_t*)g_zbuff)[i]=0xFFFFFFFF;
    ((uint32_t*)g_drawbuff[lastbuff])[i]=0x84108410;/*RGB*/
  }
}

void initialize_draw_buffer(int ps){
#ifndef OMIT_ZBUFFER_CONFLICT
  for(int i=0;i<window_width*DRAW_NLINES;i++){
    clientps[ps].zbuff[i]=65535;
    clientps[ps].drawbuff[i]=0x8410;/*RGB*/
  }
#endif
}



void generate_polygons_and_draw(unsigned int split,unsigned int num,uint16_t *zbuff,uint16_t *drawbuff){
  int searchidx = MAXPROC_POLYNUM*num/split;
  unsigned mati=0;
  int nextmati=0;
  fvector4 v[3];
  int zovercount = 0;

  if(num)
    initialize_draw_buffer(num-1);
  nextmati = materiallist[mati].num;

  for(int i=num;i<POLYNUM;i+=split){
    while(i >= nextmati){
      mati ++;
      nextmati += materiallist[mati].num;
    }
    if(drawidx[i])continue;
    for(int j=0;j<3;j++){
      v[j] = poly_transed[polyvec[i][j]];
    }
    if((draw_y*DRAW_NLINES+DRAW_NLINES < v[0].y&&draw_y*DRAW_NLINES+DRAW_NLINES < v[1].y&&draw_y*DRAW_NLINES+DRAW_NLINES < v[2].y))continue;
    drawidx[i] = 1;
    zovercount = 0;
    for(int j=0;j<3;j++){
      if(v[j].w < ZNEAR){
	zovercount++;
      }
    }
    if(zovercount==0&&unvisible(v))continue;

    //光量の計算
    float light = 1.0f;
    //テクスチャのデータ
    const texture_t tex={
      texturelist[materiallist[mati].texture],//vector2(4,4)
    };
    fvector2 puv[3];

    for(int j=0;j<3;j++){
      puv[j].x = (1.f-point_uv[polyvec[i][j]].x)*SIZE_TEX;
      puv[j].y = (point_uv[polyvec[i][j]].y)*SIZE_TEX;
    }
    if(zovercount == 1){
      fvector2 uv_near;
      fvector4 v_near;
      printf("sprit:1");
      // 1 vertex is too near the camera.
      // split 2 triangles from 1 triangle.

      // triangle1
      float t1,t2;
      int i1,i2,i3;
      if(v[0].w < ZNEAR){
	i1 = 0;
	i2 = 1;
	i3 = 2;
      }else if(v[1].w < ZNEAR){
	i1 = 1;
	i2 = 0;
	i3 = 2;
      }else{
	i1 = 2;
	i2 = 0;
	i3 = 1;
      }
      uv_near = puv[i1];
      v_near = v[i1];
      v_near.resetperspective();
      t1 = 1.f-(v[i1].w-ZNEAR)/(v[i1].w-v[i2].w);
      t2 = 1.f-(v[i1].w-ZNEAR)/(v[i1].w-v[i3].w);
      printf(",%f,%f\n",t1,t2);
      v[i2].resetperspective();
      v[i1] = v_near*t1+v[i2]*(1.f-t1);
      v[i2].setperspective();
      v[i1].setperspective();
      puv[i1] = uv_near*t1+puv[i2]*(1.f-t1);

      while(searchidx < MAXPROC_POLYNUM&&tri[searchidx/TRI_MULTI][searchidx%TRI_MULTI].isexisting)searchidx++;
      if(searchidx == MAXPROC_POLYNUM){
	printf("TOO MUCH FACES IN THIS LINE\n");
	break;
      }
      if(tri[searchidx/TRI_MULTI][searchidx%TRI_MULTI].triangle_set(v,light,&tex,puv)==-1||
	 tri[searchidx/TRI_MULTI][searchidx%TRI_MULTI].draw(zbuff,drawbuff,draw_y*DRAW_NLINES))
	tri[searchidx/TRI_MULTI][searchidx%TRI_MULTI].isexisting = 0;

      //triangle2
      v[i3].resetperspective();
      v[i2] = v_near*t2+v[i3]*(1.f-t2);
      v[i2].setperspective();
      v[i3].setperspective();
      puv[i2] = uv_near*t2+puv[i3]*(1.f-t2);
    }else if(zovercount == 2){
      printf("sprit:2\n");
      float t1,t2;
      int i1,i2,i3;
      if(v[0].w > ZNEAR){
	i1 = 0;
	i2 = 1;
	i3 = 2;
      }else if(v[1].w > ZNEAR){
	i1 = 1;
	i2 = 0;
	i3 = 2;
      }else{
	i1 = 2;
	i2 = 0;
	i3 = 1;
      }
      printf("sprit:2\n");
      t1 = 1.f-(v[i1].w-ZNEAR)/(v[i1].w-v[i2].w);
      t2 = 1.f-(v[i1].w-ZNEAR)/(v[i1].w-v[i3].w);
      v[i1].resetperspective();
      v[i2].resetperspective();
      v[i3].resetperspective();
      v[i2] = v[i1]*t1+ v[i2]*(1.f-t1);
      puv[i2] = puv[i1]*t1+ puv[i2]*(1.f-t1);
      v[i3] = v[i1]*t2 + v[i3]*(1.f-t2);
      puv[i3] = puv[i1]*t2+ puv[i3]*(1.f-t2);
      v[i1].setperspective();
      v[i2].setperspective();
      v[i3].setperspective();
      //      printf("%f,%f",v[i2].w);
      //      continue
    }else if(zovercount == 3)continue;//unvisible(behind the camera.)

    while(searchidx < MAXPROC_POLYNUM&&tri[searchidx/TRI_MULTI][searchidx%TRI_MULTI].isexisting)searchidx++;
    if(searchidx == MAXPROC_POLYNUM){
      printf("TOO MUCH FACES IN THIS LINE\n");
      break;
    }
    if(tri[searchidx/TRI_MULTI][searchidx%TRI_MULTI].triangle_set(v,light,&tex,puv)==-1||
       tri[searchidx/TRI_MULTI][searchidx%TRI_MULTI].draw(zbuff,drawbuff,draw_y*DRAW_NLINES))
      tri[searchidx/TRI_MULTI][searchidx%TRI_MULTI].isexisting = 0;
  }
}

void draw_polygons(unsigned int split,unsigned int num,uint16_t *zbuff,uint16_t *drawbuff){
  for(unsigned i=num;i<MAXPROC_POLYNUM;i+=split){
    if(tri[i/TRI_MULTI][i%TRI_MULTI].isexisting){
      if(tri[i/TRI_MULTI][i%TRI_MULTI].draw(zbuff,drawbuff,draw_y*DRAW_NLINES)){
	tri[i/TRI_MULTI][i%TRI_MULTI].isexisting = 0;
      }
    }
  }
}

void *vTask(void* prm){
  fvector3 n;
  int ps = (unsigned long long)(prm);
  printf("ps:%d, begin\n",ps);
#ifndef OMIT_ZBUFFER_CONFLICT
  clientps[ps].drawbuff = new uint16_t[window_width*DRAW_NLINES];
  clientps[ps].zbuff = new uint16_t[window_width*DRAW_NLINES];
#endif

  while(1){
    switch(clientps[ps].vtaskstate){
    case VTASK_WAIT:
      // usleep(0);
      break;

    case VTASK_VERTEX_CALCULATION:
      calc_vertices((ps+1)*POINTNUM/PROCESSNUM,(ps+2)*POINTNUM/PROCESSNUM);
      clientps[ps].vtaskstate = VTASK_WAIT;
      break;

    case VTASK_POLY_GENERATE:
      generate_polygons_and_draw(PROCESSNUM,ps+1,g_zbuff,g_drawbuff[lastbuff]);
      clientps[ps].vtaskstate = VTASK_WAIT;
      break;

    case VTASK_POLY_DRAW:
    // case VTASK_DRAW:
#ifdef OMIT_ZBUFFER_CONFLICT
      draw_polygons(PROCESSNUM,ps+1,g_zbuff,g_drawbuff[lastbuff]);
#else
      draw_polygons(PROCESSNUM,ps+1,clientps[ps].zbuff,clientps[ps].drawbuff);
#endif
      clientps[ps].vtaskstate = VTASK_WAIT;
      break;
    }
  }
}

void barrier_sync(void){
  while(1){
    int s;
    // usleep(0);
    for(s=0;s<PROCESSNUM-1&&clientps[s].vtaskstate == VTASK_WAIT;s++);
    if(s==PROCESSNUM-1)break;
  }
}

void merge_screen(void){
  #ifndef OMIT_ZBUFFER_CONFLICT
  for(int i=0;i<PROCESSNUM-1;i++){
    for(int j=0;j<window_width*DRAW_NLINES;j++){
      if(g_zbuff[j] > clientps[i].zbuff[j]){
	g_zbuff[j] = clientps[i].zbuff[j];
	g_drawbuff[lastbuff][j] = clientps[i].drawbuff[j];
      }
    }
  }
  #endif
}

int main3d(void){
  Matrix4 projection;
  Matrix4 obj;
#ifdef USE_SDL
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* sdlWindow;
  SDL_Event	ev;
  SDL_Texture *sdlTexture;
  SDL_Renderer *sdlRenderer;

  SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_OPENGL, &sdlWindow, &sdlRenderer);
  sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_TARGET, window_width, DRAW_NLINES);
#endif
#ifndef DISABLE_ANIMATION
  bl.init(bones,sizeof(bones)/sizeof(bones[0]),sizeof(mixedbone)/sizeof(mixedbone[0]));
#endif
  g_drawbuff[0] = new uint16_t[window_width*DRAW_NLINES];
  g_drawbuff[1] = new uint16_t[window_width*DRAW_NLINES];
  g_zbuff = new uint16_t[window_width*DRAW_NLINES];

  // t = new texturetriangle[MAXPROC_POLYNUM];
  for(int i=0;i<MAXPROC_POLYNUM/TRI_MULTI;i++){
    tri[i] = new texturetriangle[TRI_MULTI];
    for(int j=0;j<TRI_MULTI;j++){
      tri[i][j].isexisting = 0;
    }
  }

  //透視投影行列
  projection=loadPerspective(0.25f,float(window_height)/window_width,ZNEAR,30.f,0,0)*projection;

  fvector3 viewdir;
  float dist = 3.f;
  fvector2 np = fvector2(150.f,0);
  // float fps = 0.f;

  fvector3 n;

  float disttarget = 30.f;
  fvector3 transtarget = fvector3(0,-12,-1.2);
  fvector3 trans = fvector3(0,-12,-1.2);

  veye = fvector3(0,0,-15.5f);
  while(1){
#ifdef VISUALDEBUG
    vdb_begin();
    vdb_frame();
#endif

#ifdef USE_SDL
    /* usleep(20000); */
    if(SDL_PollEvent(&ev)){
      if(ev.type == SDL_QUIT)	{
	break;
      }
    }
#endif
    // disttarget -= 0.05f;
    if(0){
      getchar();
      if(disttarget == 30.f){
	transtarget = fvector3(0,-17,-1.2);
	disttarget = 10.f;
      }else{
	transtarget = fvector3(0,-11,-1.2);
	disttarget = 30.f;
      }
    }

    //視点計算
    dist = dist*0.7+disttarget*0.3;// + 1.4f*cosf(np.x/150.f*3.14159265358979324f);
    trans = trans*0.7+transtarget*0.3;

    veye = -fvector3(cosf(np.x/300.f*3.14159265f)*cosf(np.y/300.f*3.14159265f),sinf(np.y/300.f*3.14159265f),sinf(np.x/300.f*3.14159265f)*cosf(np.y/300.f*3.14159265f));
    //透視投影行列とカメラ行列の合成
    m=projection*lookat(fvector3(0,0,100),veye*dist)*obj*translation(trans);
#ifndef DISABLE_ANIMATION
    bl.calcall(m);
#endif

    //頂点データを変換
    for(int s=0;s<PROCESSNUM-1;s++)clientps[s].vtaskstate = VTASK_VERTEX_CALCULATION;
    calc_vertices(0,POINTNUM/PROCESSNUM);
    barrier_sync();

    for(int i=0;i<MAXPROC_POLYNUM;i++){
      tri[i/TRI_MULTI][i%TRI_MULTI].isexisting = 0;
    }
    for(int i=0;i<POLYNUM;i++){
      drawidx[i] = 0;
    }

    //ラインごとに描画しLCDに転送
    for(draw_y=0;draw_y<window_height/DRAW_NLINES;draw_y++){
      initialize_g_draw_buffer();

      for(int s=0;s<PROCESSNUM-1;s++)clientps[s].vtaskstate = VTASK_POLY_DRAW;
      //描画ステージ
      draw_polygons(PROCESSNUM,0,g_zbuff,g_drawbuff[lastbuff]);
      barrier_sync();

      for(int s=0;s<PROCESSNUM-1;s++)clientps[s].vtaskstate = VTASK_POLY_GENERATE;
      generate_polygons_and_draw(PROCESSNUM,0,g_zbuff,g_drawbuff[lastbuff]);
      barrier_sync();

      merge_screen();
#ifdef USE_SDL
      SDL_UpdateTexture(sdlTexture, NULL, (uint8_t*)g_drawbuff[lastbuff], window_width*2);
      {
	SDL_Rect dstrect;
	dstrect.x = 0;
	dstrect.y = draw_y*DRAW_NLINES;
	dstrect.w = window_width;
	dstrect.h = DRAW_NLINES;
	SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &dstrect);
      }
#else
      send_line(draw_y*DRAW_NLINES,(uint8_t*)(g_drawbuff[lastbuff]));
#endif
      lastbuff = 1-lastbuff;
    }
#ifdef USE_SDL
    SDL_RenderPresent(sdlRenderer);
#endif
#ifdef VISUALDEBUG
    vdb_end();
#endif
  }
#ifdef USE_SDL
  SDL_Quit();
#endif
  return 0;
}
