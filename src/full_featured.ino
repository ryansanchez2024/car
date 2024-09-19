/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will print the IP address of your WiFi module (once connected)
  to the Serial Monitor. From there, you can open that address in a web browser
  to turn on and off the LED_BUILTIN.

  If the IP address of your board is yourAddress:
  http://yourAddress/H turns the LED on
  http://yourAddress/L turns it off

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the WiFi.begin() call accordingly.

  Circuit:
  * Board with NINA module (Arduino MKR WiFi 1010, MKR VIDOR 4000 and Uno WiFi Rev.2)
  * LED attached to pin 9

  created 25 Nov 2012
  by Tom Igoe

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#simple-webserver
 */

#include "WiFiS3.h"

#include "arduino_secrets.h"               // your network key index number (needed only for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out to the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<p style=\"font-size:7vw;\">Click <a href=\"/H\">here</a> turn the LED on<br></p>");
            client.print("<p style=\"font-size:7vw;\">Click <a href=\"/L\">here</a> turn the LED off<br></p>");
            
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
      
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}





// #include "WiFiS3.h"
// #include <WiFi.h>

// #include "Arducam_Mega.h"
// #include "SPI.h"
// #include "ArducamLink.h"
// #include "arduino_secrets.h"

// const int MEGA_CS = 7;
// Arducam_Mega myCAM(MEGA_CS);
// ArducamLink myUart;

// #define SECRET_SSID "ribbit"
// #define SECRET_PASS "bf3006159a3b2d6b3a16c63ac4"

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
