#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "3dconfig.hpp"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif
void* vTask(void* prm);
int main3d(void);
#ifdef __cplusplus
};
#endif

#ifdef OUTPUTTERMINAL
void send_line(int ypos,uint8_t *data){
  int r,g,b;
  size_t id = 0;
  uint8_t pv[2]={1,100};
  char tmp[64];
  char *buff;
  buff = (char*)calloc(1024*1024,1);
  for(int dy=0;dy<DRAW_NLINES;dy++){
    sprintf(tmp,"\033[%d;1H",ypos+dy);strcat(buff,tmp);
    for(int i=0;i<window_width;i++){
      if(pv[0] == data[id+0]&&pv[1]==data[id+1]){
	sprintf(tmp,"  ");strcat(buff,tmp);
	id+=2;
      }else{
	r = data[id+0]>>3;
	g = ((data[id+0]&0x7)<<3)|(data[id+1]>>5);
	b = data[id+1]&0x1F;
	pv[0] = data[id+0];
	pv[1] = data[id+1];
	id+=2;
	sprintf(tmp,"\033[48;2;%d;%d;%dm  ",r*8,g*4,b*8);strcat(buff,tmp);
      }
    }
  }
  printf("%s",buff);
  fflush(stdout);
  free(buff);  
}
#endif

int psnum[PROCESSNUM];
int main(void){
#ifdef PTHREAD
  pthread_t pthread;
  static int i;
  for(i=0;i<PROCESSNUM-1;i++){
    psnum[i]=i;
    pthread_create( &pthread, NULL, &vTask, &psnum[i]);
  }
#endif
  main3d();
}
