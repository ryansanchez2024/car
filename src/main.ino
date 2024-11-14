#include <Stepper.h>

const int trigPin = 12;
const int echoPin = 13;
long duration;
int distance;

const int stepsPerRevolution = 2048; // For 28BYJ-48 with ULN2003 driver
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // IN1, IN2, IN3, IN4

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  myStepper.setSpeed(15); // Speed in RPM

}

void loop() {
  // ULTRASONIC SENSOR
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

  // STEPPER MOTOR
  myStepper.step(stepsPerRevolution); // One full revolution
  delay(1000);
  myStepper.step(-stepsPerRevolution); // Reverse direction
  delay(1000);
}