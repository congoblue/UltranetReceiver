#include <SPI.h>

/*************************************************************************************
Ultranet2SPDIF - Converter for Behringer Ultranet to SPDIF or other digital audio
v0.4.0 built on 04.09.2023
Dr.-Ing. Christian Nöding, christian@noeding-online.de
More infos at https://github.com/xn--nding-jua/UltranetReceiver
Destination-Hardware: Arduino MKR Vidor 4000 with SAMD21 Cortex M0+ 32-Bit Microcontroller

Used arduino libraries:
- Ticker by Stefan Staub

Optional libraries based on preprocessor-switches:
- Ethernet by Various 2.0.1 (https://github.com/arduino-libraries/Ethernet)
- I2C_EEPROM Library by Rob Tillaart (https://github.com/RobTillaart/I2C_EEPROM) or Sparkfun External EEPROM-Library https://github.com/sparkfun/SparkFun_External_EEPROM_Arduino_Library
- 24LC256 EEPROM Library by wvmarle (https://github.com/wvmarle/24LC256) for 24AA/24LC Series
- PubSubClient by Nick OReily


 +------------+------------------+--------+------+-----------------+--------+-----------------------+---------+---------+--------+--------+----------+----------+-------------------------+
 | Pin number |  MKR  Board pin  |  PIN   | FPGA | Notes           | Peri.A |     Peripheral B      | Perip.C | Perip.D | Peri.E | Peri.F | Periph.G | Periph.H | Usage for KDEE_HiResPG  |
 |            |                  |        |      |                 |   EIC  | ADC |  AC | PTC | DAC | SERCOMx | SERCOMx |  TCCx  |  TCCx  |    COM   | AC/GLCK  |                         |
 |            |                  |        |      |                 |(EXTINT)|(AIN)|(AIN)|     |     | (x/PAD) | (x/PAD) | (x/WO) | (x/WO) |          |          |                         |
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 | 00         | D0               |  PA22  | G1   |                 |  *06   |     |     | X10 |     |   3/00  |   5/00  |* TC4/0 | TCC0/4 |          | GCLK_IO6 | Ultranet Ch 1-8
 | 01         | D1               |  PA23  | N3   |                 |  *07   |     |     | X11 |     |   3/01  |   5/01  |* TC4/1 | TCC0/5 | USB/SOF  | GCLK_IO7 | Ultranet Ch 9-16
 | 02         | D2               |  PA10  | P3   |                 |   10   | *18 |     | X02 |     |   0/02  |   2/02  |*TCC1/0 | TCC0/2 | I2S/SCK0 | GCLK_IO4 | 
 | 03         | D3               |  PA11  | R3   |                 |   11   | *19 |     | X03 |     |   0/03  |   2/03  |*TCC1/1 | TCC0/3 | I2S/FS0  | GCLK_IO5 | SPDIF out
 | 04         | D4               |  PB10  | T3   |                 |  *10   |     |     |     |     |         |   4/02  |* TC5/0 | TCC0/4 | I2S/MCK1 | GCLK_IO4 | MCLK
 | 05         | D5               |  PB11  | T2   |                 |  *11   |     |     |     |     |         |   4/03  |* TC5/1 | TCC0/5 | I2S/SCK1 | GCLK_IO5 | LRCLK
 | 06         | D6               |  PA20  | G16  |                 |  *04   |     |     | X08 |     |   5/02  |   3/02  |        |*TCC0/6 | I2S/SCK0 | GCLK_IO4 | SCLK
 | 07         | D7               |  PA21  | G15  |                 |  *05   |     |     | X09 |     |   5/03  |   3/03  |        |*TCC0/7 | I2S/FS0  | GCLK_IO5 | SDATA
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 |            |       SPI        |        |      |                 |        |     |     |     |     |         |         |        |        |          |          | 
 | 08         | MOSI             |  PA16  | F16  |                 |  *00   |     |     | X04 |     |  *1/00  |   3/00  |*TCC2/0 | TCC0/6 |          | GCLK_IO2 | 
 | 09         | SCK              |  PA17  | F15  |                 |  *01   |     |     | X05 |     |  *1/01  |   3/01  | TCC2/1 | TCC0/7 |          | GCLK_IO3 | 
 | 10         | MISO             |  PA19  | C16  |                 |   03   |     |     | X07 |     |  *1/03  |   3/03  |* TC3/1 | TCC0/3 | I2S/SD0  | AC/CMP1  | 
 +------------+------------------+--------+------+-----------------+--------------------+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 |            |       Wire       |        |      |                 |        |     |     |     |     |         |         |        |        |          |          | 
 | 11         | SDA              |  PA08  | C15  |                 |   NMI  | *16 |     | X00 |     |  *0/00  |   2/00  | TCC0/0 | TCC1/2 | I2S/SD1  |          | 
 | 12         | SCL              |  PA09  | B16  |                 |   09   | *17 |     | X01 |     |  *0/01  |   2/01  | TCC0/1 | TCC1/3 | I2S/MCK0 |          | 
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 |            |      Serial1     |        |      |                 |        |     |     |     |     |         |         |        |        |          |          | 
 | 13         | RX               |  PB23  | C11  |                 |   07   |     |     |     |     |         |  *5/03  |        |        |          | GCLK_IO1 | -> FPGA TxD
 | 14         | TX               |  PB22  | A13  |                 |   06   |     |     |     |     |         |  *5/02  |        |        |          | GCLK_IO0 | -> FPGA RxD
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 | 15         | A0 / DAC0        |  PA02  | C2   |                 |   02   | *00 |     | Y00 | OUT |         |         |        |        |          |          | 
 | 16         | A1               |  PB02  | C3   |                 |  *02   | *10 |     | Y08 |     |         |   5/00  |        |        |          |          | 
 | 17         | A2               |  PB03  | C6   |                 |  *03   | *11 |     | Y09 |     |         |   5/01  |        |        |          |          | 
 | 18         | A3               |  PA04  | D1   |                 |   04   | *04 |  00 | Y02 |     |         |   0/00  |*TCC0/0 |        |          |          | 
 | 19         | A4               |  PA05  | D3   |                 |   05   | *05 |  01 | Y03 |     |         |   0/01  |*TCC0/1 |        |          |          | 
 | 20         | A5               |  PA06  | F3   |                 |   06   | *06 |  02 | Y04 |     |         |   0/02  | TCC1/0 |        |          |          | 
 | 21         | A6               |  PA07  | G2   |                 |   07   | *07 |  03 | Y05 |     |         |   0/03  | TCC1/1 |        | I2S/SD0  |          | 
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 |            |       USB        |        |      |                 |        |     |     |     |     |         |         |        |        |          |          | 
 | 22         |                  |  PA24  |      | USB N           |   12   |     |     |     |     |   3/02  |   5/02  |  TC5/0 | TCC1/2 | USB/DM   |          | 
 | 23         |                  |  PA25  |      | USB P           |   13   |     |     |     |     |   3/03  |   5/03  |  TC5/1 | TCC1/3 | USB/DP   |          | 
 | 24         |                  |  PA18  |      | USB ID          |   02   |     |     | X06 |     |   1/02  |   3/02  |  TC3/0 | TCC0/2 |          | AC/CMP0  | 
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 | 25         | AREF             |  PA03  | B1   |                 |   03   |  01 |     | Y01 |     |         |         |        |        |          |          | 
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 |            | FPGA             |        |      |                 |        |     |     |     |     |         |         |        |        |          |          | 
 | 26         |                  |  PA12  |      | FPGA TDI        |   12   |     |     |     |     |  *2/00  |   4/00  | TCC2/0 | TCC0/6 |          | AC/CMP0  | 
 | 27         |                  |  PA13  |      | FPGA TCK        |   13   |     |     |     |     |  *2/01  |   4/01  | TCC2/1 | TCC0/7 |          | AC/CMP1  | 
 | 28         |                  |  PA14  |      | FPGA TMS        |   14   |     |     |     |     |   2/02  |   4/02  |  TC3/0 | TCC0/4 |          | GCLK_IO0 | 
 | 29         |                  |  PA15  |      | FPGA TDO        |   15   |     |     |     |     |  *2/03  |   4/03  |  TC3/1 | TCC0/5 |          | GCLK_IO1 | 
 | 30         |                  |  PA27  |      | FPGA CLK        |   15   |     |     |     |     |         |         |        |        |          | GCLK_IO0 | 
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 | 31         |                  |  PA28  | L16  | FPGA MB INT     |   08   |     |     |     |     |         |         |        |        |          | GCLK_IO0 | 
 | 32         |                  |  PB08  |      | LED_RED_BUILTIN |   08   |  02 |     | Y14 |     |         |   4/00  |  TC4/0 |        |          |          | -> LED
 | 33         |                  |  PB09  |      | SAM_INT_OUT     |  *09   |  03 |     | Y15 |     |         |   4/01  |  TC4/1 |        |          |          | 
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+
 |            | 32768Hz Crystal  |        |      |                 |        |     |     |     |     |         |         |        |        |          |          | 
 | 34         |                  |  PA00  |      | XIN32           |   00   |     |     |     |     |         |   1/00  | TCC2/0 |        |          |          | 
 | 35         |                  |  PA01  |      | XOUT32          |   01   |     |     |     |     |         |   1/01  | TCC2/1 |        |          |          | 
 +------------+------------------+--------+------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-------------------------+

EEPROM-Mapping
+-------------+------------------+
| Address     | Description      |
+-------------+------------------+
| 00h ... 01h | Version          |
+-------------+------------------+
| 02h ... 05h | IP-Address       |
+-------------+------------------+
| 06h ... ... | unused           |
+-------------+------------------+
| FAh ... FFh | MAC-Address      |
+-------------+------------------+

**************************************************************************************/
// These define's must be placed at the beginning before #include "SAMDTimerInterrupt.h"
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Select only one to be true for SAMD21. Must must be placed at the beginning before #include "SAMDTimerInterrupt.h"
#define USING_TIMER_TC3         true      // Only TC3 can be used for SAMD51
#define USING_TIMER_TC4         false     // Not to use with Servo library
#define USING_TIMER_TC5         false
#define USING_TIMER_TCC         false
#define USING_TIMER_TCC1        false
#define USING_TIMER_TCC2        false     // Don't use this, can crash on some boards

#define EEPROM_EMULATION_SIZE     1024

#include "Controller.h"
#include "graphics.h"
#include "SAMDTimerInterrupt.h"
#include <FlashStorage_SAMD.h>

#define SELECTED_TIMER      TIMER_TC3
SAMDTimer ITimer(SELECTED_TIMER);


uint8_t activechan=0;
uint8_t UltranetPrev=0;
uint8_t PeakLevel[17];
uint8_t EncMode=0;

void TimerSecondsFcn() {
  // toggle LED to show, that we are alive
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

Ticker TimerSeconds(TimerSecondsFcn, 1000, 0, MILLIS);

void TimerHandler()
{
  static uint8_t x;
  //sense encoder and buttons
  SenseEncoder();

  x++;
  if (x>50)
  {
    x=0;
    SenseKeys();
  }
}

//--- recall eeprom settings. Init if not previously set
void EepromRecall(void)
{
  uint8_t i;
  if (EEPROM.read(EEP_SIG)!=0xA5) //not setup - write defaults
  {
      EEPROM.write(EEP_SIG,0xA5);
      Serial.println("Eeprom set defaults");
      for (i=0; i<2; i++)
      {
        mainvol[i]=255;
        EEPROM.write(EEP_MAINL+i,mainvol[i]);
      }
      for (i=0; i<16; i++)
      {        
        volume[i]=127;
        EEPROM.write(EEP_VOL+i,volume[i]);
        pan[i]=127;
        EEPROM.write(EEP_PAN+i,pan[i]);
        link[i]=0;
        EEPROM.write(EEP_LINK+i,link[i]);
      }
      EEPROM.commit();
  }

  for (i=0; i<2; i++)
  {
    mainvol[i]=EEPROM.read(EEP_MAINL+i);
    UpdateFPGAAudioEngine(0); // send main to FPGA
  }

  Serial.print("V:");  
  for (i=0; i<16; i++)
  {
    volume[i]=EEPROM.read(EEP_VOL+i);
    pan[i]=EEPROM.read(EEP_PAN+i);
    link[i]=EEPROM.read(EEP_LINK+i);
    mute[i]=0; //don't save mute state
    Serial.print(volume[i]); Serial.print(" ");   
    UpdateFPGAAudioEngine(i+1); // send values to FPGA
  }
}

//--- compare eeprom settings with current, store if different
void EepromUpdate(void)
{
  static uint32_t lastupdate=millis(); //protect against update too often
  uint8_t i, change=0;

  if ((millis()-lastupdate)<60000) return; //don't update if less than 60s
  for (i=0; i<2; i++)
  {
    if (mainvol[i]!=EEPROM.read(EEP_MAINL+i)) {change=1; EEPROM.write(EEP_MAINL+i,mainvol[i]);}
  }
  for (i=0; i<16; i++)
  {        
    if (volume[i]!=EEPROM.read(EEP_VOL+i)) {change=1; EEPROM.write(EEP_VOL+i,volume[i]);}
    if (pan[i]!=EEPROM.read(EEP_PAN+i)) {change=1; EEPROM.write(EEP_PAN+i,pan[i]); }
    if (link[i]!=EEPROM.read(EEP_LINK+i)) {change=1; EEPROM.write(EEP_LINK+i,link[i]);}
  }
  if (change!=0) 
  {
    EEPROM.commit();
    Serial.println("Eeprom write");
    lastupdate=millis();
  }
}

// ******************** SETUP FUNCTION ********************
void setup() {
  // general pin-defintions
  pinMode(LED_BUILTIN, OUTPUT);

  //rotary encoder pins
  pinMode(A4,INPUT_PULLUP);
  pinMode(A5,INPUT_PULLUP);
  pinMode(A6,INPUT_PULLUP);

  //switch pins
  pinMode(A0,INPUT_PULLUP);
  pinMode(A1,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);
  pinMode(A3,INPUT_PULLUP);

  //initialise display
  TftInit();
  backcolour=0;
  colour=0xFFFFFF;
  align=ALIGN_CENTRE;
  opclrscr(); 

  ITimer.attachInterruptInterval_MS(2, TimerHandler);

  // load bitstream to FPGA and bring it up
	setup_fpga();

  // initialize UARTs
  Serial.begin(19200); // initialize USB-Serial
  Serial.setTimeout(1000); // Timeout for commands
  Serial1.begin(19200); // initialize Hardware-UART for communication with FPGA

  EepromRecall();

  #if UseEthernet == 1
    // initialize ethernet via W5500
    InitEthernet();

    #if UseMQTT == 1
      // connect to MQTT-server
      MQTT_init();
    #endif
  #endif

  //initial display
  setfont(12,0);
  setxy(22,5);
  putstr("Ultranet");
  colour=0xFF0000;
  SymbolDisplay(2, 2, 8);
  colour=0xFFFFFF;
  //link[0]=1; link[1]=2; link[2]=1; link[3]=2; //debug... fixed links
  for (uint8_t i=0; i<16; i++)
  {
    ShowChanBox(i,activechan);
    ShowChanVolume(i,volume[i]);
    ShowChanBalance(i,pan[i]);
  }
  ShowSoftKeys();
  ShowMasterVolume(mainvol[0],0);

  activechan=0;
  EncValue=volume[activechan];

  // Start timers
  TimerSeconds.start();

}

// ******************** MAIN FUNCTION ********************
void loop() {
  uint32_t audioupdatetime=0;
  uint32_t eepromupdatetime=millis()+15000;
  uint8_t x,v;
  char buf[8];
  static uint8_t lk=0;
  #if UseEthernet == 1
    // handle ethernet clients
    HandleHTTPClients();
    HandleCMDClients();

    #if UseMQTT == 1
      HandleMQTT();
    #endif
  #endif

  // handle communication
  HandleUSBCommunication();
  HandleFPGACommunication();

  // update timer
  TimerSeconds.update();

  //update audio meters
  if ((millis()-audioupdatetime)>100)
  {
     for (x=0; x<16; x++)
     {
        //v=random(255); if (v>level[x]) level[x]=v; //random values for testing
        if (PeakLevel[x]>level[x]) level[x]=PeakLevel[x];
        ShowAudioLevel(x,level[x]);
        if (level[x]>8) level[x]-=8; else level[x]=0; //decay
     }
     SendDataToFPGA(127, 0); //request next set of metering data (cmd 127)

     if (UltranetPrev!=UltranetGood) //update ultranet status dot if changed
     {
        UltranetPrev=UltranetGood;
        if (UltranetGood==0x55) colour=0x00FF00; else colour=0xFF0000;
        SymbolDisplay(2, 2, 8);
        colour=0xFFFFFF;
     }
  }

  //check if any changes to store to eeprom
  if ((millis()-eepromupdatetime)>10000) //10sec
  { 
      eepromupdatetime=millis();
      EepromUpdate();  //check if anything is different and store if so
  }

  //process key presses
  if (KeyHit)
  {
     KeyHit=0;
     if ((LastKey==3)||(LastKey==4)) //left/right
     {
      if (KeyState==0x13) //lr held
      {        
        ShowChanBox(activechan,0xFF); //erase select mode on the chan we are leaving
        activechan=lk;
        if (activechan&1) activechan--;
        if (link[activechan]==0)
        {
          link[activechan]=1; link[activechan+1]=2;
          EraseChanBox(activechan); //erase the middle area that's different when you have 2 boxes
          ShowChanBox(activechan, activechan); 
          ShowChanBalance(activechan,pan[activechan]);
        }
        else 
        {
          link[activechan]=0; link[activechan+1]=0;
          EraseChanBox(activechan);
          ShowChanBox(activechan, activechan); 
          ShowChanBox(activechan+1, activechan); 
          ShowChanBalance(activechan,pan[activechan]);
          ShowChanBalance(activechan+1,pan[activechan+1]);
        }
      }
      else
      {
        lk=activechan; //remember what the active chan was, in case another arrow is pressed to link
        ShowChanBox(activechan,0xFF); //erase select mode on the chan we are leaving
        if (LastKey==3) {if (link[activechan]!=0) activechan+=2; else activechan++; if (activechan>15) activechan=0;}
        if (LastKey==4) {activechan--; if (activechan>15) activechan=15; if (link[activechan]!=0) activechan--;}      
        ShowChanBox(activechan, activechan); //show new chan selected
        if (EncMode==0) EncValue=volume[activechan];
        if (EncMode==1) EncValue=pan[activechan];
        //Serial.print("Act:"); Serial.print(activechan); Serial.print(" Lk:"); Serial.println(link[activechan]);
      }

     }
     if (LastKey==2) 
     {
       if ((solo==0xFF)||((solo!=0xFF)&&(solo!=activechan))) //no solo currently, or solo on different channel
       {
          if (solo!=0xFF) {x=solo; solo=0xFF; ShowChanBox(x, 0xFF);} //erase solo mode on old chan
          solo=activechan;
          if (mute[activechan]) mute[activechan]=0;
          if (link[activechan]==1) mute[activechan+1]=0; //unmute solo channel if muted
          ShowChanBox(activechan,activechan);
          ShowChanVolume(activechan,volume[activechan]);
          ShowChanBalance(activechan,pan[activechan]);
       }
       else
       {
          solo=0xFF;
          ShowChanBox(activechan,activechan);          
       }
       for (x=0; x<16; x++) UpdateFPGAAudioEngine(x+1);
     } 
     if (LastKey==1)
     {
        EncMode++; if (EncMode==3) EncMode=0;
        if (EncMode==0) {SetSoftkeyText(3," Volume "); ShowChanBox(activechan,activechan); ShowMasterVolume(mainvol[0],0); EncValue=volume[activechan];}
        if (EncMode==1) {SetSoftkeyText(3,"  Pan  "); EncValue=pan[activechan];}
        if (EncMode==2) {SetSoftkeyText(3,"Master"); ShowChanBox(activechan,0xFF); ShowMasterVolume(mainvol[0],1); EncValue=mainvol[0];}
        ShowSoftKeys();
     }
     if (LastKey==5) //encoder press (mute)
     {
        if (mute[activechan]==0) mute[activechan]=1; else mute[activechan]=0;
        if (link[activechan]==1) {if (mute[activechan+1]==0) mute[activechan+1]=1; else mute[activechan+1]=0;} //also mute linked channel
        if (solo==activechan) solo=0xFF; //cancel solo if mute
        ShowChanBox(activechan,activechan);
        ShowChanVolume(activechan,volume[activechan]);
        ShowChanBalance(activechan,pan[activechan]);
        UpdateFPGAAudioEngine(activechan+1);
        if (link[activechan]==1) UpdateFPGAAudioEngine(activechan+2);
     }
  }
  if (KeyState==0x1F) lk=0; //forget last key if nothing pressed

  //process encoder change
  if (EncChange)
  {
    EncChange=0;
    if (EncMode==2)
    {
       mainvol[0]=EncValue;
       mainvol[1]=EncValue;
       ShowMasterVolume(mainvol[0],1);
       UpdateFPGAAudioEngine(0);
    }
    else if (link[activechan]==0) //current chan is not linked or master mode
    {
      if (mute[activechan]) mute[activechan]=0;
      if (EncMode==0) volume[activechan]=EncValue; 
      if (EncMode==1) pan[activechan]=EncValue; 
      ShowChanVolume(activechan,volume[activechan]);
      ShowChanBalance(activechan,pan[activechan]);
      UpdateFPGAAudioEngine(activechan+1);
    }
    else //current chan is linked to the next one (the activechan will always be the first of a linked pair)
    {
      if (mute[activechan]) {mute[activechan]=0; mute[activechan+1]=0;}
      if (EncMode==0) {
      volume[activechan]=EncValue;
      volume[activechan+1]=EncValue;
      ShowChanVolume(activechan,volume[activechan]);
      ShowChanBalance(activechan,pan[activechan]);
      } //pan cannot be changed for paired channels
      UpdateFPGAAudioEngine(activechan+1);
      UpdateFPGAAudioEngine(activechan+2);
    }
  }


}
