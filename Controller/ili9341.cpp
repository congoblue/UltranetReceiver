/***************************************************
  Arduino TFT graphics library targetted at the UNO
  and Mega boards.

  This library has been derived from the Adafruit_GFX
  library and the associated driver library. See text
  at the end of this file.

  This is a standalone library that contains the
  hardware driver, the graphics funtions and the
  proportional fonts.

  The larger fonts are Run Length Encoded to reduce their
  size.

 ****************************************************/

#if defined(ARDUINO_ARCH_SAMD)
  #define SPI_MIN_CLOCK_DIVIDER (uint8_t)(1 + ((F_CPU - 1) / 24000000))
  #endif

//this doesn't work. You need to edit spi.h in
//C:\Users\Tim.SABRE\AppData\Local\Arduino15\packages\arduino\hardware\samd\1.8.14\libraries\SPI

#include <Arduino.h>
#include "graphics.h"
#include <SPI.h>

#define D_SCK 9
#define D_MOSI 8
#define DISPCS 10
#define DISPA0 11
#define DISPRST 12

#include "ili9341.h"

uint16_t addr_col,addr_row;
uint16_t _width,_height;
char pixwrmode=0;

uint32_t spi_speed = 24000000; 

// If the SPI library has transaction support, these functions
// establish settings and protect from interference from other
// libraries.  Otherwise, they simply do nothing.
#ifdef SPI_HAS_TRANSACTION
static inline void spi_begin(void) __attribute__((always_inline));
static inline void spi_begin(void) {
  // max speed!
  SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE0));
}
static inline void spi_end(void) __attribute__((always_inline));
static inline void spi_end(void) { SPI.endTransaction(); }
#else
#define spi_begin() ///< Create dummy Macro Function
#define spi_end()   ///< Create dummy Macro Function
#endif

/***************************************************************************************
** Function name:           spiwrite
** Description:             Write 8 bits to SPI port
***************************************************************************************/
void spiwrite(uint8_t c)
{
  char x;
  SPI.transfer(c);
}

void spiwrite_nowait(uint8_t c)
{
  SPI.transfer(c);
}



/***************************************************************************************
** Function name:           writecommand
** Description:             Send an 8 bit command to the TFT
***************************************************************************************/
void writecommand(uint8_t c)
{
  TFT_DC_C;
  TFT_CS_L;
  spiwrite(c);
  TFT_CS_H;
}

void sendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes) 
{
  uint8_t i;  
  TFT_CS_L;
  TFT_DC_C;
  spiwrite(commandByte);
  TFT_DC_D;
  for (i=0; i<numDataBytes; i++) spiwrite(*dataBytes++);
  TFT_CS_H;
}

/***************************************************************************************
** Function name:           writedata
** Description:             Send a 8 bit data value to the TFT
***************************************************************************************/
void writedata(uint8_t c)
{
  
  TFT_DC_D;
  TFT_CS_L;
  spiwrite(c);
  TFT_CS_H;
}



/***************************************************************************************
** Function name:           init
** Description:             Reset, then initialise the TFT display registers
***************************************************************************************/
#define DELAY 0x80 
//0x80 is a flag to enable delay, the actual delay follows

void tft_init(void)
{
  char tabcolor;
  

  SPI.begin(); //

  pinMode(DISPCS,OUTPUT);
  pinMode(DISPA0,OUTPUT);
  pinMode(DISPRST,OUTPUT);
  
  // toggle RST low to reset
  digitalWrite(DISPRST,0);
  delay(2);
  digitalWrite(DISPRST,1);
  delay(100);
  

  //spi_speed = 8000000;
  spi_begin();


	static const uint8_t PROGMEM initcmd[] = {
  0xEF, 3, 0x03, 0x80, 0x02,
  0xCF, 3, 0x00, 0xC1, 0x30,
  0xED, 4, 0x64, 0x03, 0x12, 0x81,
  0xE8, 3, 0x85, 0x00, 0x78,
  0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  0xF7, 1, 0x20,
  0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  ILI9341_DISPON  , 0x80,                // Display on
  0x00                                   // End of list
};


   uint8_t cmd, x, numArgs;
  uint8_t *addr = (uint8_t *)initcmd;
  while ((cmd = pgm_read_byte(addr++)) > 0) {
    x = pgm_read_byte(addr++);
    numArgs = x & 0x7F;
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;
    if (x & 0x80)
      delay(150);
  }

  _width = ILI9341_TFTWIDTH;
  _height = ILI9341_TFTHEIGHT;



}


/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void setRotation(uint8_t m) {
  uint8_t rotation = m % 4; // can't be higher than 3
  switch (rotation) {
  case 0:
    m = (MADCTL_MX | MADCTL_BGR);
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    break;
  case 1:
    m = (MADCTL_MV | MADCTL_BGR);
    _width = ILI9341_TFTHEIGHT;
    _height = ILI9341_TFTWIDTH;
    break;
  case 2:
    m = (MADCTL_MY | MADCTL_BGR);
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    break;
  case 3:
    m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
    _width = ILI9341_TFTHEIGHT;
    _height = ILI9341_TFTWIDTH;
    break;
  }
sendCommand(ILI9341_MADCTL, &m, 1);
}

/**************************************************************************/
/*!
    @brief   Set the height of the Top and Bottom Scroll Margins
    @param   top The height of the Top scroll margin
    @param   bottom The height of the Bottom scroll margin
 */
/**************************************************************************/
void setScrollMargins(uint16_t top, uint16_t bottom) {
  // TFA+VSA+BFA must equal 320
  if (top + bottom <= ILI9341_TFTHEIGHT) {
    uint16_t middle = ILI9341_TFTHEIGHT - (top + bottom);
    uint8_t data[6];
    data[0] = top >> 8;
    data[1] = top & 0xff;
    data[2] = middle >> 8;
    data[3] = middle & 0xff;
    data[4] = bottom >> 8;
    data[5] = bottom & 0xff;
    sendCommand(ILI9341_VSCRDEF, (uint8_t *)data, 6);
  }
}

/***************************************************************************************
** Function name:           drawPixel
** Description:             push a single pixel at an arbitrary position
***************************************************************************************/
// Smarter version that takes advantage of often used orthogonal coordinate plots
// where either x or y does not change
void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  char v;
  // Faster range checking, possible because x and y are unsigned
  if ((x >= _width) || (y >= _height)) return;

  TFT_DC_C;
  TFT_CS_L;

  if  ((pixwrmode==0)||(addr_col!=x)||(addr_row!=y))
  {
      if ((addr_col != x)||(addr_row!=y)) {
        SPI.transfer(ILI9341_CASET); //while((LPC_SPI0->STAT&(1<<0))==0);  v=LPC_SPI0->RXDAT; //wait for spi to send completely & clear the flag by reading
        addr_col = x;
        TFT_DC_D;
        SPI.transfer16(x); 
        TFT_DC_C;
      }

      if (addr_row != y) {
        SPI.transfer(ILI9341_PASET); //while((LPC_SPI0->STAT&(1<<0))==0);  v=LPC_SPI0->RXDAT; //wait for spi to send completely & clear the flag by reading
        addr_row = y;
        TFT_DC_D;
        SPI.transfer16(y); 
        TFT_DC_C;
      }

        SPI.transfer(ILI9341_RAMWR); //while((LPC_SPI0->STAT&(1<<0))==0);  v=LPC_SPI0->RXDAT; //wait for spi to send completely & clear the flag by reading
        pixwrmode=1;
  }
  TFT_DC_D;

  //spiwrite_nowait(color >> 8);
  //spiwrite(color);
  //SPI.transfer(color>>8); 
  //while((LPC_SPI0->STAT&(1<<0))==0); v=LPC_SPI0->RXDAT; //wait for spi to send completely & clear the flag by reading
  //SPI.transfer(color); 
  SPI.transfer16(color);
  //while((LPC_SPI0->STAT&(1<<0))==0); v=LPC_SPI0->RXDAT; //wait for spi to send completely & clear the flag by reading
  addr_col=x+1;

  TFT_CS_H;

}

/***************************************************************************************
** Function name:           setWindow
** Description:             define an area to receive a stream of pixels
***************************************************************************************/
// Chip select stays low, use setAddrWindow() from sketches

void setWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{

  addr_row = 0xFF;
  addr_col = 0xFF;

  // Column addr set
  TFT_DC_C;
  TFT_CS_L;

  SPI.transfer(ILI9341_CASET);

  TFT_DC_D;
  /*
  spiwrite_nowait(0);
  spiwrite_nowait(x0+colstart);
  spiwrite_nowait(0);*/
  SPI.transfer16(x0);
  SPI.transfer16(x1);

  // Row addr set
  TFT_DC_C;
  SPI.transfer(ILI9341_PASET);

  TFT_DC_D;
  /*spiwrite_nowait(0);
  spiwrite_nowait(y0+rowstart);
  spiwrite_nowait(0);*/
  SPI.transfer16(y0);
  SPI.transfer16(y1);

  // write to RAM
  TFT_DC_C;
  SPI.transfer(ILI9341_RAMWR);

  TFT_DC_D;
  pixwrmode=0; 

}

/***************************************************************************************
** Function name:           spiWrite16
** Descriptions:            Delay based assembler loop for fast SPI write
***************************************************************************************/
void spiWrite16(uint16_t data, int16_t count)
{
    char v;
    while (count>0)
    {
      //spiwrite(data>>8);
      //spiwrite(data&0xFF);
      //SPI.transfer(data>>8);
      //while((LPC_SPI0->STAT&(1<<0))==0); v=LPC_SPI0->RXDAT; //wait for spi to send completely & clear the flag by reading
      //SPI.transfer(data&0xFF);
      //while((LPC_SPI0->STAT&(1<<0))==0); v=LPC_SPI0->RXDAT; //wait for spi to send completely & clear the flag by reading
      SPI.transfer16(data);

      count--;
    }
}

/***************************************************************************************
** Function name:           drawFastVLine
** Description:             draw a vertical line
***************************************************************************************/
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((y + h - 1) >= _height) h = _height - y;


  setWindow(x, y, x, _height);

  spiWrite16(color, h);
  TFT_CS_H;

}

/***************************************************************************************
** Function name:           drawFastHLine
** Description:             draw a horizontal line
***************************************************************************************/
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((x + w - 1) >= _width)  w = _width - x;

  setWindow(x, y, _width, y);

  spiWrite16(color, w);
  TFT_CS_H;

}

/***************************************************************************************
** Function name:           fillRect
** Description:             draw a filled rectangle
***************************************************************************************/
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  // rudimentary clipping (drawChar w/big text requires this)
  if ((x > _width) || (y > _height) || (w==0) || (h==0)) return;
  if ((x + w - 1) > _width)  w = _width  - x;
  if ((y + h - 1) > _height) h = _height - y;

  setWindow(x, y, x + w - 1, y + h - 1);

  while (h--) spiWrite16(color, w);
  TFT_CS_H;

}

/***************************************************************************************
** Function name:           fillScreen
** Description:             Clear the screen to defined colour
***************************************************************************************/
void fillScreen(uint16_t color)
{
  fillRect(0, 0, _width, _height, color);
}



/***************************************************************************************
** Function name:           color565
** Description:             convert three 8 bit RGB levels to a 16 bit colour value
***************************************************************************************/
uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}



/***************************************************************************************
** Function name:           invertDisplay
** Description:             invert the display colours i = 1 invert, i = 0 normal
***************************************************************************************/
void invertDisplay(char i)
{
  writecommand(i ? ILI9341_INVON : ILI9341_INVOFF);
}


