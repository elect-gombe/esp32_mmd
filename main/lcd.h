#ifndef __LCD_H

#ifdef __cplusplus
extern "C"{
#endif
#include <stdint.h>  
void LCD_WriteIndex(unsigned char index);
void LCD_WriteData(unsigned short data);
void LCD_Init(void);
void LCD_SetCursor(unsigned short x, unsigned short y,unsigned short x1, unsigned short y1);
void LCD_Clear(unsigned short color);
void drawPixel(unsigned short x, unsigned short y, unsigned short color);
void transferLineViaDMA(unsigned short *buf);
void LCD_out(int x,int y,uint16_t *b,int n);
#endif
  
#ifdef __cplusplus
}
#endif
  
