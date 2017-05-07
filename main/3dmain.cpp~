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

extern "C"{
void toggleLED(void);
}

#include "poly.h"
// const fvector2_t point_uv[]={
//   {-15.67f, 126.91f, -15.04f},
//   {0.00f, 86.24f, -18.91f},
//   {15.67f, 126.91f, -15.04f},
//   {17.50f, 102.93f, -14.57f},
//   {-17.50f, 102.93f, -14.57f},
//   {13.39f, 92.29f, -16.59f},
//   {-13.39f, 92.29f, -16.59f},
//   {0.00f, 126.91f, -24.75f},
//   {0.00f, 105.80f, -24.75f},
//   {0.00f, 97.52f, -26.07f},
//   {15.30f, 92.79f, -5.66f},
//   {18.65f, 105.80f, -8.47f},
//   {-18.65f, 105.80f, -8.47f},
//   {-15.30f, 92.79f, -5.66f},
//   {17.62f, 118.17f, -16.62f},
//   {0.00f, 115.88f, -27.40f},
//   {-17.62f, 118.17f, -16.62f},
//   {0.00f, 89.13f, 25.14f},
//   {0.00f, 88.17f, -5.66f},
//   {15.30f, 92.79f, -5.66f},
//   {18.65f, 105.80f, -8.47f},
//   {-18.65f, 105.80f, -8.47f},
//   {-15.30f, 92.79f, -5.66f},
//   {17.62f, 118.17f, -16.62f},
//   {-17.62f, 118.17f, -16.62f},
//   {18.53f, 89.68f, 21.75f},
//   {24.15f, 90.34f, -3.60f},
//   {-18.53f, 89.68f, 21.75f},
//   {-24.15f, 90.34f, -3.60f},
//   {0.00f, 89.13f, 25.14f},
//   {0.00f, 126.63f, -30.57f},
//   {19.15f, 111.03f, -24.39f},
//   {15.30f, 126.51f, -21.60f},
//   {23.39f, 116.43f, -10.63f},
//   {-19.15f, 111.03f, -24.39f},
//   {0.00f, 108.67f, -30.08f},
//   {-15.30f, 126.51f, -21.60f},
//   {-23.39f, 116.43f, -10.63f},
//   {0.00f, 138.70f, -19.18f},
//   {16.82f, 133.40f, -8.12f},
//   {0.00f, 140.98f, -4.98f},
//   {-16.82f, 133.40f, -8.12f},
//   {23.58f, 105.61f, 3.81f},
//   {19.20f, 126.14f, 11.08f},
//   {-19.20f, 126.14f, 11.08f},
//   {-23.58f, 105.61f, 3.81f},
//   {0.00f, 137.75f, 13.79f},
//   {14.45f, 101.95f, 20.52f},
//   {0.00f, 123.52f, 22.32f},
//   {-14.45f, 101.95f, 20.52f},
//   {-33.47f, 130.01f, -20.16f},
//   {-22.39f, 147.79f, -20.16f},
//   {-25.45f, 136.34f, -20.16f},
//   {-11.20f, 128.75f, -20.16f},
//   {17.62f, 118.17f, -16.62f},
//   {17.50f, 102.93f, -14.57f},
//   {-17.50f, 102.93f, -14.57f},
//   {-17.62f, 118.17f, -16.62f},
//   {-14.45f, 101.95f, 20.52f},
//   {14.45f, 101.95f, 20.52f},
//   {0.00f, 108.67f, -30.08f},
//   {-4.85f, 83.50f, -10.77f},
//   {-13.44f, 73.60f, 8.34f},
//   {4.85f, 83.50f, -10.77f},
//   {13.44f, 73.60f, 8.34f},
//   {19.20f, 81.53f, 8.81f},
//   {-19.20f, 81.53f, 8.81f},
//   {-15.26f, 58.02f, -11.43f},
//   {-19.02f, 25.90f, -15.97f},
//   {19.02f, 25.90f, -15.97f},
//   {18.55f, 25.90f, 20.15f},
//   {-18.55f, 25.90f, 20.15f},
//   {13.44f, 73.39f, -10.48f},
//   {-13.44f, 73.39f, -10.48f},
//   {-19.72f, 80.95f, -8.62f},
//   {19.72f, 80.95f, -8.62f},
//   {17.48f, 73.92f, -8.63f},
//   {18.39f, 75.22f, 8.12f},
//   {-17.48f, 73.92f, -8.63f},
//   {-18.39f, 75.22f, 8.12f},
//   {-24.28f, 78.83f, -2.58f},
//   {24.28f, 78.83f, -2.58f},
//   {20.55f, 72.21f, -2.91f},
//   {-20.55f, 72.21f, -2.91f},
//   {15.26f, 58.02f, -11.43f},
//   {0.00f, 87.27f, 8.81f},
//   {0.00f, 25.90f, -40.85f},
//   {0.00f, 75.53f, 10.03f},
//   {0.00f, 25.90f, 41.42f},
//   {15.26f, 58.02f, 15.26f},
//   {43.32f, 25.90f, -0.03f},
//   {20.55f, 72.21f, 3.10f},
//   {0.00f, 47.49f, 0.00f},
//   {-43.32f, 25.90f, -0.03f},
//   {51.82f, 57.97f, -2.17f},
//   {24.28f, 78.83f, 2.76f},
//   {-20.55f, 72.21f, 3.10f},
//   {-51.82f, 57.97f, -2.17f},
//   {-24.28f, 78.83f, 2.76f},
//   {14.07f, 86.83f, 0.00f},
//   {-14.07f, 86.83f, 0.00f},
//   {23.24f, 83.37f, 0.00f},
//   {-23.24f, 83.37f, 0.00f},
//   {7.32f, 87.57f, 0.00f},
//   {0.00f, 88.26f, 4.41f},
//   {-7.32f, 87.57f, 0.00f},
//   {0.00f, 85.47f, -4.41f},
//   {0.00f, 95.21f, -1.66f},
//   {6.69f, 95.71f, -1.66f},
//   {0.00f, 96.25f, 1.94f},
//   {-6.69f, 95.71f, -1.66f},
//   {0.00f, 94.96f, -5.27f},
//   {0.00f, 85.08f, -8.81f},
//   {10.73f, 86.06f, -12.24f},
//   {0.00f, 81.19f, -14.28f},
//   {13.44f, 74.47f, -12.99f},
//   {-10.73f, 86.06f, -12.24f},
//   {-13.44f, 74.47f, -12.99f},
//   {36.69f, 70.36f, 2.18f},
//   {34.23f, 65.98f, 2.69f},
//   {-36.69f, 70.36f, 2.18f},
//   {-34.23f, 65.98f, 2.69f},
//   {-15.26f, 58.02f, 15.26f},
//   {-16.19f, 61.88f, -12.52f},
//   {16.19f, 61.88f, -12.52f},
//   {0.00f, 61.15f, -19.46f},
//   {34.92f, 71.57f, -2.14f},
//   {32.28f, 66.87f, -2.63f},
//   {-34.92f, 71.57f, -2.14f},
//   {-32.28f, 66.87f, -2.63f},
//   {38.47f, 69.15f, -2.00f},
//   {36.18f, 65.09f, -2.54f},
//   {-38.47f, 69.15f, -2.00f},
//   {-36.18f, 65.09f, -2.54f},
//   {23.24f, 83.37f, 0.00f},
//   {14.07f, 86.83f, 0.00f},
//   {-14.07f, 86.83f, 0.00f},
//   {-23.24f, 83.37f, 0.00f},
//   {-24.28f, 78.83f, 2.76f},
//   {24.28f, 78.83f, 2.76f},
//   {-15.26f, 58.02f, 15.26f},
//   {-43.32f, 25.90f, -0.03f},
//   {43.32f, 25.90f, -0.03f},
//   {15.26f, 58.02f, 15.26f},
//   {0.00f, 81.19f, -14.28f},
//   {4.85f, 83.50f, -10.77f},
//   {-4.85f, 83.50f, -10.77f},
//   {0.00f, 81.19f, -14.28f},
//   {-7.32f, 87.57f, 0.00f},
//   {7.32f, 87.57f, 0.00f},
//   {-13.44f, 73.39f, -10.48f},
//   {13.44f, 73.39f, -10.48f},
//   {-20.55f, 72.21f, 3.10f},
//   {-13.44f, 73.60f, 8.34f},
//   {20.55f, 72.21f, 3.10f},
//   {13.44f, 73.60f, 8.34f},
//   {-19.02f, 25.90f, -15.97f},
//   {-43.32f, 25.90f, -0.03f},
//   {0.00f, 25.90f, -40.85f},
//   {19.02f, 25.90f, -15.97f},
//   {43.32f, 25.90f, -0.03f},
//   {-18.55f, 25.90f, 20.15f},
//   {0.00f, 25.90f, 41.42f},
//   {18.55f, 25.90f, 20.15f},
//   {7.32f, 87.57f, 0.00f},
//   {0.00f, 88.26f, 4.41f},
//   {-7.32f, 87.57f, 0.00f},
//   {0.00f, 81.19f, -14.28f},
//   {24.28f, 78.83f, 2.76f},
//   {24.28f, 78.83f, -2.58f},
//   {20.55f, 72.21f, 3.10f},
//   {20.55f, 72.21f, -2.91f},
//   {-24.28f, 78.83f, -2.58f},
//   {-24.28f, 78.83f, 2.76f},
//   {-20.55f, 72.21f, 3.10f},
//   {-20.55f, 72.21f, -2.91f},
//   {0.00f, 81.19f, -14.28f},
//   {0.00f, 85.08f, -8.81f},
//   {3.61f, 41.37f, -4.35f},
//   {14.91f, 41.36f, -4.35f},
//   {10.53f, 0.50f, -12.94f},
//   {14.91f, 40.90f, 7.93f},
//   {3.61f, 40.90f, 7.94f},
//   {-3.61f, 41.37f, -4.35f},
//   {-14.91f, 41.36f, -4.35f},
//   {-10.53f, 0.50f, -12.94f},
//   {-14.91f, 40.90f, 7.93f},
//   {-3.61f, 40.90f, 7.94f},
//   {13.85f, 24.53f, 2.39f},
//   {5.38f, 24.54f, 2.39f},
//   {-13.85f, 24.53f, 2.39f},
//   {-5.38f, 24.54f, 2.39f},
//   {5.38f, 6.12f, -3.63f},
//   {13.84f, 6.12f, -3.62f},
//   {13.84f, 0.10f, 4.74f},
//   {5.37f, 0.10f, 4.74f},
//   {-5.38f, 6.12f, -3.63f},
//   {-13.84f, 6.12f, -3.62f},
//   {-13.84f, 0.10f, 4.74f},
//   {-5.37f, 0.10f, 4.74f},
//   {5.38f, 20.49f, -5.93f},
//   {13.85f, 20.48f, -5.93f},
//   {5.38f, 15.70f, 2.49f},
//   {13.85f, 15.69f, 2.49f},
//   {-13.85f, 20.48f, -5.93f},
//   {-5.38f, 20.49f, -5.93f},
//   {-13.85f, 15.69f, 2.49f},
//   {-5.38f, 15.70f, 2.49f},
//   {-5.37f, 0.10f, 4.74f},
//   {-5.38f, 24.54f, 2.39f},
//   {-3.61f, 40.90f, 7.94f},
//   {-5.38f, 15.70f, 2.49f},
//   {-5.37f, 0.10f, 4.74f},
//   {5.38f, 24.54f, 2.39f},
//   {3.61f, 40.90f, 7.94f},
//   {5.38f, 15.70f, 2.49f},
//   {5.37f, 0.10f, 4.74f},
//   {5.37f, 0.10f, 4.74f},
//   {-91.38f, 113.91f, -3.63f},
//   {-34.31f, 32.40f, -3.63f},
//   {-88.13f, 116.19f, -3.63f},
//   {-31.06f, 34.68f, -3.63f},
//   {-88.13f, 116.19f, 3.63f},
//   {-31.06f, 34.68f, 3.63f},
//   {-91.38f, 113.91f, 3.63f},
//   {-34.31f, 32.40f, 3.63f},
//   {-31.06f, 34.68f, 3.63f},
//   {-88.13f, 116.19f, 3.63f},
//   {-34.31f, 32.40f, 3.63f},
//   {-91.38f, 113.91f, 3.63f},
//   {-31.06f, 34.68f, -3.63f},
//   {-34.31f, 32.40f, -3.63f},
//   {-91.20f, 141.27f, 3.97f},
//   {-80.60f, 139.56f, -6.34f},
//   {-75.51f, 134.49f, 6.91f},
//   {-81.46f, 127.68f, 15.14f},
//   {-97.72f, 130.24f, -7.71f},
//   {-77.69f, 127.13f, -11.41f},
//   {-95.06f, 124.81f, 11.27f},
//   {-86.23f, 114.53f, -6.42f},
//   {-73.47f, 124.38f, 0.00f},
//   {-86.39f, 113.54f, 7.17f},
//   {-95.47f, 116.15f, 0.42f},
//   {-74.30f, 99.45f, 0.00f},
//   {-77.07f, 114.55f, 0.04f},
//   {-86.39f, 113.54f, 7.17f},
//   {-74.30f, 99.45f, 0.00f},
//   {-81.46f, 127.68f, 15.14f}
// };

// const fvector3_t pointvec[]={
//   {-0.8f, -0.8f, -0.8f },//0
//   {-0.8f, -0.8f,  0.8f },//1
//   {-0.8f,  0.8f, -0.8f },//2
//   {-0.8f,  0.8f,  0.8f },//3
//   { 0.8f, -0.8f, -0.8f },//4
//   { 0.8f, -0.8f,  0.8f },//5
//   { 0.8f,  0.8f, -0.8f },//6
//   { 0.8f,  0.8f,  0.8f },//7
// };

const int wireframe[12][2]={
  {0,1},//0
  {0,2},//1
  {0,4},//2
  {1,3},//3
  {1,5},//4
  {2,3},//5
  {2,6},//6
  {3,7},//7
  {4,5},//8
  {4,6},//9
  {5,7},//10
  {6,7},//11
};

// const int polyvec[][3]={
//   {1,0,2},
//   {1,3,2},
//   {1,3,7},
//   {1,5,7},
//   {0,2,6},
//   {0,4,6},
//   {2,3,7},
//   {2,6,7},
//   {0,4,5},
//   {0,1,5},
//   {4,5,7},
//   {4,6,7},
// };

#define POLYNUM int(sizeof(polyvec)/sizeof(polyvec[0]))
#define POINTNUM int(sizeof(pointvec)/sizeof(pointvec[0]))
#define WIRENUM int(sizeof(wireframe)/sizeof(wireframe[0]))

vector3 pv[12][3];

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

uint16_t drawbuff[2][window_width];

int main3d(void){
  Matrix4 m;
  Matrix4 projection;
  Matrix4 obj;
  
  // {
  //   vector3 pv[3]={vector3(30,10,0),vector3(80,40,65536),vector3(60,110,0)};
  //   triangle t(pv,0);
  //   while(!t.draw(zlinebuf,sdl)){
  //     for(int i=0;i<160;i++){
  // 	zlinebuf[i]=0x7FFFFFFF;
  //     }
  //     sdl.flush();
  //     sdl.delay(1);
  //   }
  // }
  // for(int i=0;i<300;i++){
  //   sdl.flush();
  //   sdl.delay(10);
  // }

  projection=loadPerspective(0.25f,float(window_height)/window_width,0.0001f,3.f,0,0)*projection;

  fvector3 viewdir;
  fvector3 veye;
  float dist = 2;
  vector2 mouse;
  vector2 pmouse;
  vector2 np;
  vector2 pnp;
  bool clicking = false;
  
  veye = fvector3(0,0,-2.5f);
  obj = obj*magnify(1);
  while(1){
    np.x+=5;
    //    dist = sinf(f*3.141592f/86.f)+2.8f;
    veye = -fvector3(cos(np.x/300.f*3.14159265f)*cos(np.y/300.*3.14159265f),sin(np.y/300.*3.14159265f),sin(np.x/300.*3.14159265f)*cos(np.y/300.*3.14159265f));
    m=projection*lookat(fvector3(0,0,0),veye*dist);
    
    // print(m);

    //    obj=obj*translation(vector3(0,0,65536*3));
    //    obj=rotatey(frame*5);
    // m=m*rotatez(5461);

    //  m=m*magnify(2);
    
    
    fvector3 aax[2];
    for(int j=0;j<WIRENUM;j++){
      for(int i=0;i<2;i++){
	aax[i] = m.applyit(fvector3(pointvec[wireframe[j][i]]));
      }
      aax[0].x=aax[0].x*window_width+window_width/2;aax[0].y=aax[0].y*window_height+window_height/2;
      aax[1].x=aax[1].x*window_width+window_width/2;aax[1].y=aax[1].y*window_height+window_height/2;
      
      //sdl.Line(aax[0],aax[1]);
    }
    fvector4 vo[3];
    fvector4 v[3];
    fvector3 n;
    int tnum=0;

    fvector4 obj_transed[POINTNUM];
    fvector4 poly_transed[POINTNUM];
  
    float zlinebuf[window_width];
    texturetriangle t[POLYNUM];

    for(int j=0;j<POINTNUM;j++){
      obj_transed[j] = fvector4(pointvec[j].x,pointvec[j].y,pointvec[j].z);
      poly_transed[j] = m.applyit_v4(fvector3(pointvec[j]));
      //std::cout<<"poly"<<poly_transed[j].x<<","<<poly_transed[j].y<<","<<poly_transed[j].z<<std::endl;
    }

    for(int i=0;i<POLYNUM;i++){
      for(int j=0;j<3;j++){
	v[j] = poly_transed[polyvec[i][j]];
	vo[j] = obj_transed[polyvec[i][j]];
      }

      //      if(v[0].z < 0) break;
      if(!(
	   !((abs(v[0].x) > 1.f)||(abs(v[0].y) > 1.f)||(abs(v[0].z) > 1.f))||
	   !((abs(v[1].x) > 1.f)||(abs(v[1].y) > 1.f)||(abs(v[1].z) > 1.f))||
	   !((abs(v[2].x) > 1.f)||(abs(v[2].y) > 1.f)||(abs(v[2].z) > 1.f))))
	continue;

      v[0].x=v[0].x*window_width+window_width/2;v[0].y=v[0].y*window_height+window_height/2;
      v[1].x=v[1].x*window_width+window_width/2;v[1].y=v[1].y*window_height+window_height/2;
      v[2].x=v[2].x*window_width+window_width/2;v[2].y=v[2].y*window_height+window_height/2;

      //      std::cout<<v1.x
      //sdl.setbrush();
      const texture_t tex={
	stonetex,vector2(4,4)
      };
      fvector2 puv[3];
      for(int j=0;j<3;j++){
	puv[j].x = (1.f-point_uv[polyvec[i][j]].x)*SIZE_TEX;
	puv[j].y = (point_uv[polyvec[i][j]].y)*SIZE_TEX;
      }
      float light = abs(loadPower(fvector3(),veye,vo))*0.9f+0.1f;
      if(light+1){
	if(t[tnum++].triangle_set(v,light,&tex,puv)==-1)tnum--;
      }
      //int loadPower(const vector3 &light_pos,const vector3 &light_n,const vector3 obj[3]){

    }

    int draworder[POLYNUM];
    float zdata[POLYNUM];
    for(int i=0;i<tnum;i++){
      zdata[i] = t[i].p[1].z+t[i].p[0].z+t[i].p[2].z;
      draworder[i] = i;
    }

    float tmpdata;
    int tmpsubdata;
    int j;

    for (int i = 1; i < tnum; i++) {
      tmpdata = zdata[i];
      tmpsubdata = draworder[i];
      if (zdata[i-1] > zdata[i]) {
        j = i;
        do {
	  zdata[j] = zdata[j-1];
	  draworder[j] = draworder[j-1];
	  j--;
        } while (j > 0 && zdata[j - 1] > tmpdata);
        zdata[j] = tmpdata;
	draworder[j] = tmpsubdata;
      }
    }
    
    for(int y=0;y<window_height;y++){
      for(int i=0;i<window_width;i++){
	zlinebuf[i]=1.f;
	drawbuff[y&1][i]=0x1000;
      }
      for(int i=0;i<tnum;i++){
	if(t[draworder[i]].ymin < y&&t[draworder[i]].ymax >= y){
	  t[draworder[i]].draw(zlinebuf,drawbuff[y&1]);
	}
      }
      LCD_out(0,y,drawbuff[y&1],160);
    }

    fvector3 vp;
    // np.x -= 300;
    // np.y -= 200;
    // vp.x = -cos(np.x/300.)*cos(np.y/300.)*4000;
    // vp.y = 0;
    // vp.z = -sin(np.x/300.)*cos(np.y/300.)*4000;
    // np.x += 300;
    // np.y += 200;
    // if(isKeyPressed()){
    //   switch(getkey()){
    //   case SDLK_UP:
    // 	veye = veye + vp;
    // 	break;
    //   case SDLK_DOWN:
    // 	veye = veye - vp;
    // 	break;
    //   case SDLK_LEFT:
    // 	np.x += 20;
    // 	break;
    //   case SDLK_RIGHT:
    // 	np.x -= 20;
    // 	break;
    //   case 'a':
    // 	np.y += 20;
    // 	break;
    //   case 's':
    // 	np.y -= 20;
    // 	break;
    //   }
    // }

    // // for(int i=0;i<3;i++){
    // frame++;
    // if(prevsec!=time(NULL)){
    //   prevsec=time(NULL);
    //   printf("%d\n",frame);
    //   frame=0;
    // }
    if(frame%2==0){
      for(int i=0;i<1;i++){
	// sdl.flush();
	
      }
    }
    // sdl.screenclear();
  }
}

#if 0
//#define NVERTEX 8
//#define NPOLY 12
//#define NMATERIAL 1
//
//// 頂点の初期配置リスト
//const Vec3 init_vertex[NVERTEX] = {
//	{-40, 20, -40},
//	{+40, 20, -40},
//	{-40, 100, -40},
//        {+40, 100, -40},
//	{-40, 20, +40},
//	{+40, 20, +40},
//	{-40, 100, +40},
//        {+40, 100, +40}
//};
//
//// ポリゴン(頂点番号の組み合わせ)
//const unsigned int init_poly[NPOLY][3] = {
//	{0, 1, 2},
//        {2, 1, 3},
//        {4, 2, 6},
//        {4, 0, 2},
//        {1, 5, 3},
//        {5, 7, 3},
//        {1, 0, 5},
//        {0, 4, 5},
//        {2, 3, 6},
//        {3, 7, 6},
//        {4, 6, 7},
//        {4, 7, 5}
//};
//
//// 各ポリゴンの材料番号
//const unsigned int material[NPOLY] = {
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0
//};
//
//// 材料（ポリゴンの色）のリスト
//const unsigned int material_list[NMATERIAL] = {
//	0x73FF  // 空色
//};




#define NVERTEX 248
#define NPOLY 300
#define NMATERIAL 1

const Vec3 init_vertex[NVERTEX] = {
};
#endif




