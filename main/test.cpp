#include "lcd.h"

void main3d();
extern "C"{
  int appmain(void);
  void toggleLED();
  void transferSPI2(int data);
}

class hoge{
private:
  int i;
public:
  hoge(){
    i = 10;
  }
  int hello();
  int hogeworld();
};

int appmain(void){
  uint16_t ld[2][160];
  int f=0;
  LCD_Init();
  LCD_Init();
  while(1){
    for(int i=0;i<160;i++){
      ld[f&1][i] = i+f;
    }
    LCD_out(0,f%128,ld[f&1],160);
    main3d();
    f++;
  }
}
