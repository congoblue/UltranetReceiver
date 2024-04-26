#include "graphics.h"

uint8_t Saturate(uint16_t value, uint8_t max) {
  if (value>max) {
    return max;
  }else{
    return value;
  }
}

void UpdateFPGAAudioEngine(uint8_t channel) {
  // send volume for left and right for desired channel

  uint32_t volume_left;
  uint32_t volume_right;
  uint8_t pv;

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

  // send data to FPGA
  SendDataToFPGA(channel, uint8_t(volume_left>>1));
  SendDataToFPGA(channel + 17, uint8_t(volume_right>>1)); // offset of 17, because we have 16 channels + main
  //Serial.print(uint8_t(volume_left>>1));
  //Serial.print("<>");
  //Serial.println(uint8_t(volume_right>>1));
}