#include "main.h"
#include "stm32f4xx_hal.h"

volatile int trans_flag;

#define RS_LOW()    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET)
#define RS_HIGH()   HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET)
#define RST_LOW()   HAL_GPIO_WritePin(RST_GPIO_Port,RST_Pin,GPIO_PIN_RESET)
#define RST_HIGH()  HAL_GPIO_WritePin(RST_GPIO_Port,RST_Pin,GPIO_PIN_SET)
#define CS_LOW()    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH()   HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)

const int width = 160;
const int height = 128;

void delay_ms(int ms){
  HAL_Delay(ms);
}

void LCD_set_dat(uint8_t d){
  HAL_SPI_Transmit(&hspi2, &d, 1, 10);
}

void LCD_WriteIndex(unsigned char i)
{
  // Write Index
  CS_LOW();
  RS_LOW();
  LCD_set_dat(i);
  CS_HIGH();
}

void LCD_WriteData(unsigned short i)
{
  // Write Data
  CS_LOW();
  RS_HIGH();
  LCD_set_dat(i);
  CS_HIGH();
}

void LCD_WriteReg(unsigned char index, unsigned short data)
{
  // Write Index
  LCD_WriteIndex(index);
  // Write Data
  LCD_WriteData(data);
}

void LCD_Init()
{
  GPIO_InitTypeDef a={
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_HIGH,
  };
  a.Pin = RS_Pin;
  HAL_GPIO_Init(RS_GPIO_Port, &a);
  a.Pin = RST_Pin;
  HAL_GPIO_Init(RST_GPIO_Port, &a);
  a.Pin = CS_Pin;
  HAL_GPIO_Init(CS_GPIO_Port, &a);
  delay_ms(10);
  
  //CS_HIGH();//LCD_CS_HI;                  // Disable LCD

  RST_HIGH(); // SCL
  delay_ms(2);
  RST_LOW();
  delay_ms(50);
  RST_HIGH();
  delay_ms(2);

  CS_LOW();

  LCD_WriteIndex(0x11); //Exit Sleep
  delay_ms(2);

  LCD_WriteIndex(0x26); //Set Default Gamma
  LCD_WriteData(0x04);

  LCD_WriteIndex(0xB1);//Set Frame Rate
  LCD_WriteData(0x0C);
  LCD_WriteData(0x14);

  LCD_WriteIndex(0xC0); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
  LCD_WriteData(0x0C);
  LCD_WriteData(0x05);

  LCD_WriteIndex(0xC1); //Set BT[2:0] for AVDD & VCL & VGH & VGL
  LCD_WriteData(0x02);//0x00

  LCD_WriteIndex(0xC5); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
  LCD_WriteData(0x29);  //31  21  29
  LCD_WriteData(0x43);  //58   48  43

  LCD_WriteIndex(0xC7);
  LCD_WriteData(0x40);

  LCD_WriteIndex(0x3a); //Set Color Format
  LCD_WriteData(0x05);
  /*
    LCD_WriteIndex(0x2A); //Set Column Address
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x7F);	

    LCD_WriteIndex(0x2B); //Set Page Address
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x9F);	
  */
  LCD_WriteIndex(0x36); //Set Scanning Direction
  LCD_WriteData(0xA8); //0xA8
  LCD_WriteIndex(0xB7); //Set Source Output Direction
  LCD_WriteData(0x00);

  LCD_WriteIndex(0xF2); //Enable Gamma bit
  LCD_WriteData(0x01);	
  /*
    LCD_WriteIndex(0xE0);
    LCD_WriteData(0x36);//p1
    LCD_WriteData(0x29);//p2
    LCD_WriteData(0x12);//p3
    LCD_WriteData(0x22);//p4
    LCD_WriteData(0x1C);//p5
    LCD_WriteData(0x15);//p6
    LCD_WriteData(0x42);//p7
    LCD_WriteData(0xB7);//p8
    LCD_WriteData(0x2F);//p9
    LCD_WriteData(0x13);//p10
    LCD_WriteData(0x12);//p11
    LCD_WriteData(0x0A);//p12
    LCD_WriteData(0x11);//p13
    LCD_WriteData(0x0B);//p14
    LCD_WriteData(0x06);//p15

    LCD_WriteIndex(0xE1);
    LCD_WriteData(0x09);//p1
    LCD_WriteData(0x16);//p2
    LCD_WriteData(0x2D);//p3
    LCD_WriteData(0x0D);//p4
    LCD_WriteData(0x13);//p5
    LCD_WriteData(0x15);//p6
    LCD_WriteData(0x40);//p7
    LCD_WriteData(0x48);//p8
    LCD_WriteData(0x53);//p9
    LCD_WriteData(0x0C);//p10
    LCD_WriteData(0x1D);//p11
    LCD_WriteData(0x25);//p12
    LCD_WriteData(0x2E);//p13
    LCD_WriteData(0x34);//p14
    LCD_WriteData(0x39);//p15
  */
  LCD_WriteIndex(0x29); // Display On
  LCD_WriteIndex(0x2c);
  //CS0 = 1;
    
}

void LCD_SetCursor(unsigned short x, unsigned short y,unsigned short x1, unsigned short y1)
{

  LCD_WriteIndex(0x2A);  // column addr set
  LCD_WriteData(0x00);
  LCD_WriteData(x+1);   // XSTART 
  LCD_WriteData(0x00);
  LCD_WriteData(x1+1);   // XEND

  LCD_WriteIndex(0x2B);  // row addr set
  LCD_WriteData(0x00);
  LCD_WriteData(y+2);    // YSTART
  LCD_WriteData(0x00);
  LCD_WriteData(y1+2);    // YEND

  LCD_WriteIndex(0x2C);  // write to RAM

}

void LCD_out(int x,int y,uint16_t *b,int n){
  while(trans_flag)    toggleLED();
  trans_flag = 1;
  LCD_SetCursor(x,y,160-1,128-1);
  CS_LOW();
  RS_HIGH();//LCD_RS_HI;
  HAL_SPI_Transmit_DMA(&hspi2, (void*)b, n*2);
}

void LCD_continuous_output(unsigned short x,unsigned short y,unsigned short color,int n)
{
  //High speed continuous output
  int i;
  unsigned char d1,d2;
  LCD_SetCursor(x,y,160-1,128-1);
  //LCD_WriteIndex(0x2C);
  CS_LOW();
  RS_HIGH();//LCD_RS_HI;
  d1=color>>8;
  d2=(unsigned char)color;
  for (i=0; i < n ; i++){
    LCD_set_dat(d1);
    LCD_set_dat(d2);
  }
  CS_HIGH();
}

void LCD_Clear(unsigned short color)
{
  LCD_continuous_output(0,0,color,width*height);
}

void drawPixel(unsigned short x, unsigned short y, unsigned short color)
{
  LCD_SetCursor(x,y,x+1,y+1);
  //LCD_WriteIndex(0x2C);
  CS_LOW();
  RS_HIGH();
  LCD_set_dat(color>>8);
  LCD_set_dat(color);
  CS_HIGH();
}
