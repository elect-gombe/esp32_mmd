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
  void send_line(int ypos, uint16_t *line) ;
  int main3d();
}

#include "poly.h"

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

fvector4 obj_transed[POINTNUM];
fvector4 poly_transed[POINTNUM];

float zlinebuf[window_width];

//ソート用のデータの作成
int draworder[POLYNUM];
float zdata[POLYNUM];
int main3d(void){
  texturetriangle *t[POLYNUM];
  Matrix4 m;
  Matrix4 projection;
  Matrix4 obj;

  for(int i=0;i<POLYNUM;i++){
    t[i] = new texturetriangle;
  }

  //透視投影行列
  projection=loadPerspective(0.25f,float(window_height)/window_width,0.0001f,3.f,0,0)*projection;

  fvector3 viewdir;
  fvector3 veye;
  float dist = 5.f;
  vector2 mouse;
  vector2 pmouse;
  vector2 np;
  vector2 pnp;
  bool clicking = false;
  
  veye = fvector3(0,0,-15.5f);
  obj = obj*magnify(1);
  while(1){
    np.x+=10;    //camera rotation
    //視点計算
    veye = -fvector3(cos(np.x/300.f*3.14159265f)*cos(np.y/300.*3.14159265f),sin(np.y/300.*3.14159265f),sin(np.x/300.*3.14159265f)*cos(np.y/300.*3.14159265f));
    //透視投影行列とカメラ行列の合成
    m=projection*lookat(fvector3(0,0,0),veye*dist);    
    
    fvector4 vo[3];
    fvector4 v[3];
    fvector3 n;
    int tnum=0;

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
      if(!(
	   !((abs(v[0].x) > 1.f)||(abs(v[0].y) > 1.f)||(abs(v[0].z) > 1.f))||
	   !((abs(v[1].x) > 1.f)||(abs(v[1].y) > 1.f)||(abs(v[1].z) > 1.f))||
	   !((abs(v[2].x) > 1.f)||(abs(v[2].y) > 1.f)||(abs(v[2].z) > 1.f))))
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
      float light = abs(loadPower(fvector3(),veye,vo))*0.9f+0.1f;
      if(light+1){
	if(t[tnum++]->triangle_set(v,light,&tex,puv)==-1)tnum--;
      }
    }
    
    for(int i=0;i<tnum;i++){
      //      zdata[i] = t[i].p[1].z+t[i].p[0].z+t[i].p[2].z;
      draworder[i] = i;
    }

    float tmpdata;
    int tmpsubdata;
    int j;
    //Z順にソート
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
    
    //ラインごとに描画しLCDに転送
    for(int y=0;y<window_height;y++){
      for(int i=0;i<window_width;i++){
	zlinebuf[i]=1.f;
	drawbuff[y&1][i]=0x1000;
      }
      for(int i=0;i<tnum;i++){
	if(t[draworder[i]]->ymin < y&&t[draworder[i]]->ymax >= y){
	  t[draworder[i]]->draw(zlinebuf,drawbuff[y&1]);
	}
      }
      send_line(y,drawbuff[y&1]);
    }
  }
  delete[] t;
}
