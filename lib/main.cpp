#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"

// Set pin 7 as the CS pin for the ArduCAM Mini 2MP Plus
#define CS_PIN 7

ArduCAM myCAM(OV2640, CS_PIN);

void setup() {
  uint8_t vid, pid;
  uint8_t temp;

  Wire.begin();
  Serial.begin(115200);
  Serial.println(F("ArduCAM Start!"));

  // Initialize the SPI interface
  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // Check if the camera module is detected
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  if (temp != 0x55) {
    Serial.println(F("SPI interface Error!"));
    while (1);
  }

  // Initialize the camera module
  myCAM.InitCAM();
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
  delay(1000);

  // Check if the camera module is OV2640
  myCAM.wrSensorReg8_8(0xff, 0x01);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
  if ((vid != 0x26) || (pid != 0x42)) {
    Serial.println(F("Can't find OV2640 module!"));
  } else {
    Serial.println(F("OV2640 detected."));
  }
}

void loop() {
  // Capture an image
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  myCAM.start_capture();
  Serial.println(F("Start capture."));

  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  Serial.println(F("Capture done."));

  // Read the image data from the FIFO
  uint8_t temp = 0, temp_last = 0;
  uint32_t length = myCAM.read_fifo_length();
  if (length >= MAX_FIFO_SIZE) {
    Serial.println(F("Over size."));
    return;
  }
  if (length == 0) {
    Serial.println(F("Size is 0."));
    return;
  }

  myCAM.CS_LOW();
  myCAM.set_fifo_burst();
  Serial.write(0xFF);
  Serial.write(0xD8);
  while (length--) {
    temp_last = temp;
    temp = SPI.transfer(0x00);
    if ((temp == 0xD9) && (temp_last == 0xFF)) {
      Serial.write(temp);
      break;
    }
    Serial.write(temp);
  }
  myCAM.CS_HIGH();
  Serial.println(F("Image capture done."));
  delay(1000);
}