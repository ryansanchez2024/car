// // ==================================================================================================




// #include "ArducamLink.h"
// #include "Arducam_Mega.h"
// #include "SPI.h"


// const int MEGA_CS = 7;
// Arducam_Mega myCAM(MEGA_CS);
// ArducamLink myUart;


// unsigned long lastCaptureTime = 0;
// const unsigned long captureInterval = 10000; // 10 seconds in milliseconds


// void captureAndSendImage();


// void setup() {
//    SPI.begin();
//    myUart.arducamUartBegin(115200);
//    myCAM.begin();


//    delay(1000);
//    // myUart.arducamUartWrite(0xFF);
//    // myUart.arducamUartWrite(0xAA);
//    // myUart.arducamUartWrite(0x01);
//    // myUart.printf("Start marker sent\n");
//    captureAndSendImage();
// }


// void loop() {
//    // unsigned long currentTime = millis();


//    // if (currentTime - lastCaptureTime >= captureInterval) {
//    //     myUart.printf("Time to capture image\n");
//    //     captureAndSendImage();
//    //     lastCaptureTime = currentTime;
//    // }
// }


// void captureAndSendImage() {
//    // Send start marker
//    myUart.arducamUartWrite(0xFF);
//    myUart.arducamUartWrite(0xAA);
//    myUart.arducamUartWrite(0x01);
//    myUart.printf("Start marker sent\n");


//    myCAM.takePicture(CAM_IMAGE_MODE_UXGA, CAM_IMAGE_PIX_FMT_JPG);


//    // Send image size
//    uint32_t imageSize = myCAM.getTotalLength();
//    myUart.arducamUartWrite((uint8_t)(imageSize & 0xFF));
//    myUart.arducamUartWrite((uint8_t)((imageSize >> 8) & 0xFF));
//    myUart.arducamUartWrite((uint8_t)((imageSize >> 16) & 0xFF));
//    myUart.arducamUartWrite((uint8_t)((imageSize >> 24) & 0xFF));


//    char msgBuffer[64];
//    sprintf(msgBuffer, "Image size sent: %lu bytes\n", imageSize);
//    myUart.printf(msgBuffer);


//    // Send image data
//    uint8_t imgBuffer[256];
//    uint32_t sentBytes = 0;
//    while (imageSize > 0) {
//        uint16_t bytesToRead = min(256, imageSize);
//        myCAM.readBuff(imgBuffer, bytesToRead);
//        myUart.arducamUartWriteBuff(imgBuffer, bytesToRead);
//        imageSize -= bytesToRead;
//        sentBytes += bytesToRead;
//        if (sentBytes % 1024 == 0) {
//            sprintf(msgBuffer, "Sent %lu bytes\n", sentBytes);
//            myUart.printf(msgBuffer);
//        }
//    }
//    sprintf(msgBuffer, "Image data sent: %lu bytes\n", sentBytes);
//    myUart.printf(msgBuffer);
  
//    // Send end marker
//    myUart.arducamUartWrite(0xFF);
//    myUart.arducamUartWrite(0xBB);
//    myUart.printf("End marker sent\n");
// }








// ==================================================================================================







// #include "ArducamLink.h"
// #include "Arducam_Mega.h"
// #include "SPI.h"

// const int MEGA_CS = 7;
// Arducam_Mega myCAM(MEGA_CS);
// ArducamLink myUart;

// unsigned long lastCaptureTime = 0;
// const unsigned long captureInterval = 10000; // 10 seconds in milliseconds

// // Function declarations
// void captureAndSendImage();

// void setup()
// {
//     Serial.begin(115200);  // Initialize serial for debugging
//     SPI.begin();
//     myUart.arducamUartBegin(115200);
//     myUart.send_data_pack(7, "Hello Arduino UNO!");
//     myCAM.begin();
//     myUart.send_data_pack(8, "Mega start!");
//     Serial.println("Setup complete");
// }

// void loop()
// {
//     unsigned long currentTime = millis();

//     if (currentTime - lastCaptureTime >= captureInterval) {
//         captureAndSendImage();
//         lastCaptureTime = currentTime;
//     }
//     myCAM.captureThread();
// }


// void captureAndSendImage()
// {
//     myCAM.takePicture(CAM_IMAGE_MODE_UXGA, CAM_IMAGE_PIX_FMT_JPG);

//     // Send start marker
//     myUart.arducamUartWrite(0xFF);
//     myUart.arducamUartWrite(0xAA);
//     myUart.arducamUartWrite(0x01);
//     Serial.println("Start marker sent");

//     // Send image size
//     uint32_t imageSize = myCAM.getTotalLength();
//     myUart.arducamUartWrite((uint8_t)(imageSize & 0xFF));
//     myUart.arducamUartWrite((uint8_t)((imageSize >> 8) & 0xFF));
//     myUart.arducamUartWrite((uint8_t)((imageSize >> 16) & 0xFF));
//     myUart.arducamUartWrite((uint8_t)((imageSize >> 24) & 0xFF));
//     Serial.print("Image size: ");
//     Serial.println(imageSize);

//     // Send image data
//     uint8_t buffer[256];
//     while (imageSize > 0) {
//         uint16_t bytesToRead = min(256, imageSize);
//         myCAM.readBuff(buffer, bytesToRead);
//         myUart.arducamUartWriteBuff(buffer, bytesToRead);
//         imageSize -= bytesToRead;
//     }

//     // Send end marker
//     myUart.arducamUartWrite(0xFF);
//     myUart.arducamUartWrite(0xBB);
//     Serial.println("End marker sent");
// }





// ==================================================================================================



// #include "ArducamLink.h"
// #include "Arducam_Mega.h"
// #include "SPI.h"

// ArducamLink myUart;  // Declare myUart globally

// void setup()
// {
//     Serial.begin(115200);  // Initialize serial for debugging
//     myUart.arducamUartBegin(115200);
//     Serial.println("Arduino setup complete");
// }

// void loop()
// {
//     // Send start marker
//     myUart.arducamUartWrite(0xFF);
//     myUart.arducamUartWrite(0xAA);
//     myUart.arducamUartWrite(0x01);
//     // Serial.println("Start marker sent");

//     // Send payload (structured data)
//     uint8_t payload[] = {0x01, 0x02, 0x03, 0x04, 0x05};  // Example payload
//     myUart.arducamUartWriteBuff(payload, sizeof(payload));
//     Serial.println("Payload sent");

//     // Send end marker
//     myUart.arducamUartWrite(0xFF);
//     myUart.arducamUartWrite(0xBB);
//     Serial.println("End marker sent");

//     delay(2000);  // Send every 2 seconds
// }



// ==================================================================================================







// // Copyright 2021 Arducam Technology co., Ltd. All Rights Reserved.
// // License: MIT License (https://en.wikipedia.org/wiki/MIT_License)
// // Web: http://www.ArduCAM.com
// // This program is a demo of how to use most of the functions
// // of the library with ArduCAM Spi camera, and can run on Arduino platform.
// // This demo was made for ArduCAM Spi Camera.
// // It needs to be used in combination with PC software.
// // It can test ArduCAM Spi Camerafunctions
// #include "ArducamLink.h"
// #include "Arducam_Mega.h"
// #include "SPI.h"
// const int MEGA_CS = 7;
// Arducam_Mega myCAM(MEGA_CS);
// ArducamLink myUart;
// uint8_t temp             = 0xff;
// uint8_t sendFlag         = TRUE;
// uint8_t commandBuff[20]  = {0};
// uint8_t commandLength    = 0;
// uint32_t readImageLength = 0;
// uint8_t jpegHeadFlag     = 0;


// unsigned long lastCaptureTime = 0;
// const unsigned long captureInterval = 10000; // 10 seconds in milliseconds


// uint8_t readBuffer(uint8_t* imagebuf, uint8_t length)
// {
//    if (imagebuf[0] == 0xff && imagebuf[1] == 0xd8) {
//        jpegHeadFlag    = 1;
//        readImageLength = 0;
//        myUart.arducamUartWrite(0xff);
//        myUart.arducamUartWrite(0xAA);
//        myUart.arducamUartWrite(0x01);


//        myUart.arducamUartWrite((uint8_t)(myCAM.getTotalLength() & 0xff));
//        myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 8) & 0xff));
//        myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 16) & 0xff));
//        myUart.arducamUartWrite((uint8_t)((myCAM.getTotalLength() >> 24) & 0xff));
//        myUart.arducamUartWrite(((CAM_IMAGE_PIX_FMT_JPG & 0x0f) << 4) | 0x01);
//    }
//    if (jpegHeadFlag == 1) {
//        readImageLength += length;
//        for (uint8_t i = 0; i < length; i++) {
//            myUart.arducamUartWrite(imagebuf[i]);
//        }
//    }
//    if (readImageLength == myCAM.getTotalLength()) {
//        jpegHeadFlag = 0;
//        myUart.arducamUartWrite(0xff);
//        myUart.arducamUartWrite(0xBB);
//    }
//    return sendFlag;
// }


// void stop_preivew()
// {
//    readImageLength = 0;
//    jpegHeadFlag    = 0;
//    uint32_t len    = 9;


//    myUart.arducamUartWrite(0xff);
//    myUart.arducamUartWrite(0xBB);
//    myUart.arducamUartWrite(0xff);
//    myUart.arducamUartWrite(0xAA);
//    myUart.arducamUartWrite(0x06);
//    myUart.arducamUartWriteBuff((uint8_t*)&len, 4);
//    myUart.printf("streamoff");
//    myUart.arducamUartWrite(0xff);
//    myUart.arducamUartWrite(0xBB);
// }


// void setup()
// {
//    SPI.begin();
//    myUart.arducamUartBegin(115200);
//    myUart.send_data_pack(7, "Hello Arduino UNO!");
//    myCAM.begin();
//    myUart.send_data_pack(8, "Mega start!");
//    myCAM.registerCallBack(readBuffer, 200, stop_preivew);
// }


// void loop()
// {
//    unsigned long currentTime = millis();


//    if (currentTime - lastCaptureTime >= captureInterval) {
//        captureAndSendImage();
//        lastCaptureTime = currentTime;
//    }


//    if (myUart.arducamUartAvailable()) {
//        temp = myUart.arducamUartRead();
//        delay(5);
//        if (temp == 0x55) {
//            while (myUart.arducamUartAvailable()) {
//                commandBuff[commandLength] = myUart.arducamUartRead();
//                if (commandBuff[commandLength] == 0xAA) {
//                    break;
//                }
//                commandLength++;
//            }
//            myUart.arducamFlush();
//            myUart.uartCommandProcessing(&myCAM, commandBuff);
//            commandLength = 0;
//        }
//    }
//    myCAM.captureThread();
// }


// void captureAndSendImage()
// {
//    myCAM.takePicture(CAM_IMAGE_MODE_UXGA, CAM_IMAGE_PIX_FMT_JPG);
  
//    // Send start marker
//    myUart.arducamUartWrite(0xFF);
//    myUart.arducamUartWrite(0xAA);
//    myUart.arducamUartWrite(0x01);


//    // Send image size
//    uint32_t imageSize = myCAM.getTotalLength();
//    myUart.arducamUartWrite((uint8_t)(imageSize & 0xFF));
//    myUart.arducamUartWrite((uint8_t)((imageSize >> 8) & 0xFF));
//    myUart.arducamUartWrite((uint8_t)((imageSize >> 16) & 0xFF));
//    myUart.arducamUartWrite((uint8_t)((imageSize >> 24) & 0xFF));


//    // Send image data
//    uint8_t buffer[256];
//    while (imageSize > 0) {
//        uint16_t bytesToRead = min(256, imageSize);
//        myCAM.readBuff(buffer, bytesToRead);
//        myUart.arducamUartWriteBuff(buffer, bytesToRead);
//        imageSize -= bytesToRead;
//    }


//    // Send end marker
//    myUart.arducamUartWrite(0xFF);
//    myUart.arducamUartWrite(0xBB);
// }
