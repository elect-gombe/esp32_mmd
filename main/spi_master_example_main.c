/* SPI Master example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"

spi_device_handle_t spi;

/*
  This code displays some fancy graphics on the ILI9341-based 320x240 LCD on an ESP-WROVER_KIT board.
  It is not very fast, even when the SPI transfer itself happens at 8MHz and with DMA, because
  the rest of the code is not very optimized. Especially calculating the image line-by-line
  is inefficient; it would be quicker to send an entire screenful at once. This example does, however,
  demonstrate the use of both spi_device_transmit as well as spi_device_queue_trans/spi_device_get_trans_result
  as well as pre-transmit callbacks.

  Some info about the ILI9341: It has an C/D line, which is connected to a GPIO here. It expects this
  line to be low for a command and high for data. We use a pre-transmit callback here to control that
  line: every transaction has as the user-definable argument the needed state of the D/C line and just
  before the transaction is sent, the callback will set this line to the correct state.
*/

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5


int main3d();

/*
  The ILI9341 needs a bunch of command/argument values to be initialized. They are stored in this struct.
*/
typedef struct {
  uint8_t cmd;
  uint8_t data[16];
  uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} ili_init_cmd_t;

//Send a command to the ILI9341. Uses spi_device_transmit, which waits until the transfer is complete.
void ili_cmd(spi_device_handle_t spi, const uint8_t cmd) 
{
  esp_err_t ret;
  spi_transaction_t t;
  memset(&t, 0, sizeof(t));       //Zero out the transaction
  t.length=8;                     //Command is 8 bits
  t.tx_buffer=&cmd;               //The data is the cmd itself
  t.user=(void*)0;                //D/C needs to be set to 0
  ret=spi_device_transmit(spi, &t);  //Transmit!
  assert(ret==ESP_OK);            //Should have had no issues.
}

//Send data to the ILI9341. Uses spi_device_transmit, which waits until the transfer is complete.
void ili_data(spi_device_handle_t spi, const uint8_t *data, int len) 
{
  esp_err_t ret;
  spi_transaction_t t;
  if (len==0) return;             //no need to send anything
  memset(&t, 0, sizeof(t));       //Zero out the transaction
  t.length=len*8;                 //Len is in bytes, transaction length is in bits.
  t.tx_buffer=data;               //Data
  t.user=(void*)1;                //D/C needs to be set to 1
  ret=spi_device_transmit(spi, &t);  //Transmit!
  assert(ret==ESP_OK);            //Should have had no issues.
}

//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
void ili_spi_pre_transfer_callback(spi_transaction_t *t) 
{
  int dc=(int)t->user;
  gpio_set_level(PIN_NUM_DC, dc);
}

//Initialize the display
void ili_init(spi_device_handle_t spi) 
{
  //Initialize non-SPI GPIOs
  gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
  gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
  gpio_set_direction(PIN_NUM_BCKL, GPIO_MODE_OUTPUT);

  //Reset the display
  gpio_set_level(PIN_NUM_RST, 0);
  vTaskDelay(100 / portTICK_RATE_MS);
  gpio_set_level(PIN_NUM_RST, 1);
  vTaskDelay(100 / portTICK_RATE_MS);

  /* //Send all the commands */
  /* ili_cmd(spi, ili_init_cmds[cmd].cmd); */
  /* ili_data(spi, ili_init_cmds[cmd].data, ili_init_cmds[cmd].databytes&0x1F); */
  uint8_t data;
#define TRANS_1DATA(x) {			\
    data = (x);					\
    ili_data(spi,&data,1);			\
  }
    
  ili_cmd(spi,0x11); //Exit Sleep
  vTaskDelay(2);

  ili_cmd(spi,0x26); //Set Default Gamma
  TRANS_1DATA(0x04);

  ili_cmd(spi,0xB1);//Set Frame Rate
  TRANS_1DATA(0x0C);
  TRANS_1DATA(0x14);

  ili_cmd(spi,0xC0); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
  TRANS_1DATA(0x0C);
  TRANS_1DATA(0x05);

  ili_cmd(spi,0xC1); //Set BT[2:0] for AVDD & VCL & VGH & VGL
  TRANS_1DATA(0x02);//0x00

  ili_cmd(spi,0xC5); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
  TRANS_1DATA(0x29);  //31  21  29
  TRANS_1DATA(0x43);  //58   48  43

  ili_cmd(spi,0xC7);
  TRANS_1DATA(0x40);

  ili_cmd(spi,0x3a); //Set Color Format
  TRANS_1DATA(0x05);
  /*
    ili_cmd(spi,0x2A); //Set Column Address
    TRANS_1DATA(0x00);
    TRANS_1DATA(0x00);
    TRANS_1DATA(0x00);
    TRANS_1DATA(0x7F);	

    ili_cmd(spi,0x2B); //Set Page Address
    TRANS_1DATA(0x00);
    TRANS_1DATA(0x00);
    TRANS_1DATA(0x00);
    TRANS_1DATA(0x9F);	
  */
  ili_cmd(spi,0x36); //Set Scanning Direction
  TRANS_1DATA(0xA8); //0xA8
  ili_cmd(spi,0xB7); //Set Source Output Direction
  TRANS_1DATA(0x00);

  ili_cmd(spi,0xF2); //Enable Gamma bit
  TRANS_1DATA(0x01);	
  /*
    ili_cmd(spi,0xE0);
    TRANS_1DATA(0x36);//p1
    TRANS_1DATA(0x29);//p2
    TRANS_1DATA(0x12);//p3
    TRANS_1DATA(0x22);//p4
    TRANS_1DATA(0x1C);//p5
    TRANS_1DATA(0x15);//p6
    TRANS_1DATA(0x42);//p7
    TRANS_1DATA(0xB7);//p8
    TRANS_1DATA(0x2F);//p9
    TRANS_1DATA(0x13);//p10
    TRANS_1DATA(0x12);//p11
    TRANS_1DATA(0x0A);//p12
    TRANS_1DATA(0x11);//p13
    TRANS_1DATA(0x0B);//p14
    TRANS_1DATA(0x06);//p15

    ili_cmd(spi,0xE1);
    TRANS_1DATA(0x09);//p1
    TRANS_1DATA(0x16);//p2
    TRANS_1DATA(0x2D);//p3
    TRANS_1DATA(0x0D);//p4
    TRANS_1DATA(0x13);//p5
    TRANS_1DATA(0x15);//p6
    TRANS_1DATA(0x40);//p7
    TRANS_1DATA(0x48);//p8
    TRANS_1DATA(0x53);//p9
    TRANS_1DATA(0x0C);//p10
    TRANS_1DATA(0x1D);//p11
    TRANS_1DATA(0x25);//p12
    TRANS_1DATA(0x2E);//p13
    TRANS_1DATA(0x34);//p14
    TRANS_1DATA(0x39);//p15
  */
  ili_cmd(spi,0x29); // Display On
  ili_cmd(spi,0x2c);
  //CS0 = 1;
#undef TRANS_1DATA        
  ///Enable backlight
  gpio_set_level(PIN_NUM_BCKL, 0);
}


void send_line_finish(void) 
{
  spi_transaction_t *rtrans;
  esp_err_t ret;
  //Wait for all 6 transactions to be done and get back the results.
  for (int x=0; x<6; x++) {
    ret=spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY);
    assert(ret==ESP_OK);
    //We could inspect rtrans now if we received any info back. The LCD is treated as write-only, though.
  }
}

//To send a line we have to send a command, 2 data bytes, another command, 2 more data bytes and another command
//before sending the line data itself; a total of 6 transactions. (We can't put all of this in just one transaction
//because the D/C line needs to be toggled in the middle.)
//This routine queues these commands up so they get sent as quickly as possible.
void send_line(int ypos, uint16_t *line) 
{
  esp_err_t ret;
  int x;
  //Transaction descriptors. Declared static so they're not allocated on the stack; we need this memory even when this
  //function is finished because the SPI driver needs access to it even while we're already calculating the next line.
  static spi_transaction_t trans[6];
  static int transed;
  
  if(transed!=0)send_line_finish();
  transed = 1;
  //In theory, it's better to initialize trans and data only once and hang on to the initialized
  //variables. We allocate them on the stack, so we need to re-init them each call.
  for (x=0; x<6; x++) {
    memset(&trans[x], 0, sizeof(spi_transaction_t));
    if ((x&1)==0) {
      //Even transfers are commands
      trans[x].length=8;
      trans[x].user=(void*)0;
    } else {
      //Odd transfers are data
      trans[x].length=8*4;
      trans[x].user=(void*)1;
    }
    trans[x].flags=SPI_TRANS_USE_TXDATA;
  }
  trans[0].tx_data[0]=0x2A;           //Column Address Set
  trans[1].tx_data[0]=1;              //Start Col High
  trans[1].tx_data[1]=1;              //Start Col Low
  trans[1].tx_data[3]=160;     //End Col Low
  trans[2].tx_data[0]=0x2B;           //Page address set
  trans[3].tx_data[0]=(ypos+2)>>8;        //Start page high
  trans[3].tx_data[1]=(ypos+2)&0xff;      //start page low
  trans[3].tx_data[2]=(ypos+3)>>8;    //end page high
  trans[3].tx_data[3]=(ypos+3)&0xff;  //end page low
  trans[4].tx_data[0]=0x2C;           //memory write
  trans[5].tx_buffer=line;            //finally send the line data
  trans[5].length=160*2*8;            //Data length, in bits
  trans[5].flags=0; //undo SPI_TRANS_USE_TXDATA flag

  //Queue all transactions.
  for (x=0; x<6; x++) {
    ret=spi_device_queue_trans(spi, &trans[x], portMAX_DELAY);
    assert(ret==ESP_OK);
  }

  //When we are here, the SPI driver is busy (in the background) getting the transactions sent. That happens
  //mostly using DMA, so the CPU doesn't have much to do here. We're not going to wait for the transaction to
  //finish because we may as well spend the time calculating the next line. When that is done, we can call
  //send_line_finish, which will wait for the transfers to be done and check their status.
}



//Simple routine to generate some patterns and send them to the LCD. Don't expect anything too
//impressive. Because the SPI driver handles transactions in the background, we can calculate the next line
//while the previous one is being sent.
static void display_pretty_colors(spi_device_handle_t spi)
{
  uint16_t line[2][160];
  int x, y, frame=0;
  //Indexes of the line currently being sent to the LCD and the line we're calculating.
  int sending_line=-1;
  int calc_line=0;
    
  while(1) {
    frame++;
    for (y=0; y<128; y++) {
      //Calculate a line.
      for (x=0; x<160; x++) {
	line[calc_line][x]=((x<<3)^(y<<3)^(frame+x*y));
      }
      //Finish up the sending process of the previous line, if any
      //Swap sending_line and calc_line
      sending_line=calc_line;
      calc_line=(calc_line==1)?0:1;
      //Send the line we currently calculated.
      send_line(y, line[sending_line]);
      //The line is queued up for sending now; the actual sending happens in the
      //background. We can go on to calculate the next line as long as we do not
      //touch line[sending_line]; the SPI sending process is still reading from that.
    }
  }
}

void app_main()
{
  esp_err_t ret;
  spi_bus_config_t buscfg={
    .miso_io_num=PIN_NUM_MISO,
    .mosi_io_num=PIN_NUM_MOSI,
    .sclk_io_num=PIN_NUM_CLK,
    .quadwp_io_num=-1,
    .quadhd_io_num=-1
  };
  spi_device_interface_config_t devcfg={
    .clock_speed_hz=20000000,               //Clock out at 20 MHz
    .mode=0,                                //SPI mode 0
    .spics_io_num=PIN_NUM_CS,               //CS pin
    .queue_size=7,                          //We want to be able to queue 7 transactions at a time
    .pre_cb=ili_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
  };
  //Initialize the SPI bus
  ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
  assert(ret==ESP_OK);
  //Attach the LCD to the SPI bus
  ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
  assert(ret==ESP_OK);
  //Initialize the LCD
  ili_init(spi);
  //Go do nice stuff.
  /* display_pretty_colors(spi); */
  main3d();
}
