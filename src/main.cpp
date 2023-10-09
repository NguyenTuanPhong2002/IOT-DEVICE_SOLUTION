#include <Arduino.h>

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

    // Set pin modes
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    // Measure distance and print to serial monitor
    float distance = measure_distance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Wait for 1 second before measuring again
    delay(1000);
}