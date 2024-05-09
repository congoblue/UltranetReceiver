#include "graphics.h"

uint8_t Saturate(uint16_t value, uint8_t max) {
  if (value>max) {
    return max;
  }else{
    return value;
  }
}

void UpdateFPGAAudioEngine(uint8_t channel) {
  // send volume for left and right for desired channel to fpga
  // channel=1-16, if 0 then set master

  uint32_t volume_left;
  uint32_t volume_right;
  uint8_t pv;
  uint8_t devchan; //correct for offset channels

  // value is between 0...255. Sent as 0..127 (all serial comms <128 except for start flag)

  if (channel == 0) {
    // main has only left or right, no balance
    volume_left = mainvol[0];
    volume_right = mainvol[1];
  }else{
    
    // channel 1..16
    if (link[channel-1]==0) //if this channel is not linked
      pv=pan[channel-1];
    else
    {
      if (link[channel-1]==1) pv=0; else pv=255; //fixed hard pan for linked channels
    }
    if ((solo==0xFF)||(solo==channel-1)||((link[channel-1]==2)&&(solo==channel-2))) //if not solo mode, or this channel solo
    {
      volume_left = volume[channel-1] * Saturate((255 - pv) * 2, 255) / 256;
      volume_right = volume[channel-1] * Saturate(pv * 2, 255) / 256;
    }
    else
    {
      volume_left=0; volume_right=0;
    }
    if (mute[channel-1]!=0)
    {
      volume_left=0; volume_right=0;
    }
  }

  //work out the actual audio channel on the fpga, taking offset into account
  //on x32, the chan we receive as ultranet ch1 may be ultranet ch3 or ch5 on the x32
  //so if we set offset=2, we want to actually adjust ch3 instead of ch1 (wrap at 8)
  if (channel==0) //master vol
  {
     devchan=0;
  }
  else
  {
    if (channel>8)
    {
      devchan=8+(((channel-8)-ChanOffset)&0x07);       
    }
    else
    {
      devchan=1+(((channel-1)-ChanOffset)&0x07);
    }
  }

  // send data to FPGA
  SendDataToFPGA(devchan, uint8_t(volume_left>>1));
  SendDataToFPGA(devchan + 17, uint8_t(volume_right>>1)); // offset of 17, because we have 16 channels + main
  //Serial.print(uint8_t(volume_left>>1));
  //Serial.print("<>");
  //Serial.println(uint8_t(volume_right>>1));
}