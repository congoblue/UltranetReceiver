uint8_t Saturate(uint16_t value, uint8_t max) {
  if (value>max) {
    return max;
  }else{
    return value;
  }
}

void InitAudiomixer() {
  audiomixer.mainVolumeLeft = 255; // set to 100%
  audiomixer.mainVolumeRight = 255; // set to 100%
  UpdateFPGAAudioEngine(0); // send main to FPGA

  uint8_t i;
  for (i=1; i<=16; i++) {
    audiomixer.chVolume[i-1] = 127; // set to 0%
    audiomixer.chBalance[i-1] = 127; // bring to center
    UpdateFPGAAudioEngine(i); // send values to FPGA
  }
}

void UpdateFPGAAudioEngine(uint8_t channel) {
  // send volume for left and right for desired channel

  uint32_t volume_left;
  uint32_t volume_right;

  // value is between 0...100. We will change this value to meet 8bit = 0..256 to make calculation in FPGA a bit easier
  // within FPGA we will do an integer-calculation like: ((AudioSampleData * ReceivedValueFromMicrocontroller) >> 8) = DataForDAC

  if (channel == 0) {
    // main has only left or right, no balance
    volume_left = audiomixer.mainVolumeLeft;
    volume_right = audiomixer.mainVolumeRight;
  }else{
    // channel 1..16
    volume_left = audiomixer.chVolume[channel-1] * Saturate((255 - audiomixer.chBalance[channel-1]) * 2, 255) / 256;
    volume_right = audiomixer.chVolume[channel-1] * Saturate(audiomixer.chBalance[channel-1] * 2, 255) / 256;
  }

  // send data to FPGA
  SendDataToFPGA(channel, uint8_t(volume_left>>1));
  SendDataToFPGA(channel + 17, uint8_t(volume_right>>1)); // offset of 17, because we have 16 channels + main
  //Serial.print(uint8_t(volume_left>>1));
  //Serial.print("<>");
  //Serial.println(uint8_t(volume_right>>1));
}