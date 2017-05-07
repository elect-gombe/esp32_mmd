/*
 * graphic library using SDL2
 *
 */

#include <stdint.h>
#include "vector2.hpp"
#include "fvector3.hpp"
#include <assert.h>

#undef assert
#define assert(x) (void)(x)

#ifndef __GRAPHICLIB_H
#define __GRAPHICLIB_H

extern int frame;
const static int window_width = 160;
const static int window_height = 128;

extern const int window_width;
extern const int window_height;

template <typename T>
T min(T v1 , T v2) {
  if (v1 < v2) return v1;
  else return v2;
}

template <typename T>
T abs(T v) {
  if (v > 0) return v;
  else return -v;
}

template <typename T>
T max(T v1 , T v2) {
  if (v1 > v2) return v1;
  else return v2;
}

typedef uint8_t color_t;

class graphiclib {
private:
  vector2 pv;
  color_t strokecolor;
  color_t fillcolor;
  const uint8_t *fontp;
  unsigned int fontexpa;
  unsigned int fontsize;
  inline uint8_t get_fontwidth();
  inline uint8_t get_fontheight();
  const uint8_t *get_font(char code);
  int calc_seq_code(vector2 c);
  int calc_intsec_x( vector2 c0, vector2 c1, int clip_x, vector2& c );
  int calc_intsec_y( vector2 c0, vector2 c1, int clip_y, vector2& c );
  int calc_clipped_point( int code, vector2 c0, vector2 c1, vector2& c );
public:
  void setbrush(color_t col){strokecolor = col;}
  void setfill(color_t col){fillcolor = col;}
  void Line(const vector2& v){Line(v,pv);pv=v;}
  void Line(vector2 v1,vector2 v2);
  void Line(const fvector3 &vin1,const fvector3 &vin2);
  inline void putdot(const vector2& v);
  inline void putdot(int x,int y);
  inline void putsubdot(const vector2& v);
  inline void putsubdot(int x,int y);
  void circle(const vector2& vin,int r);
  void fillcircle(const vector2& vin,int r);
  inline void Hline(int x,int y,int w);
  inline void Hlinefill(int x,int y,int w);
  int clipping( vector2& c0, vector2& c1 );
  
  int textSize(int size);
  void drawFont(const vector2 &v,char m);
  void print_string(const char *str);
  void setPoint(vector2 v);
  void screenclear(void);
};


inline void graphiclib::Hline(int x,int y,int w){
  assert(y >= 0 && y < window_height);
  assert(x >= 0 && x+w <= window_width);
  
  for(int i=0;i<w;i++){
    #warning todo
    // vram[y*window_width+x+i] = strokecolor;
  }
}

inline void graphiclib::Hlinefill(int x,int y,int w){
  assert(0 >= y && y < window_height);
  assert(0 >= x && x+w < window_width);
  for(int i=0;i<w;i++){
    // vram[y*window_width+x+i] = fillcolor;
  }
}

inline void graphiclib::putsubdot(int x,int y){
  assert(x >= 0&&x < window_width);
  assert(y >= 0&&y < window_height);
  if(fillcolor!=255){
    #warning todo
    //    vram[y*window_width+x] = fillcolor;
  }
}

inline void graphiclib::putsubdot(const vector2& v){
  assert(v.x >= 0&&v.x < window_width);
  assert(v.y >= 0&&v.y < window_height);
  if(fillcolor!=255){
    //vram[v.y*window_width+v.x] = fillcolor;
  }
}

inline void graphiclib::putdot(int x,int y){
  assert(x >= 0&&x < window_width);
  assert(y >= 0&&y < window_height);
  //vram[y*window_width+x] = strokecolor;
}

inline void graphiclib::putdot(const vector2& v){
  assert(v.x >= 0&&v.x < window_width);
  assert(v.y >= 0&&v.y < window_height);
  //  vram[v.y*window_width+v.x] = strokecolor;
}
int isKeyPressed();
int getkey();

int isMousePressed();
int isMouseReleased();
void getWheel(int &y);
#endif
