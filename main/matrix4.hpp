/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/
#include "fvector3.hpp"
#include "fvector4.hpp"
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#ifndef __MATRIX4_HPP_
#define __MATRIX4_HPP_

class Matrix4{
public:
  float m[16];
  Matrix4(){
    for(int i=0;i<16;i++){
      m[i]=0;
    }
    for(int i=0;i<4;i++){
      m[i*4+i]=1.f;
    }
  }
  Matrix4(float m00,float m01,float m02,float m03,
	  float m10,float m11,float m12,float m13,
	  float m20,float m21,float m22,float m23,
	  float m30,float m31,float m32,float m33){
    m[0]=m00,m[1]=m01,m[2]=m02,m[3]=m03;
    m[4]=m10,m[5]=m11,m[6]=m12,m[7]=m13;
    m[8]=m20,m[9]=m21,m[10]=m22,m[11]=m23;
    m[12]=m30,m[13]=m31,m[14]=m32,m[15]=m33;

  }

  void set(int n,float a){
    m[n] = a;
  }

  float operator[](unsigned int n)const{
    return m[n];
  }
  
  void print();
  Matrix4& rotate_x(uint16_t angle){
    /*とりあえず。。。*/
    int sint,cost;
    sint = sin(((double)angle)*2*3.14159265359/65536)*65536;
    cost = cos(((double)angle)*2*3.14159265359/65536)*65536;

    int   m1 = m[1],  m2 = m[2],
      m5 = m[5],  m6 = m[6],
      m9 = m[9],  m10= m[10],
      m13= m[13], m14= m[14];
    
    m[1] = m1 * cost/65536 + m2 *-sint/65536;
    m[2] = m1 * sint/65536 + m2 * cost/65536;
    m[5] = m5 * cost/65536 + m6 *-sint/65536;
    m[6] = m5 * sint/65536 + m6 * cost/65536;
    m[9] = m9 * cost/65536 + m10*-sint/65536;
    m[10]= m9 * sint/65536 + m10* cost/65536;
    m[13]= m13* cost/65536 + m14*-sint/65536;
    m[14]= m13* sint/65536 + m14* cost/65536;

    return *this;
  }

  Matrix4& rotate_y(uint16_t angle) {
    int sint,cost;
    sint = sin(((double)angle)*2*3.14159265359/65536)*65536;
    cost = cos(((double)angle)*2*3.14159265359/65536)*65536;

    int   m0 = m[0],  m2 = m[2],
      m4 = m[4],  m6 = m[6],
      m8 = m[8],  m10= m[10],
      m12= m[12], m14= m[14];

    m[0] = m0 * cost/65536 + m2 * sint/65536;
    m[2] = m0 *-sint/65536 + m2 * cost/65536;
    m[4] = m4 * cost/65536 + m6 * sint/65536;
    m[6] = m4 *-sint/65536 + m6 * cost/65536;
    m[8] = m8 * cost/65536 + m10* sint/65536;
    m[10]= m8 *-sint/65536 + m10* cost/65536;
    m[12]= m12* cost/65536 + m14* sint/65536;
    m[14]= m12*-sint/65536 + m14* cost/65536;

    return *this;
  }

  Matrix4& rotate_z(uint16_t angle){
    int sint,cost;
    sint = sin(((double)angle)*2*3.14159265359/65536)*65536;
    cost = cos(((double)angle)*2*3.14159265359/65536)*65536;
    int   m0 = m[0],  m1 = m[1],
      m4 = m[4],  m5 = m[5],
      m8 = m[8],  m9 = m[9],
      m12= m[12], m13= m[13];
  
    m[0] = m0 * cost/65536 + m1 *-sint/65536;
    m[1] = m0 * sint/65536 + m1 * cost/65536;
    m[4] = m4 * cost/65536 + m5 *-sint/65536;
    m[5] = m4 * sint/65536 + m5 * cost/65536;
    m[8] = m8 * cost/65536 + m9 *-sint/65536;
    m[9] = m8 * sint/65536 + m9 * cost/65536;
    m[12]= m12* cost/65536 + m13*-sint/65536;
    m[13]= m12* sint/65536 + m13* cost/65536;

    return *this;
  }
  
  Matrix4& scale(float s){
    return scale(s, s, s);
  }

  Matrix4& scale(float x, float y, float z){
    m[0] = m[0]*x;   m[4] = m[4]*x;   m[8]  = m[8]  *x;  m[12] = m[12] * x;
    m[1] = m[1]*y;   m[5] = m[5]*y;   m[9]  = m[9]  *y;  m[13] = m[13] * y;
    m[2] = m[2]*z;   m[6] = m[6]*z;   m[10] = m[10] *z;  m[14] = m[14] * z;
    return *this;
  }

  fvector3 mul_fv3(fvector3 v){
    float rw;
    fvector3 av;
    av.x = (v.x)*m[0]+(m[4])*v.y+(v.z)*m[8]+m[12];
    av.y = (v.x)*m[1]+(m[5])*v.y+(v.z)*m[9]+m[13];
    av.z = (v.x)*m[2]+(m[6])*v.y+(v.z)*m[10]+m[14];
    rw  = 1.f/(v.x*m[3]+m[7]*v.y+v.z*m[11]+m[15]);
    // printf("wwwww%d\n",w);

    av.x=av.x*rw;
    av.y=av.y*rw;
    av.z=av.z*rw;

    return av;
  }


  Matrix4 transpose_rotation(){
    return Matrix4(
		   m[0],m[4],m[8],0,
		   m[1],m[5],m[9],0,
		   m[2],m[6],m[10],0,
		   0,0,0,1);
  }
  
  fvector4 mul_fv4(fvector3 v){
    float rw;
    fvector4 av;
    av.x = (v.x)*m[0]+(m[4])*v.y+(v.z)*m[8]+m[12];
    av.y = (v.x)*m[1]+(m[5])*v.y+(v.z)*m[9]+m[13];
    av.z = (v.x)*m[2]+(m[6])*v.y+(v.z)*m[10]+m[14];
    av.w = (v.x*m[3]+m[7]*v.y+v.z*m[11]+m[15]);
    rw  = 1.f/av.w;
    // printf("wwwww%d\n",w);

    av.x=av.x*rw;
    av.y=av.y*rw;
    av.z=av.z*rw;

    return av;
  }
  fvector3 reversetranslation(void){
    return fvector3(m[12],m[13],m[14]);
  }
};

Matrix4 rotatex(uint16_t ang);
Matrix4 rotatey(uint16_t ang);
Matrix4 rotatez(uint16_t ang);
Matrix4 translation(fvector3 v);

void print(Matrix4 );
static inline
Matrix4 operator*(const Matrix4& m,const Matrix4& n) {
  Matrix4 r;

  r= Matrix4(m[0]*n[0] + m[4]*n[1] + m[8]*n[2]  + m[12]*n[3],
	     m[1]*n[0] + m[5]*n[1] + m[9]*n[2]  + m[13]*n[3],
	     m[2]*n[0] + m[6]*n[1] + m[10]*n[2] + m[14]*n[3],
	     m[3]*n[0] + m[7]*n[1] + m[11]*n[2] + m[15]*n[3],
	     
	     m[0]*n[4] + m[4]*n[5] + m[8]*n[6]  + m[12]*n[7],
	     m[1]*n[4] + m[5]*n[5] + m[9]*n[6]  + m[13]*n[7],
	     m[2]*n[4] + m[6]*n[5] + m[10]*n[6] + m[14]*n[7],
	     m[3]*n[4] + m[7]*n[5] + m[11]*n[6] + m[15]*n[7],
		 
	     m[0]*n[8] + m[4]*n[9] + m[8]*n[10] + m[12]*n[11],
	     m[1]*n[8] + m[5]*n[9] + m[9]*n[10] + m[13]*n[11],
	     m[2]*n[8] + m[6]*n[9] + m[10]*n[10]+ m[14]*n[11],
	     m[3]*n[8] + m[7]*n[9] + m[11]*n[10]+ m[15]*n[11],

	     m[0]*n[12]+ m[4]*n[13]+ m[8]*n[14] + m[12]*n[15],
	     m[1]*n[12]+ m[5]*n[13]+ m[9]*n[14] + m[13]*n[15],
	     m[2]*n[12]+ m[6]*n[13]+ m[10]*n[14]+ m[14]*n[15],
	     m[3]*n[12]+ m[7]*n[13]+ m[11]*n[14]+ m[15]*n[15]);

  return r;
}

static inline
Matrix4 operator+(const Matrix4& m,const Matrix4& n) {
  Matrix4 r;

  r= Matrix4(m[0]+n[0],m[1]+n[1],m[2]+n[2],m[3]+n[3],
	     m[4]+n[4],m[5]+n[5],m[6]+n[6],m[7]+n[7],
	     m[8]+n[8],m[9]+n[9],m[10]+n[10],m[11]+n[11],
	     m[12]+n[12],m[13]+n[13],m[14]+n[14],m[15]+n[15]
	     );

  return r;
}

static inline
Matrix4 operator*(const Matrix4& m,const float n) {
  Matrix4 r;

  r= Matrix4(m[0]*n,m[1]*n,m[2]*n,m[3]*n,
	     m[4]*n,m[5]*n,m[6]*n,m[7]*n,
	     m[8]*n,m[9]*n,m[10]*n,m[11]*n,
	     m[12]*n,m[13]*n,m[14]*n,m[15]*n
	     );

  return r;
}

Matrix4 rotation_axis_and_angle(fvector3 axis,float angle);
Matrix4 rotation_axis_and_cosv(fvector3 u,float cosv);

Matrix4 loadPerspective(float fovy, float aspect,float zNear, float zFar,int width,int height);
Matrix4 magnify(float n);
Matrix4 lookat(fvector3 eye,fvector3 goal);
Matrix4 magnify_y(int n);

#endif
