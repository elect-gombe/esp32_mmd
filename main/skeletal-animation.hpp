#include "matrix4.hpp"
#include "fvector3.hpp"
#include "quaternion.hpp"
#include <iostream>
#include "motion.h"

#ifdef VISUALDEBUG
#include "vdb.h"
#endif

struct  bone_t{
  int16_t parent;
  fvector3_t pos;
};

struct mixedbone_t{
  uint8_t weight;
  uint16_t bone[2];
};

struct ik_t{
  uint16_t rootid;
  uint16_t targetid;
  uint16_t len;
  const uint16_t data[5];
  uint8_t looptimes;
  float anglelimit;
};

#include <sys/time.h>

/**
 * Returns the current time in microseconds.
 */
static
int64_t getMicrotime(){
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

extern const
mixedbone_t mixedbone[];

extern const
ik_t iks[7];

//this value includes all bone and mixed transformation matrix.
#define MAX_MATRIX 160

#define MAX_BONE_NUM 92

#define ROOTID -1

#define LINE(X,Y) vdb_line(X.x,X.y,-X.z,Y.x,Y.y,-Y.z)

#define POINT(X) vdb_point(X.x,X.y,-X.z)

//#define SURPORT_MIXED_TRANSFORM
const
int hiza[]={
  37,66,//膝のボーン
};

// ref https://veeenu.github.io/2014/05/09/implementing-skeletal-animation.html
int ft;
class bone{
public:
  Matrix4 transform;
  Matrix4 init;
  Matrix4 ofs;
  bone *parent=0;
  fvector3 basepos;
  Matrix4 modellocal;

  bone(){}
  
  void settransform(fvector3 t1,quaternion q1){
    //この時点ではlocal
    transform = translation(t1)*q1.toMatrix4();
  }

  void initbone(const bone* p,const bone_t *pbt,const bone_t *base){
    parent = (bone*)p;
    fvector3 pdiff = fvector3(base->pos);
    basepos = fvector3(base->pos);

    if(p){
      pdiff = fvector3(base->pos)-fvector3(pbt->pos);
      init = translation(pdiff);
    }
    else
      init = translation(pdiff);
    ofs = translation(-fvector3(base->pos));
    settransform(fvector3(),quaternion());
  }
  
  void calc_bonemat(){
    if(parent){
      //Globalまであとofsだけ
      modellocal = parent->modellocal*init*transform;
      // transform.print();
    }else{
      modellocal = init*transform;
    }
    // transform.print();
  }
};


static int t;
static int64_t begin=-1;
static uint64_t ptime;

class bonelist{
public:
  uint16_t num;
  uint16_t nummixed;
  bone listbone[MAX_BONE_NUM];
  Matrix4 boneworld[MAX_MATRIX];
  const motion_t *mp;

  bonelist(){}

  fvector3 pos[2][300];
  quaternion q[2][300];
  int frametime[2][300]={};

  void init(const bone_t* bones,int n,int mn){
    int i;
    for(i=0;i<300;i++){
      frametime[0][i]=-1;
    }
    num = n;
    nummixed = mn;
    // listbone[num-2].initbone(NULL,NULL,&bones[num-2]);
    for(int i=0;i<num;i++){
      if(bones[i].parent!=-1)
	listbone[i].initbone(&listbone[bones[i].parent],&bones[bones[i].parent],&bones[i]);
      else{
	listbone[i].initbone(NULL,NULL,&bones[i]);
      }
    }
    mp = motion;
  }

  void setpose(float t){
    fvector3 p;
    quaternion r;
    while(1){
      if(mp->boneid == -1)goto cont;
      if(mp->frame == -1)break;
      if(mp->boneid >= num){
	break;
      }
      if(frametime[0][mp->boneid]<=t){
	frametime[1][mp->boneid] = frametime[0][mp->boneid];
	frametime[0][mp->boneid] = mp->frame;
	pos[1][mp->boneid] = pos[0][mp->boneid];
	pos[0][mp->boneid] = fvector3(mp->pos);
	q[1][mp->boneid] = q[0][mp->boneid];
	q[0][mp->boneid] = quaternion(mp->rotation);
      }else{
	// printf("%f\n",frametime[1][mp->boneid]-t);
	break;
      }
    cont:
      mp++;
    }
    for(int i=0;i<num;i++){
      float ratio;
      if(frametime[1][i]-frametime[0][i]&&frametime[1][mp->boneid] != -1&&frametime[0][mp->boneid] != -1&&frametime[0][i]>t){
	ratio = ((float)t-frametime[1][i])/(frametime[0][i]-frametime[1][i]);
	// if(i==50){
	  // printf("%f\n",ratio);
	// }
	r = slerpQuaternion(q[1][i],q[0][i],ratio);
	p = pos[1][i]*(1-ratio)+pos[0][i]*ratio;
      }else{
	r = q[0][i];
	p = pos[0][i];
      }
      listbone[i].settransform(p,r);
    }
  }

  // quaternion_t qt={0.f,0.02,0,0.9996f};
  quaternion_t qt={0,0,0,1};
  
  void calcall(Matrix4 root){
    if(begin==-1){
      begin = getMicrotime();
      ptime = begin-1;
    }
    // for(int i=0;i<num;i++){
    //   listbone[i].settransform(fvector3(),quaternion(qt));
    // }

    setpose((getMicrotime()-begin)/1000000.f*30.f);
    printf("%.1f\n",1000000.f/(getMicrotime()-ptime));
    ptime = getMicrotime();

    for(int i=0;i<num;i++){
      listbone[i].calc_bonemat();
      // boneworld[i] = root;
    }


    updateiks();
#ifdef VISUALDEBUG
    for(int i=0;i<num;i++){
      fvector3 linkpos,linkpos2;
      if(listbone[i].parent){
	linkpos = (listbone[i].modellocal*listbone[i].ofs).mul_fv3(listbone[i].basepos);
	if(listbone[i].parent->parent){
	  linkpos2 = (listbone[i].parent->modellocal*listbone[i].parent->ofs).mul_fv3(listbone[i].parent->basepos);
	}
	vdb_line(linkpos.x,linkpos.y,-linkpos.z,linkpos2.x,linkpos2.y,-linkpos2.z);
      }
    }
#endif
    
    for(int i=0;i<num;i++){
      if(listbone[i].parent){
	boneworld[i] = root*listbone[i].modellocal*listbone[i].ofs;
      }
      // std::cout << "ofs" << std::endl;
      // listbone[i].ofs.print();
      // std::cout << "init" << std::endl;
      // listbone[i].init.print();
      // std::cout<<i<<std::endl;
      //      (listbone[i].ofs*listbone[i].transform).print();
    }
    for(int i=num;i<num+nummixed;i++){
      boneworld[i] = (boneworld[mixedbone[i-num].bone[0]]*(1.f-(mixedbone[i-num].weight*0.01f))+boneworld[mixedbone[i-num].bone[1]]*(mixedbone[i-num].weight*0.01f));
    }
  }

  void update1ik(const ik_t *ik){
    fvector3 linkpos;
    fvector3 effectpos;
    fvector3 targetpos;
    fvector3 localeffectpos;
    fvector3 localtargetpos;
    fvector3 axis;
    float cosval;
#define REDUCE_IKLOOP
#ifdef REDUCE_IKLOOP
    int loopn = ik->looptimes*2/3;
    if(loopn > 15)loopn = 15;
#else
    int loopn = ik->looptimes;
#endif
    for(int n=0;n<loopn;n++){
      effectpos = (listbone[ik->rootid].modellocal).reversetranslation();
      for(int j=0;j<ik->len;j++){
	int i = ik->data[j];
	targetpos = (listbone[ik->targetid].modellocal).reversetranslation();
	linkpos = (listbone[i].modellocal).reversetranslation();
	localeffectpos = (listbone[i].modellocal).transpose_rotation().mul_fv3(effectpos - linkpos);
	localtargetpos = (listbone[i].modellocal).transpose_rotation().mul_fv3(targetpos - linkpos);
	if(hiza[0]==i||hiza[1]==i){
	  localeffectpos.x = 0;
	  localtargetpos.x = 0;
	  // vdb_color(1,0,1);
	  // POINT(linkpos);
	}
	localeffectpos.normalize();
	localtargetpos.normalize();
#ifdef VISUALDEBUG
	if(ik==&iks[3]||ik==&iks[4]){
	  vdb_color(0,1,0);
	  LINE(effectpos,linkpos);
	  vdb_color(0,1,1);
	  LINE(targetpos,linkpos);
	  // vdb_color(n/5.+0.1,n/5.+0.1,n/5.+0.1);
	  // LINE(localeffectpos,localtargetpos);
	  vdb_color(1,0,0);
	}
#endif

	cosval = localeffectpos * localtargetpos;
	// printf("%f->%2.1f\n",cosval,acos(cosval)/3.14159265358979324f/2*360);
	if(cosval < cos(ik->anglelimit))cosval = cos(ik->anglelimit);
	if(cosval < 0.999999f){
	  axis = cross(localtargetpos,localeffectpos);
	  axis.normalize();
	  if(hiza[0]==i||hiza[1]==i){
	    if((listbone[i].transform*rotation_axis_and_cosv(-axis,cosval)).transpose_rotation()[6]<0){
	      axis = -axis;
	    }
	  }
	  	  
	  // LINE((axis+linkpos),linkpos);
	  // LINE((axis+localeffectpos),localeffectpos);
	  listbone[i].transform = listbone[i].transform*rotation_axis_and_cosv(-axis,cosval);
	  for(int q=j;q!=-1;q--){
	    int i=ik->data[q];
	    listbone[i].calc_bonemat();
	  }
	  listbone[ik->targetid].calc_bonemat();
	}
      }
      //      for(int j=0;j<ik->len;j++){
    }

#ifdef VISUALDEBUG
    vdb_color(1,0,0);
    LINE(fvector3(),fvector3(1,0,0));
    vdb_color(0,1,0);
    LINE(fvector3(),fvector3(0,1,0));
    vdb_color(0,0,1);
    LINE(fvector3(),fvector3(0,0,1));
    
    vdb_color(1,1,1);
    POINT(fvector3());
#endif
  }

  void updateiks(void){
    for(int i=0;i<// 1||false&&
	  (int)(sizeof(iks)/sizeof(iks[0]));i++){
	update1ik(&iks[i]);
      }
    }
};
