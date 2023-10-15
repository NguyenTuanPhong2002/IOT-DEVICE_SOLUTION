#define BLYNK_TEMPLATE_ID "TMPL6lcCs9c-r"
#define BLYNK_TEMPLATE_NAME "GIAM SAT HE THONGCopy"
#define BLYNK_AUTH_TOKEN "kpJ74yxYFsUJKQKTQFD0NMaGJ46XKPwN"

#define BLYNK_PRINT Serial
// define pins
#define TRIG_PIN 23
#define ECHO_PIN 22
#define RELAY_PIN 19

// define Blynk
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char ssid[] = "NTP";       // replace with your SSID
char pass[] = "01112002";  // replace with your password

BlynkTimer timer;

float distance;
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, distance);
}

// Define function to measure distance
float measure_distance() {
  // Send a 10us pulse to trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for echo pin to go high
  while (digitalRead(ECHO_PIN) == LOW) {};

  // Measure duration of echo pin being high
  unsigned long start_time = micros();
  while (digitalRead(ECHO_PIN) == HIGH) {};
  unsigned long end_time = micros();

  // Calculate distance in cm
  float duration = end_time - start_time;
  float distance = duration / 58;

  return distance;
}

void process() {
  // Measure distance and print to serial monitor
  distance = measure_distance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Blynk.virtualWrite(V1, distance);
  if (distance < 10) {
    digitalWrite(RELAY_PIN, LOW);  // turn on the relay
    Serial.println("Relay is ON");
    delay(500);                    // wait for a second
    digitalWrite(RELAY_PIN, HIGH);  // turn off the relay
    Serial.println("Relay is OFF");
    delay(500);  // wait for a second
  }
  // Wait for 1 second before measuring again
  delay(1000);
}


void setup() {
  // Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // blynk app setup

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  process();
}