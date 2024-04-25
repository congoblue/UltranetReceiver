
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#include <stdarg.h>


#include <Arduino.h>
//#include "global.h"
#include "graphics.h"
#include "ili9341.h"

/*#include "bitmaps/bitmap_pwr.h"
#include "bitmaps/bitmap_menu.h"
#include "bitmaps/bitmap_left.h"
#include "bitmaps/bitmap_right.h"
#include "bitmaps/bitmap_dotselect.h"
#include "bitmaps/bitmap_dotnotselect.h"
#include "bitmaps/bitmap_colselect_text.h"
#include "bitmaps/bitmap_colpalette.h"
#include "bitmaps/bitmap_colpalette_w.h"
#include "bitmaps/bitmap_colselectring.h"
#include "bitmaps/bitmap_coltune_text.h"
#include "bitmaps/bitmap_dimmer_text.h"
#include "bitmaps/bitmap_shade_text.h"
#include "bitmaps/bitmap_white_text.h"
#include "bitmaps/bitmap_soundtolight_text.h"
#include "bitmaps/bitmap_autoscroll_text.h"
#include "bitmaps/bitmap_whitebar.h"
#include "bitmaps/bitmap_pointerdot.h"
#include "bitmaps/bitmap_autoscroll_icon.h"
#include "bitmaps/bitmap_soundtolight_icon.h"
#include "bitmaps/bitmap_offon_bar.h"
#include "bitmaps/bitmap_offon_dot.h"
#include "bitmaps/bitmap_offon_text.h"
#include "bitmaps/bitmap_slowfast_text.h"
#include "bitmaps/bitmap_sequence_text.h"
#include "bitmaps/bitmap_zone_text.h"
#include "bitmaps/bitmap_select_unselect.h"
#include "bitmaps/bitmap_dot_select_flash.h"*/

char chname[16][16]=
{
  "DrumsL",
  "DrumsR",
  "PianoL",
  "PianoR",
  "Acoustic",
  "Electric",
  "Inst1",
  "Inst2",
  "Vox1",
  "Vox2",
  "Vox3",
  "Vox4",
  "Extra",
  "Lead",
  "AmbL",
  "AmbR"
};


int xp=0;
int yp=0;
volatile uint8_t EncValue=0;
volatile uint8_t EncChange=0;
volatile uint8_t KeyState=0;
volatile uint8_t KeyHit=0;
volatile uint8_t LastKey=0;
uint8_t level[16]; //metering
uint8_t volume[16];
uint8_t pan[16];
uint8_t link[16];
uint8_t UltranetGood=0;

//font structure
typedef struct
{
  char ht;
  char w[95];
  unsigned int pos[95];
  //char v[];
} fontstruct;

//const fontstruct cfont_basic=
//#include "cfont_basic.inc"

/*const fontstruct cfont_arial=
#include "fonts/carial.inc"

const fontstruct cfont_arialb=
#include "fonts/carialb.inc"

const fontstruct cfont_arial12=
#include "fonts/rle_arial12.inc"
const fontstruct cfont_arial12b=
#include "fonts/rle_arial12b.inc"


const fontstruct cfont_arial16=
#include "fonts/rle_arial16.inc"*/
//const fontstruct cfont_arial16b=
//#include "fonts/rle_arial16b.inc"
/*const fontstruct cfont_arial32=
#include "fonts/rle_arial32.inc"
const fontstruct cfont_georgia=
#include "fonts/cgeorgia.inc"
const fontstruct cfont_georgiab=
#include "fonts/cgeorgiab.inc"
const fontstruct cfont_georgia12=
#include "fonts/rle_georgia12.inc"
const fontstruct cfont_georgia12b=
#include "fonts/rle_georgia12b.inc"
const fontstruct cfont_georgia16=
#include "fonts/rle_georgia16.inc"
const fontstruct cfont_georgia16b=
#include "fonts/rle_georgia16b.inc"

const fontstruct cfont_lucida12=
#include "fonts/rle_lucida12.inc"
const fontstruct cfont_lucida14=
#include "fonts/rle_lucida14.inc"
//const fontstruct cfont_fixed14=
//#include "fonts/rle_cfixed14.inc"
const fontstruct cfont_courier16=
#include "fonts/ccourier16.inc"


const fontstruct cfont_basic=
#include "fonts/cbasic8.inc"
//const fontstruct cfont_basic12=
#include "fonts/cbasic12.h"
//const fontstruct cfont_basic16=
#include "fonts/cbasic16.h"

const fontstruct cfont_swiss721md16=
#include "fonts/rle_swis721md_16.inc"
const fontstruct cfont_swiss721lt=
#include "fonts/rle_swis721lt.inc"*/


//const fontstruct cfont_lr14v=
#include "rle_lr14v2.h" 
//#include "fonts/rle_ubuntu12.inc" 


fontstruct *pcurrentfont=&cfont_lr14v;
char *pcurrentfontdata=(char*)&cfont_lr14vc[0];

unsigned char linespacing=2;
unsigned char align=0;
unsigned char valign=0;
unsigned char wordwrap=1;
unsigned char autofontsize=1;
int fontsize=16;
int cfont=0;
unsigned int colour=0xFFFFFF;
unsigned int backcolour=0;
unsigned char backmode=0;
unsigned char textrotate=0;

unsigned char Trigger=0;
unsigned char IsClr=0;

//unsigned char remap_buf[(swidth*16)*(sheight*8)];

const uint16_t arrowup[]={
0b0000010000000000,
0b0000111000000000,
0b0001111100000000,
0b0011111110000000,
0b0111111111000000,
0b1111111111100000,
0b0011111110000000,
0b0011111110000000,
0b0011111110000000,
0b0011111110000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
};

const uint16_t arrowdown[]={
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0011111110000000,
0b0011111110000000,
0b0011111110000000,
0b0011111110000000,
0b1111111111100000,
0b0111111111000000,
0b0011111110000000,
0b0001111100000000,
0b0000111000000000,
0b0000010000000000,
};

const uint16_t exclaim[]={
0b0000001111000000,
0b0000001111000000,
0b0000011111100000,
0b0000011111100000,
0b0000111001110000,
0b0000111001110000,
0b0001111001111000,
0b0001111001111000,
0b0011111001111100,
0b0011111001111100,
0b0111111001111110,
0b0111111111111110,
0b1111111001111111,
0b1111111001111111,
0b1111111111111111,
0b1111111111111111,
};

const uint16_t info[]={
0b0000001111000000,
0b0001111111111000,
0b0011111001111100,
0b0111111001111110,
0b0111111111111110,
0b0111110001111110,
0b1111110001111111,
0b1111111001111111,
0b1111111001111111,
0b1111111001111111,
0b0111111001111110,
0b0111111001111110,
0b0111110000111110,
0b0011110000111100,
0b0001111111111000,
0b0000001111000000,
};

const uint16_t rightarrow[]={
0b0000000000000000,
0b0000000000100000,
0b0000000000110000,
0b0000000000111000,
0b0000000000111100,
0b0000000000111110,
0b0000000000111111,
0b0000000000111110,
0b0000000000111100,
0b0000000000111000,
0b0000000000110000,
0b0000000000100000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
};

const uint16_t downarrow[]={
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0011111111111000,
0b0001111111110000,
0b0000111111100000,
0b0000011111000000,
0b0000001110000000,
0b0000000100000000,
};

const uint16_t checkbox[]={
0b1111111111110000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1000000000010000,
0b1111111111110000,
0b0000000000000000,
0b0000000000000000,
};

const uint16_t checkboxtick[]={
0b1111111111110000,
0b1000000000010000,
0b1010000001010000,
0b1011000011010000,
0b1001100110010000,
0b1000111100010000,
0b1000011000010000,
0b1000111100010000,
0b1001100110010000,
0b1011000011010000,
0b1010000001010000,
0b1000000000010000,
0b1000000000010000,
0b1111111111110000,
0b0000000000000000,
0b0000000000000000,
};

const uint16_t dot[]={
0b0000001111000000,
0b0001111111111000,
0b0011111111111100,
0b0111111111111110,
0b0111111111111110,
0b0111111111111110,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b0111111111111110,
0b0111111111111110,
0b0111111111111110,
0b0011111111111100,
0b0001111111111000,
0b0000001111000000,
};

const unsigned char HSLCurve[] = {
	//linear spectrum
	
   0,6,12,18,24,30,36,42,47,53,59,65,71,77,83,89,
   95,101,107,113,119,125,130,136,142,148,154,160,166,172,178,184,
   190,196,202,208,213,219,225,231,237,243,249,255
   
	//damped exponential
	/*0,0,2,4,8,10,14,16,19,22,24,27,30,33,36,39,
    43,46,50,53,57,61,65,70,75,80,85,91,97,103,110,117,
    125,133,142,152,162,174,186,199,213,229,255*/
	//exponential spectrum
	/*
 	0,0,0,1,2,2,3,4,5,6,7,8,9,11,13,14,
    16,18,21,23,26,29,32,36,40,45,50,55,61,68,75,83,
    91,101,111,123,136,150,165,182,200,220,255
	*/

};

//-----------------------------------------------------------------------------
// process state change in the rotary encoder
//-----------------------------------------------------------------------------
void SenseEncoder(void)
{
   static uint8_t encstate=0xFF;

   //if (encstate==0xFF) encstate=digitalRead(A4)|(digitalRead(A5)<<1);

   if ((encstate&1)&&(digitalRead(A4)==0)) //x falling edge
   {
      if (digitalRead(A5)!=0) 
      {
        if (EncValue<252) EncValue+=4; else EncValue=255;
      }
      else
      {
        if (EncValue>4) EncValue-=4; else EncValue=0;
      }
      EncChange=1;
   }
   else if (((encstate&1)==0)&&(digitalRead(A4)!=0)) //x rising edge
   {
      if (digitalRead(A5)==0) 
      {
        if (EncValue<252) EncValue+=4; else EncValue=255;
      }
      else
      {
        if (EncValue>4) EncValue-=4; else EncValue=0;
      }
      EncChange=1;    
   }
   encstate=digitalRead(A4)|(digitalRead(A5)<<1);

}

//-----------------------------------------------------------------------------
// process state change in the pushbuttons + rotary encoder push
//-----------------------------------------------------------------------------
void SenseKeys(void)
{
    uint8_t v;
    uint8_t newkeystate=digitalRead(A0)|(digitalRead(A1)<<1)|(digitalRead(A2)<<2)|(digitalRead(A3)<<3)|(digitalRead(A6)<<4);
    v=newkeystate^KeyState;
    if (newkeystate<KeyState) //new press if less
    {
        KeyHit=1;
        if (v&1) LastKey=1;
        if (v&2) LastKey=2;
        if (v&4) LastKey=3;
        if (v&8) LastKey=4;
        if (v&0x10) LastKey=5;
    }

    KeyState=newkeystate;
}

//-----------------------------------------------------------------------------
// convert hsl to rgb
//-----------------------------------------------------------------------------
void HslToRgbw(uint8_t *opr, uint8_t *og, uint8_t *ob, uint8_t h, uint8_t s, uint8_t spread, uint8_t dim)
{
	uint8_t r,g,b,w=0;
	//hue
	h=(h+spread)&0xFF;
	h+=43; //give 0=red
	if (h<43)
	{
		r=255;
		g=0;
		b=HSLCurve[42-h];
	}
	else if (h<85)
	{
		r=255;
		g=HSLCurve[h-43];
		b=0;
	}
	else if (h<128)
	{
		r=HSLCurve[127-h];
		g=255;
		b=0;
	}
	else if (h<171)
	{
		r=0;
		g=255;
		b=HSLCurve[h-128];
	}
	else if (h<213)
	{
		r=0;
		g=HSLCurve[212-h];
		b=255;
	}
	else
	{
		r=HSLCurve[h-213];
		g=0;
		b=255;
	}
	//sat
	if (r+s<255) r=r+s; else r=255;
	if (g+s<255) g=g+s; else g=255;
	if (b+s<255) b=b+s; else b=255;
	//dim
	*opr=r*dim/255;
	*og=g*dim/255;
	*ob=b*dim/255;
	}


//-----------------------------------------------------------------------------
// convert white colour temp to rgb
//-----------------------------------------------------------------------------
void ColTemp(uint8_t *opr, uint8_t *og, uint8_t *ob, uint8_t h, uint8_t dim)
{
	uint8_t r,g,b,cr,cg,cb,w=0;
	uint8_t wr,wg,wb;

  #define WWHITE 0xFFFF80
  #define CWHITE 0x80FFFF

  wr=(WWHITE>>16)&0xFF;
  wg=(WWHITE>>8)&0xFF;
  wb=WWHITE&0xFF;
  cr=(CWHITE>>16)&0xFF;
  cg=(CWHITE>>8)&0xFF;
  cb=CWHITE&0xFF;  
	
  r=((cr*(255-h))+(wr*h))>>8;
  g=((cg*(255-h))+(wg*h))>>8;
  b=((cb*(255-h))+(wb*h))>>8;

	//dim
	*opr=r*dim/255;
	*og=g*dim/255;
	*ob=b*dim/255;
}


//-----------------------------------------------------------------------------
// calculate white
//-----------------------------------------------------------------------------
uint8_t CalcWhite(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t mw=0xFF;
  if (r<mw) mw=r;
  if (g<mw) mw=g;
  if (b<mw) mw=b;

  return mw;
}

//----------------------------------------------------------------------------
//!  set font
//----------------------------------------------------------------------------
void setfont(char size, char style)
{
    if (style==0)
    {
      //if (size==8) pcurrentfont=&cfont_arial;
      //if (size==12) {pcurrentfont=&cfont_ubuntu12; pcurrentfontdata=(char*)cfont_ubuntu12c;}
      //if (size==16) pcurrentfont=&cfont_arial16;
      //if (size==32) pcurrentfont=&cfont_arial32;
    }
    
    /*if (style==1)
    {
      //if (size==8) pcurrentfont=&cfont_arialb;
      //if (size==12) pcurrentfont=&cfont_arial12b;
      if (size==16) {pcurrentfont=&cfont_arial16b; pcurrentfontdata=(char*)cfont_arial16bc;}
    }*/
    
    /*
    if (style==2)
    {
      if (size==8) pcurrentfont=&cfont_georgia;
      if (size==12) pcurrentfont=&cfont_georgia12;
      if (size==16) pcurrentfont=&cfont_georgia16;
    }*/
    /*if (style==3)
    {
      if (size==8) pcurrentfont=&cfont_georgiab;
      if (size==12) pcurrentfont=&cfont_georgia12b;
      if (size==16) pcurrentfont=&cfont_georgia16b;
    }*/
    /*if (style==4)
    {
      if (size==8) pcurrentfont=&cfont_arialn;
      if (size==12) pcurrentfont=&cfont_arialn12;
      if (size==16) pcurrentfont=&cfont_arialn16;
    }
    */
    if (style==5)
    {
      //if (size==8) pcurrentfont=&cfont_arialnb;
      //if (size==13) pcurrentfont=&cfont_lr13;
      if (size==14) {pcurrentfont=&cfont_lr14v; pcurrentfontdata=(char*)cfont_lr14vc;}
      //if (size==16) pcurrentfont=&cfont_arialn16;
    }
    
    /*if (style==6)
    {
      //if (size==8) pcurrentfont=&cfont_impact8;
      if (size==12) pcurrentfont=&cfont_lucida12;
      if (size==14) pcurrentfont=&cfont_lucida14;
      if (size==16) pcurrentfont=&cfont_courier16;
    }
    if (style==7)
    {
      if (size==8) pcurrentfont=&cfont_basic;
      if (size==12) pcurrentfont=&cfont_basic12;
      if (size==16) pcurrentfont=&cfont_basic16;
    }
    if (style==8)
    {
      if (size==16) pcurrentfont=&cfont_swiss721md16;
      if (size==32) pcurrentfont=&cfont_swiss721lt;
    }*/


    cfont=style;
}



//----------------------------------------------------------------------------
//!  string height of multiline string
//----------------------------------------------------------------------------
unsigned int stringheight(char *c)
{
    unsigned int ht=pcurrentfont->ht; //start of one line high
    while (*c!=0)
    {
       if (*c==10) ht=ht+(pcurrentfont->ht)+linespacing;
       c++;
    }
    return ht;
}

//----------------------------------------------------------------------------
//!  string width to next cr or end
//----------------------------------------------------------------------------
unsigned int linewidth(char *c)
{
    int wid=0;
    while ((*c!=0)&&(*c!=13))
    {
       if (*c>=32) wid=wid+(pcurrentfont->w[(*c)-32])+1;
       c++;
    }
    return wid;
}

//----------------------------------------------------------------------------
//!  set pixel
//----------------------------------------------------------------------------
void opset(unsigned int x, unsigned int y, unsigned char c)
{
   if ((x<0)||(x>=_width)) return;
   if ((y<0)||(y>=_height)) return;
   //x=(_width)-1-x;    //correct for being upside down
   //y=_height*8-1-y;
   drawPixel(x,y,color565(c,c,c));
   IsClr=0; //flag screen not clr

}

//----------------------------------------------------------------------------
//!  clearscreen
//----------------------------------------------------------------------------
void opclrscr(void)
{
   if (IsClr) return; //save time if already clr
   fillScreen(color565((backcolour>>16)&0xFF,(backcolour>>8)&0xFF,backcolour&0xFF));
   IsClr=1;
   xp=0; yp=0;
}

void UserRectClr(void)
{
   fillRect(0,32,128,160-32,color565((backcolour>>16)&0xFF,(backcolour>>8)&0xFF,backcolour&0xFF));
}

void DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
   uint16_t col=color565((colour>>16)&0xFF,(colour>>8)&0xFF,colour&0xFF);
   drawFastHLine(x,y,w-1,col);
   drawFastHLine(x,y+h-1,w-1,col);
   drawFastVLine(x,y,h-1,col);
   drawFastVLine(x+w-1,y,h-1,col);
}

//----------------------------------------------------------------------------
//!  putch
//----------------------------------------------------------------------------
void opputc(char c)
{
   int txp=xp, typ=yp;
   int i,j,cc;
   char *p,pv;
   unsigned char rc=0,rv;
   char cr=(colour>>16)&0xFF,cg=(colour>>8)&0xFF,cb=colour&0xFF;
   char br=(backcolour>>16)&0xFF,bg=(backcolour>>8)&0xFF,bb=backcolour&0xFF;

   //check for special chars
   if (c==180) c=39; //strange '

   
   p=(char *)(pcurrentfontdata+pcurrentfont->pos[c-32]); //this gives a pointer to the character bitmap data
   if (c>=32)
   {
     for (j=0; j<pcurrentfont->ht; j++)
     {
        for (i=0; i<(pcurrentfont->w[c-32]); i++)
        {
           if (rc==0) //not in a runlength encode
           {
             pv=*p;     
             p++;
             if (pv==1) 
             {
              rv=*p; p++; rc=*p-1; p++; //run length encoding start. if pix value is 1 then next 2 bytes are value:count
              pv=rv;
             }
           }
           else //currently in a runlength encode
           {
             pv=rv; //just keep repeating the same value
             rc--;
           }
           if (backmode==0)
           {
            cc=color565(((pv*cr)>>8)+(((255-pv)*br)>>8),((pv*cg)>>8)+(((255-pv)*bg)>>8),((pv*cb)>>8)+(((255-pv)*bb)>>8));
            if (textrotate==0)
              drawPixel(xp+i,yp+j,cc);
            else
              drawPixel(xp+j,yp-i,cc);
           }
           else 
           {
              if (pv>128) 
              {
                if (textrotate==0)
                  drawPixel(xp+i,yp+j,colour);
                else
                  drawPixel(xp+j,yp-i,colour);
              }              
           }
        }
        if (backmode==0) 
        {      
          if (textrotate==0)
            drawPixel(xp+i,yp+j,color565(br,bg,bb)); //1pix gap to right of char
          else
            drawPixel(xp+j,yp-i,color565(br,bg,bb)); //1pix gap to right of char
        }
     }
     if (textrotate==0)
     {
      xp+=pcurrentfont->w[c-32]+1;
      if (xp>=_width) {xp=0; yp+=pcurrentfont->ht+linespacing;}
     }
     else
     {
      yp-=pcurrentfont->w[c-32]+1;
      if (yp<0) {yp=_height-1; xp+=pcurrentfont->ht+linespacing;}
     }
   }
   IsClr=0; //flag screen not clr
}

//----------------------------------------------------------------------------
//!  word wrap the string
//----------------------------------------------------------------------------
void wrap(char *c, char *out)
{
    int wid=0;
    int len;
    char *lastsp=c;
    char *plinestart;
    while (*c!=0)
    {
        wid=0;
        lastsp=0;
        plinestart=c;
        while ((*c!=0)&&(*c!=13)&&(wid<((_width*16)+1)))
        {
           if (*c>=32) wid=wid+(pcurrentfont->w[(*c)-32])+1;
           if (*c==32) lastsp=c; //remember the pos of most recent space
           c++;
        }
        if (wid>(_width*16)) //line ran over
        {
           if (lastsp!=0) //there was a space on the line
           {
              len=lastsp-plinestart;
              memcpy(out,plinestart,len);
              out+=len;
              *out++=13;
              *out++=10;
              c=lastsp+1;
           }
           else //there wasn't a space, split the word with a -
           {
              len=c-plinestart-2;
              memcpy(out,plinestart,len);
              out+=len;
              *out++='-';
              *out++=13;
              *out++=10;
              c-=2;
           }
        }
        else //line fits, copy the line
        {
            len=c-plinestart;
            memcpy(out,plinestart,len+1);
            out+=len+1;
            if (*c==0) *out=0;
            if (*c==13) 
            {
              c++;
              *out++=10;
              if (*c==10) c++;
            }
        }

    }
    *out++=0; //term null
}

//----------------------------------------------------------------------------
//!  putstr
//----------------------------------------------------------------------------
void putstr(char *c)
{
    char wrapbuf[256];
    if (wordwrap!=0)
    {
      wrap(c,wrapbuf);
      c=wrapbuf;
    }
    while (*c!=0)
    {
       if (*c==13) //carriage return
       {
          c++;
          if (align==ALIGN_CENTRE)
            xp=(_width*16/2)-(linewidth(c)/2);
          else if (align==ALIGN_RIGHT)
            xp=_width*16-linewidth(c);
          else
            xp=0; 
       }
       else if (*c==10) 
       {
          yp=yp+pcurrentfont->ht+linespacing; 
          c++;

       }
       else opputc(*c++);
    }
}

//----------------------------------------------------------------------------
//!  putstr with alignment
//----------------------------------------------------------------------------
void putstr_align(char *c)
{
    char wrapbuf[256];
    if (wordwrap!=0)
    {
      wrap(c,wrapbuf);
      c=wrapbuf;
    }

    if (align==ALIGN_CENTRE)
        xp=xp-(linewidth(c)/2);
    else if (align==ALIGN_RIGHT)
        xp=xp-linewidth(c);

    if (valign==VALIGN_CENTRE)
        yp=(_height*8/2)-(stringheight(c)/2);
    else if (valign==VALIGN_BOTTOM)
        yp=_height*8-stringheight(c);
    //else
    //    yp=0; */

    while (*c!=0)
    {
       if (*c==13) //carriage return
       {
          c++;
          if (align==ALIGN_CENTRE)
            xp=xp-(linewidth(c)/2);
          else if (align==ALIGN_RIGHT)
            xp=xp-linewidth(c);
       }
       else if (*c==10) 
       {
          yp=yp+pcurrentfont->ht+linespacing; 
          c++;

       }
       else opputc(*c++);
    }
}


//----------------------------------------------------------------------------
//!  setxy
//----------------------------------------------------------------------------
void setxy(int x, int y)
{
    xp=x;
    yp=y;
}

//----------------------------------------------------------------------------
//!  call init function
//----------------------------------------------------------------------------
void TftInit(void)
{
  tft_init();
  setRotation(1);
}

//---------------------------------------------------------
//! display logo
//---------------------------------------------------------
void LogoDisplay(void)
{
  int x,y,txp,typ;
  uint16_t *dp;

  /*dp=(uint16_t *)&logobw[0];
  txp=38;
  typ=35; //ystart pos

  for (y=0; y<42; y++) //bitmap height
  {
    for (x=0; x<46; x++) //bitmap must be full width of screen 160px
    {    
      drawPixel(x+txp,y+typ,*dp++);
    }
  }
*/
}

//---------------------------------------------------------
//! display symbol
//---------------------------------------------------------
void SymbolDisplay(int px, int py, char v)
{
   const uint16_t *p;
   int x,y;
   char d;
   uint16_t fg;


   fg=color565((colour>>16)&0xFF,(colour>>8)&0xFF,colour&0xFF);


   if (v==0) p=arrowup;
   if (v==1) p=arrowdown;
   if (v==2) p=exclaim;
   if (v==3) p=info;
   if (v==4) p=rightarrow;
   if (v==5) p=downarrow;
   if (v==6) p=checkbox;
   if (v==7) p=checkboxtick;
   if (v==8) p=dot;

  setWindow(px,py,px+15,py+15);
  for (y=0; y<16; y++) //bitmap height
  {
    for (x=0; x<16; x++) //bitmap width
    {    
      if ((*p)&(0x8000>>x)) drawPixel(x+px,y+py,fg); else drawPixel(x+px,y+py,0);
    }
    p++;
  }


}

//---------------------------------------------------------
//! display bitmap symbol
//
//! use vb_bitmap app (on gitea) to make these from the bmp files
//---------------------------------------------------------
void BitmapDisplay(uint8_t n, uint32_t txp, uint32_t typ, uint8_t overlay)
{
  int w,h,x,y,i;
  const uint16_t *dp=0;
  uint16_t val, run;

  /*if (n==0) dp=bitmap_menu;
  else if (n==1) dp=bitmap_left;
  else if (n==2) dp=bitmap_right;
  else if (n==3) dp=bitmap_pwr;
  else if (n==4) dp=bitmap_dotselect;
  else if (n==5) dp=bitmap_dotnotselect;
  else if (n==6) dp=bitmap_colselect_text;
  #ifndef COLOR_PALETTE_WHITE
    else if (n==7) dp=bitmap_colpalette;
  #else
    else if (n==7) dp=bitmap_colorpalette_w;
  #endif
  else if (n==8) dp=bitmap_colselect_ring;
  else if (n==9) dp=bitmap_coltune_text;
  else if (n==10) dp=bitmap_dimmer_text;
  else if (n==11) dp=bitmap_shade_text;
  else if (n==12) dp=bitmap_white_text;
  else if (n==13) dp=bitmap_soundtolight_text;
  else if (n==14) dp=bitmap_autoscroll_text;
  else if (n==15) dp=bitmap_whitebar;
  else if (n==16) dp=bitmap_pointerdot;
  else if (n==17) dp=bitmap_autoscroll_icon;
  else if (n==18) dp=bitmap_soundtolight_icon;
  else if (n==19) dp=bitmap_offon_bar;
  else if (n==20) dp=bitmap_offon_dot;
  else if (n==21) dp=bitmap_offon_text;
  else if (n==22) dp=bitmap_slowfast_text;
  else if (n==23) dp=bitmap_sequence_text;
  else if (n==24) dp=bitmap_zone_text;
  else if (n==25) dp=bitmap_select_unselect;
  else if (n==26) dp=bitmap_dot_select_flash;*/

  
  //get bitmap width and height in first 2 words
  w=*dp++; h=*dp++;

  x=0; y=0;
  while (y<h) //bitmap height
  {
    if ((*dp==0x0020)&&(((*(dp+1))&0x8000)==0x8000)) //check for rle start marker
    {
         run=(*(dp+1))&0x7FFF; //marker is 0020 followed by 8xxx where x is length
         val=*(dp+2); //the value to repeat
         for (i=0; i<run; i++)
         {
            if (overlay==0)
              drawPixel(x+txp,y+typ,val);
            else
            {
              if (val>0) drawPixel(x+txp,y+typ,val);
            }
            x++; if (x>=w) {x=0; y++;}
         }
         dp+=3;
    }
    else //not rle, just output the pixel
    {
        if (overlay==0)
          drawPixel(x+txp,y+typ,*dp++);
        else
        {
          if (*dp>0) drawPixel(x+txp,y+typ,*dp++); else dp++;
        }
        x++; if (x>=w) {x=0; y++;}
    }
  }

}

  #define CBOXW 37
  #define CBOXH 80
  #define CBOXT 65
  #define VOLW 6
  #define PANW 15


  #define PANTHUMB 3

//---------------------------------------------------------
//! channel display routines
//---------------------------------------------------------
void ShowChanVolume(uint8_t ch, uint8_t v)
{
  uint8_t vv,x=0; 
  uint16_t vp=CBOXT,bgcol;
  if (ch>=8) {x=8; vp+=CBOXH+4;}
  vv=v*(CBOXH-20)/256;
  if (v==0) bgcol=color565(96,0,0); else bgcol=color565(96,96,96);
  fillRect(27+(ch-x)*(CBOXW+2),vp+17,VOLW,(CBOXH-20)-vv,bgcol); //the inactive volume
  fillRect(27+(ch-x)*(CBOXW+2),vp+17+(CBOXH-20-vv),VOLW,vv,color565(255,255,255)); //the active volume
}

void ShowChanBalance(uint8_t ch, uint8_t v)
{
  uint8_t vv,x=0; 
  uint16_t vp=CBOXT,bgcol;
  if (ch>=8) {x=8; vp+=CBOXH+4;}
  vv=v*PANW/256;
  fillRect(24+(ch-x)*(CBOXW+2),vp+5,PANW+PANTHUMB,4,color565(96,96,96)); //the full bar
  fillRect(24+(ch-x)*(CBOXW+2)+vv,vp+5,PANTHUMB,4,color565(255,255,255)); //the active pan blob
}

void ShowAudioLevel(uint8_t ch, uint8_t v)
{
  uint8_t vv,vr,vy,vg, x=0; 
  uint16_t vp=CBOXT;
  if (ch>=8) {x=8; vp+=CBOXH+4;}
  vv=v*(CBOXH-20)/256; //the total level
  if (v>192) vr=(v-192)*(CBOXH-20)/256; else vr=0;
  if (v>128) vy=(v-128)*(CBOXH-20)/256; else vy=0;
  if (vy>64*(CBOXH-20)/256) vy=64*(CBOXH-20)/256;
  vg=vv;
  if (vg>128*(CBOXH-20)/256) vg=128*(CBOXH-20)/256;
  fillRect(8+(CBOXW-VOLW-4)+(ch-x)*(CBOXW+2),vp+17,VOLW,(CBOXH-20)-vv,0); //blank the inactive level meter
  if (vr>0) fillRect(8+(CBOXW-VOLW-4)+(ch-x)*(CBOXW+2),vp+17+((CBOXH-20)*64/256-vr),VOLW,vr,color565(255,0,0)); //level red if above 75%
  if (vy>0) fillRect(8+(CBOXW-VOLW-4)+(ch-x)*(CBOXW+2),vp+17+((CBOXH-20)*128/256-vy),VOLW,vy,color565(255,255,0)); //level yellow if above 50%
  fillRect(8+(CBOXW-VOLW-4)+(ch-x)*(CBOXW+2),vp+17+(CBOXH-20-vg),VOLW,vg,color565(0,255,0)); //green below 50%
}

void ShowChanBox(uint8_t chan, uint8_t active)
{
  uint32_t tempcol=colour;
  char buf[8];
  uint8_t x=0;
  uint16_t v=CBOXT;
  textrotate=1;
  if (chan>7) {x=8; v+=CBOXH+4;}
  if (link[chan]==1) //channel paired with the next one. Draw an extra wide box
  {
    if (chan==active) colour=0xFF0000;    
    DrawRect(8+(chan-x)*(CBOXW+2),v,CBOXW*2+2,CBOXH);
    sprintf(buf,"%d:%s",chan+1,chname[chan]);
    setxy(9+(chan-x)*(CBOXW+2)+2,CBOXH+v-3);
    putstr(buf);
    colour=tempcol;
  }
  else 
  {    
    if ((chan==0)||(link[chan-1]==0)) //if chan is 0 or isnt linked with previous chan, draw the box
    {
      if (chan==active) colour=0xFF0000;
      DrawRect(8+(chan-x)*(CBOXW+2),v,CBOXW,CBOXH);
    }
    else
    {
      if ((chan-1)==active) colour=0xFF0000;
      setWindow(8+(chan-x)*(CBOXW+2),v,8+(chan-x)*(CBOXW+2)+CBOXW,v+CBOXH);
    }
    sprintf(buf,"%d:%s",chan+1,chname[chan]);
    setxy(9+(chan-x)*(CBOXW+2)+2,CBOXH+v-3);
    putstr(buf);
    colour=tempcol;
  }
  ShowChanVolume(chan,volume[chan]);
  x=pan[chan];
  if (link[chan]!=0) x=0;
  if ((chan!=0)&&(link[chan-1]!=0)) x=255;
  ShowChanBalance(chan,x);
  textrotate=0;
}




//---------------------------------------------------------
//! menu display routines
//---------------------------------------------------------

#define MENUBG 0x374B53
#define MENULINEBG 0x8BA6CA
#define MENUACTBG 0xF3EC84
#define MENUFGLIGHT 0xFFFFFF
#define MENUFGDARK 0x000000

void disp_menuclear(void)
{
    fillRect(0,0,128,160,color565((MENUBG>>16)&0xFF,(MENUBG>>8)&0xFF,MENUBG&0xFF));
    setxy(0,0);
}

void disp_putmenuline(char y, const char *s, char style)
{
   unsigned int bg, fg, oldbg=backcolour, oldfg=colour,w;
   unsigned char old_align=align;
   if (style&0x80) w=113; else w=128;
   style&=0x7F;
   if (style==0) {bg=MENUBG; fg=MENUFGLIGHT; align=ALIGN_LEFT;}
   else if (style==1) {bg=MENULINEBG; fg=MENUFGLIGHT; align=ALIGN_LEFT;}
   else if (style==2) {bg=MENUACTBG; fg=MENUFGDARK; align=ALIGN_LEFT;}
   fillRect(0,y*20,w,19,color565((bg>>16)&0xFF,(bg>>8)&0xFF,bg&0xFF));
   setxy(2,y*20+4);
   backcolour=bg; colour=fg;
   putstr_align((char *)s);
   backcolour=oldbg; colour=oldfg;
}

void disp_putmenuip(char y, char sel, uint32_t ip)
{
   unsigned int oldbg=backcolour, oldfg=colour,w;
   char buf[8];
   backcolour=MENULINEBG; colour=MENUFGLIGHT; 
   fillRect(0,y*20,128,19,color565((backcolour>>16)&0xFF,(backcolour>>8)&0xFF,backcolour&0xFF)); //draw full back box
   if (sel==0) {backcolour=MENUACTBG; colour=MENUFGDARK; fillRect(1,y*20,25,19,color565((backcolour>>16)&0xFF,(backcolour>>8)&0xFF,backcolour&0xFF));}
   sprintf(buf,"%03d",ip&0xFF);
   setxy(2,y*20+4);putstr(buf);
   backcolour=MENULINEBG; colour=MENUFGLIGHT;   
   setxy(27,y*20+4);putstr(".");
   if (sel==1) {backcolour=MENUACTBG; colour=MENUFGDARK; fillRect(30,y*20,25,19,color565((backcolour>>16)&0xFF,(backcolour>>8)&0xFF,backcolour&0xFF));}
   sprintf(buf,"%03d",(ip>>8)&0xFF);
   setxy(31,y*20+4);putstr(buf);
   backcolour=MENULINEBG; colour=MENUFGLIGHT;   
   setxy(56,y*20+4);putstr(".");
   if (sel==2) {backcolour=MENUACTBG; colour=MENUFGDARK; fillRect(59,y*20,25,19,color565((backcolour>>16)&0xFF,(backcolour>>8)&0xFF,backcolour&0xFF));}
   sprintf(buf,"%03d",(ip>>16)&0xFF);
   setxy(60,y*20+4);putstr(buf);
   backcolour=MENULINEBG; colour=MENUFGLIGHT;   
   setxy(87,y*20+4);putstr(".");
   if (sel==3) {backcolour=MENUACTBG; colour=MENUFGDARK; fillRect(90,y*20,25,19,color565((backcolour>>16)&0xFF,(backcolour>>8)&0xFF,backcolour&0xFF));}
   sprintf(buf,"%03d",(ip>>24)&0xFF);
   setxy(91,y*20+4);putstr(buf);
   
   backcolour=oldbg; colour=oldfg;
}


void disp_menu_scrollbar(char typ)
{
   unsigned int oldfg=colour;
   drawFastVLine(113, 20, 140, color565((MENUBG>>16)&0xFF,(MENUBG>>8)&0xFF,MENUBG&0xFF)); //separator line
   drawFastVLine(114, 20, 140, color565((MENUBG>>16)&0xFF,(MENUBG>>8)&0xFF,MENUBG&0xFF)); //separator line
   fillRect(115,20,16,131,color565((MENULINEBG>>16)&0xFF,(MENULINEBG>>8)&0xFF,MENULINEBG&0xFF));
   colour=MENUFGLIGHT;
   if (typ&1) SymbolDisplay(116,23,0); //up
   if (typ&2) SymbolDisplay(116,134,1); //down
   colour=oldfg;
}

