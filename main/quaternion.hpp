/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

// 参考
// http://marupeke296.com/DXG_No58_RotQuaternionTrans.html
#ifndef _QUATERNION_HPP
#define _QUATERNION_HPP

struct quaternion_t{
  float qx,qy,qz,qw;
};

class quaternion{
public:
  float qx,qy,qz,qw;

  quaternion(){
    qx=0.f;
    qy=0.f;
    qz=0.f;
    qw=1.f;
  }

  quaternion(quaternion_t q){
    qx=q.qx;
    qy=q.qy;
    qz=q.qz;
    qw=q.qw;
  }
  
  Matrix4 toMatrix4() {
    return Matrix4(1.0f - 2.0f * qy * qy - 2.0f * qz * qz, 2.0f * qx * qy + 2.0f * qw * qz,  2.0f * qx * qz - 2.0f * qw * qy , 0.f, 
		   2.0f * qx * qy - 2.0f * qw * qz,  1.0f - 2.0f * qx * qx - 2.0f * qz * qz,  2.0f * qy * qz + 2.0f * qw * qx, 0.f, 
		   2.0f * qx * qz + 2.0f * qw * qy,  2.0f * qy * qz - 2.0f * qw * qx, 1.0f - 2.0f * qx * qx - 2.0f * qy * qy , 0.f,
		   0.f,0.f,0.f,1.f);
  }
  float getLen(){
    return sqrt( qx * qx + qy * qy + qz * qz + qw * qw );
  }
};

// クォータニオン球面線形補間
quaternion slerpQuaternion(
    quaternion q1,
    quaternion q2,
    float t
) {
  quaternion out;

  float cos_w = (q1.qx * q2.qx + q1.qy * q2.qy + q1.qz * q2.qz + q1.qw * q2.qw);
  if(cos_w < 0.f){
    q2.qx = -q2.qx;
    q2.qy = -q2.qy;
    q2.qz = -q2.qz;
    q2.qw = -q2.qw;
    cos_w = -cos_w;
  }

  float mult_q1;
  float mult_q2;

  if(cos_w > 0.999f){
    mult_q1 = 1.f-t;
    mult_q2 = t;
  }else{
    const float sin_w = sqrtf(1.f-cos_w*cos_w);
    const float omega = atan2(sin_w,cos_w);

    // 球面線形補間
    const float one_over_sin_w = 1.f / sin_w;
    mult_q1 = ((1.f-t)*omega)*one_over_sin_w;
    mult_q2 = (t*omega)*one_over_sin_w;
  }
    
  out.qx = mult_q1 * q1.qx + mult_q2 * q2.qx;
  out.qy = mult_q1 * q1.qy + mult_q2 * q2.qy;
  out.qz = mult_q1 * q1.qz + mult_q2 * q2.qz;
  out.qw = mult_q1 * q1.qw + mult_q2 * q2.qw;
    
  return out;
}

#endif
