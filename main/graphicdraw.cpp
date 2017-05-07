#include "graphiclib.hpp"
#include <assert.h>

void graphiclib::screenclear(void){
  for(int i=0;i<window_height;i++){
    setbrush(4*36+6+2);
    Hline(0,i,window_width);
  }
}

void graphiclib::setPoint(vector2 v){
  pv = v;
}


void graphiclib::fillcircle(const vector2& vin,int r){
  vector2 v;
  v.x = r;
  int f = -2 * r + 3;

  while ( v.x >= v.y ) {
    Hline( vin.x - v.x, vin.y - v.y, v.x*2+1);
    Hline( vin.x - v.x, vin.y + v.y, v.x*2+1);
    Hline( vin.x - v.y, vin.y - v.x, v.y*2+1);
    Hline( vin.x - v.y, vin.y + v.x, v.y*2+1);
    if ( f >= 0 ) {
      v.x--;
      f -= 4 * v.x;
    }
    v.y++;
    f += 4 * v.y + 2;
  }
}

void graphiclib::circle(const vector2& vin,int r){
  vector2 v;
  v.x = r;
  int f = -2 * r + 3;

  while ( v.x >= v.y ) {
    putdot( vin.x + v.x, vin.y + v.y );
    putdot( vin.x - v.x, vin.y + v.y );
    putdot( vin.x + v.x, vin.y - v.y );
    putdot( vin.x - v.x, vin.y - v.y );
    putdot( vin.x + v.y, vin.y + v.x );
    putdot( vin.x - v.y, vin.y + v.x );
    putdot( vin.x + v.y, vin.y - v.x );
    putdot( vin.x - v.y, vin.y - v.x );
    if ( f >= 0 ) {
      v.x--;
      f -= 4 * v.x;
    }
    v.y++;
    f += 4 * v.y + 2;
  }
}

/* 端点分類コード */
enum Edge {
  LEFT = 1,
  RIGHT = 2,
  TOP = 4,
  BOTTOM = 8,
};

int graphiclib::calc_seq_code(vector2 c){
  int code = 0;
  if( c.x < 0 ) code += LEFT;
  if( c.x >= window_width ) code += RIGHT;
  if( c.y < 0 ) code += TOP;
  if( c.y >= window_height ) code += BOTTOM;

  return code;
}

int graphiclib::calc_intsec_x( vector2 c0, vector2 c1, int clip_x, vector2& c ){
  int cy = ( c1.y - c0.y ) * ( clip_x - c0.x ) / ( c1.x - c0.x ) + c0.y;

  /* エリア外の場合はFalseを返す */
  if ( ( cy < 0 ) || ( cy >= window_height ) ) return( 0 );

  c.x = clip_x;
  c.y = cy;

  return 1;
}

int graphiclib::calc_intsec_y( vector2 c0, vector2 c1, int clip_y, vector2& c ){
  int cx = ( c1.x - c0.x ) * ( clip_y - c0.y ) / ( c1.y - c0.y ) + c0.x;

  /* エリア外の場合はFalseを返す */
  if ( ( cx < 0 ) || ( cx >= window_width ) ) return( 0 );

  c.x = cx;
  c.y = clip_y;

  return 1;
}

int graphiclib::calc_clipped_point( int code, vector2 c0, vector2 c1, vector2& c ){
  /* ウィンドウの左端より外側にある */
  if ( ( code & LEFT ) != 0 )
    if ( calc_intsec_x( c0, c1, 0, c ) )
      return 1;

  /* ウィンドウの右端より外側にある */
  if ( ( code & RIGHT ) != 0 )
    if ( calc_intsec_x( c0, c1, window_width-1, c ) )
      return 1;

  /* ウィンドウの上端より外側にある */
  if ( ( code & TOP ) != 0)
    if ( calc_intsec_y( c0, c1, 0, c ) )
      return 1;

  /* ウィンドウの下端より外側にある */
  if ( ( code & BOTTOM ) != 0 )
    if ( calc_intsec_y( c0, c1, window_height-1, c ) )
      return 1;

  return 0;  /* クリッピングされなかった場合、線分は完全に不可視 */
}

int graphiclib::clipping( vector2& c0, vector2& c1 ){
  int code0, code1; /* 端点分類コード */

  code0 = calc_seq_code(c0);  /* 始点の端点分類コードを求める */
  code1 = calc_seq_code(c1);  /* 終点の端点分類コードを求める */

  /* 端点分類コードがどちらも0000ならばクリッピングは必要なし */
  if (( code0 == 0 ) && ( code1 == 0 )) return 0;

  /* 始点・終点の端点分類コードの論理積が非０ならば線分は完全に不可視 */
  if (( code0 & code1 ) != 0) return -1;

  /* 始点のクリッピング */
  if(code0 != 0)
    if (!calc_clipped_point( code0, c0, c1, c0 ))
      return -1;

  /* 終点のクリッピング */
  if(code1 != 0)
    if (!calc_clipped_point( code1, c0, c1, c1 ))
      return -1;

  return 1;
}

void graphiclib::Line(const fvector3 &vin1,const fvector3 &vin2){
  vector2 v1,v2;
  v1.x = vin1.x; v1.y = vin1.y;
  v2.x = vin2.x; v2.y = vin2.y;
  Line(v1,v2);
}

void graphiclib::Line(vector2 vin1,vector2 vin2){
  vector2 s,d;
  vector2 v1,v2;
  vector2 p1,p2;
  v1 = vin1;
  v2 = vin2;
  int i,e;
  
  if(clipping(v1,v2)==-1)return;
  // std::cout << v1.x << "," << v1.y << "  " << v2.x << "," << v2.y << std::endl;

  if(v1.x<v2.x){
    d.x=v2.x-v1.x;
    s.x=1;
  }
  else{
    d.x=v1.x-v2.x;
    s.x=-1;
  }
  if(v2.y>v1.y){
    d.y=v2.y-v1.y;
    s.y=1;
  }
  else{
    d.y=v1.y-v2.y;
    s.y=-1;
  }

  if(d.x>=d.y){
    e=-d.x;
    for(i=0;i<=d.x;i++){
      putdot(v1);
      v1.x+=s.x;
      e+=d.y*2;
      if(e>=0){
	v1.y+=s.y;
	e-=d.x*2;
      }
    }
  }
  else{
    e=-d.y;
    for(i=0;i<=d.y;i++){
      putdot(v1);
      v1.y+=s.y;
      e+=d.x*2;
      if(e>=0){
	v1.x+=s.x;
	e-=d.y*2;
      }
    }
  }
}
