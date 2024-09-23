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

#include <SPI.h>
#include "WiFiS3.h"
#include "Arducam_Mega.h"
#include "ArducamLink.h"
#include "arduino_secrets.h"

const int MEGA_CS = 7;
Arducam_Mega myCAM(MEGA_CS);
ArducamLink myUart;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  server.begin();
  printWifiStatus();

  myUart.arducamUartBegin(115200);
  myCAM.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");
    String currentLine = "";
    String request = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // End of the HTTP request header

            // Determine what the client is requesting
            if (request.indexOf("GET /H") >= 0) {
              digitalWrite(LED_BUILTIN, HIGH);
              sendHTMLResponse(client, "LED is now ON");
            } else if (request.indexOf("GET /L") >= 0) {
              digitalWrite(LED_BUILTIN, LOW);
              sendHTMLResponse(client, "LED is now OFF");
            } else if (request.indexOf("GET /capture") >= 0) {
              captureAndSendImage(client);
            } else {
              // Default response
              sendHTMLResponse(client, "Welcome to the Arduino Web Server");
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}

void captureAndSendImage(WiFiClient &client) {
  // Capture image
  myCAM.takePicture(CAM_IMAGE_MODE_VGA, CAM_IMAGE_PIX_FMT_JPG);
  uint32_t imgLen = myCAM.getTotalLength();
  Serial.print("Image Length: ");
  Serial.println(imgLen);

  // Send HTTP response headers
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: image/jpeg");
  client.println("Connection: close");
  client.println();

  // Buffer to hold image data
  const size_t bufferSize = 1024;
  uint8_t buffer[bufferSize];
  uint32_t bytesSent = 0;

  // Read image data from camera and send to client
  while (bytesSent < imgLen) {
    size_t bytesToRead = (imgLen - bytesSent) > bufferSize ? bufferSize : (imgLen - bytesSent);
    myCAM.readBuff(buffer, bytesToRead);
    client.write(buffer, bytesToRead);
    bytesSent += bytesToRead;

    // Optional: Add a short delay to avoid overwhelming the WiFi connection
    delay(10);
  }

  // End the response
  client.println();

  // Instead of clearFIFOBuffer, we'll just print a message
  // Optional: If you need to reset the camera or clear its buffer,
  // consult the Arducam_Mega documentation for the correct method
  // or consider reinitializing the camera if necessary
  
  Serial.println("Image sent to client");
  myCAM.reset();
}

void sendHTMLResponse(WiFiClient &client, String message) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>Arduino Web Server</title></head>");
  client.println("<body>");
  client.println("<h1>" + message + "</h1>");
  client.println("<p><a href=\"/H\">Turn LED ON</a></p>");
  client.println("<p><a href=\"/L\">Turn LED OFF</a></p>");
  client.println("<p><a href=\"/capture\">Capture Image</a></p>");
  client.println("</body>");
  client.println("</html>");

  client.println();
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
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}