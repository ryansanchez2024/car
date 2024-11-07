   const int trigPin = 12;
   const int echoPin = 13;
   long duration;
   int distance;

   void setup() {
     pinMode(trigPin, OUTPUT);
     pinMode(echoPin, INPUT);
     Serial.begin(9600);
   }

   void loop() {
     // Clear the trigPin
     digitalWrite(trigPin, LOW);
     delayMicroseconds(2);

     // Set the trigPin on HIGH state for 10 microseconds
     digitalWrite(trigPin, HIGH);
     delayMicroseconds(10);
     digitalWrite(trigPin, LOW);

     // Read the echoPin, returns the sound wave travel time in microseconds
     duration = pulseIn(echoPin, HIGH);

     // Calculate the distance
     distance = duration * 0.034 / 2;

     // Print the distance on the Serial Monitor
     Serial.print("Distance: ");
     Serial.println(distance);

     delay(1000);
   }





// /*
//  * Ultrasonic Simple
//  * Prints the distance read by an ultrasonic sensor in
//  * centimeters. They are supported to four pins ultrasound
//  * sensors (liek HC-SC04) and three pins (like PING)))
//  * and Seeed Studio sensors).
//  *
//  * The circuit:
//  * * Module HR-SC04 (four pins) or PING))) (and other with
//  *   three pins), attached to digital pins as follows:
//  * ---------------------    --------------------
//  * | HC-SC04 | Arduino |    | 3 pins | Arduino |
//  * ---------------------    --------------------
//  * |   Vcc   |   5V    |    |   Vcc  |   5V    |
//  * |   Trig  |   12    | OR |   SIG  |   13    |
//  * |   Echo  |   13    |    |   Gnd  |   GND   |
//  * |   Gnd   |   GND   |    --------------------
//  * ---------------------
//  * Note: You do not obligatorily need to use the pins defined above
//  * 
//  * By default, the distance returned by the read()
//  * method is in centimeters. To get the distance in inches,
//  * pass INC as a parameter.
//  * Example: ultrasonic.read(INC)
//  */

// #include <Ultrasonic.h>

// /*
//  * Pass as a parameter the trigger and echo pin, respectively,
//  * or only the signal pin (for sensors 3 pins), like:
//  * Ultrasonic ultrasonic(13);
//  */
// Ultrasonic ultrasonic(12,13);
// int distance;

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   // Pass INC as a parameter to get the distance in inches
//   distance = ultrasonic.read();
  
//   if (distance == 0) {
//     Serial.println("No echo received. Check sensor and connections.");
//   } else {
//     Serial.print("Distance in CM: ");
//     Serial.println(distance);
//   }
  
//   delay(1000);
// }