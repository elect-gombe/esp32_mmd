#include "matrix4.hpp"
#include "vector3.hpp"

Matrix4 loadPerspective(float fovy,float aspect,float zNear,float zFar,int width,int height){
  Matrix4 m;
  const float dz(zFar - zNear);
  (void)width;
  (void)height;

  if (dz != 0.0f) {
    m.set( 5 , 1.f / tanf(fovy*3.1415926535898f));
    m.set( 0, m[5]*aspect );
    m.set(10, -(zFar + zNear) / dz);
    m.set(11, -1.f);
    m.set(14, -2.f * zFar * zNear / dz);
  }
  return m;
}

Matrix4 lookat(fvector3 goal,fvector3 eye){
  Matrix4 m,rm;

  fvector3 t,r,s;
  t=eye-goal;
  r=cross(fvector3(0.f,-1.f,0.f),t);
  s=cross(t,r);

  float rrs,rss,rts;
  rrs = 1.f/r.abs();
  rss = 1.f/s.abs();
  rts = 1.f/t.abs();

  m.set(0, r.x*rrs);
  m.set(4, r.y*rrs);
  m.set(8, r.z*rrs);

  m.set(1, s.x*rss);
  m.set(5, s.y*rss);
  m.set(9, s.z*rss);

  m.set(2, t.x*rts);
  m.set(6, t.y*rts);
  m.set(10, t.z*rts);
  // print(m);

  return m*translation(-eye);
}

// void print(Matrix4 m){
//   for(int i=0;i<16;i++){
//     std::cout <<std::fixed<< std::setprecision(5)<<m[i]<< ",";
//     if(i%4==3)std::cout<<std::endl;
//   }
//   std::cout<<std::endl;
// }

float sin16(uint16_t ang){
  return sinf(ang*(2.f*3.14159265359f/65536.f));
}

float cos16(uint16_t ang){
  return sin16(ang + 65536/4);
}

float tan16(uint16_t ang){
  return sin16(ang) / cos16(ang);;
}

Matrix4 rotatex(uint16_t ang){
  Matrix4 r;
  float sint,cost;
  sint = sin16(ang);
  cost = cos16(ang);

  return Matrix4(1.f,   0,      0, 0,
		 0,  cost, -sint , 0,
		 0,  sint, cost  , 0,
		 0,     0,      0, 1.f);
}

Matrix4 rotatey(uint16_t ang){
  Matrix4 r;
  float sint,cost;
  sint = sin16(ang);
  cost = cos16(ang);

  return Matrix4(   cost, 0    , sint , 0,
		    0   , 1.f  , 0    , 0,
		   -sint, 0    , cost , 0,
		    0   , 0    , 0    , 1.f);
}

Matrix4 rotatez(uint16_t ang){
  float sint,cost;
  sint = sin16(ang);
  cost = cos16(ang);
  return Matrix4(cost , -sint , 0    , 0,
		 sint , cost  , 0    , 0,
		 0    , 0     , 1.f  , 0,
		 0    , 0     , 0    , 1.f);
}

Matrix4 translation(fvector3 v){
  return Matrix4(1.f,0,0,0,
		 0,1.f,0,0,
		 0,0,1.f,0,
		 v.x,v.y,v.z,1.f);
}

Matrix4 magnify(float n){
  return Matrix4(1.f*n,0,0,0,
		 0,1.f*n,0,0,
		 0,0,1.f*n,0,
		 0,0,0,1.f);
}

Matrix4 magnify_y(float n){
  return Matrix4(1.f,0,0,0,
		 0,n,0,0,
		 0,0,1.f,0,
		 0,0,0,1.f);
}

