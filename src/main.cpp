#include <Arduino.h>
#include <TinyGPSPlus.h>


TinyGPSPlus gps;
//define pins
#define TRIG_PIN 23
#define ECHO_PIN 22
// Define function to measure distance
float measure_distance() {
    // Send a 10us pulse to trigger pin
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Wait for echo pin to go high
    while (digitalRead(ECHO_PIN) == LOW);

    // Measure duration of echo pin being high
    unsigned long start_time = micros();
    while (digitalRead(ECHO_PIN) == HIGH);
    unsigned long end_time = micros();

    // Calculate distance in cm
    float duration = end_time - start_time;
    float distance = duration / 58;

    return distance;
}

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    Serial2.begin(9600);
    delay(3000);

    // Set pin modes
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}
void updateSerial(){
  delay(500);
  while (Serial.available())  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid()){
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }
}
void loop() {
    // Measure distance and print to serial monitor
    float distance = measure_distance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Wait for 1 second before measuring again
    delay(1000);

    //updateSerial();
    while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
        displayInfo();
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}