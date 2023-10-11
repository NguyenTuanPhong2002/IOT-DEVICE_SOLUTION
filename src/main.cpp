#include <Arduino.h>
#include <BlynkApi.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


//define pins
#define TRIG_PIN 23
#define ECHO_PIN 22
#define RELAY_PIN 19

//define Blynk 
#define BLYNK_TEMPLATE_ID "TMPL6AT63NeLc"
#define BLYNK_TEMPLATE_NAME "Prjchongngaplutchooto"
#define BLYNK_AUTH_TOKEN "TmH2tSZtd2HUsr4osmr9Yrn7ejjgZjP9"
#define SSID "Esp32_lora_990812_ndp_5G"       // replace with your SSID
#define Password "0919764460ndp"           //replace with your password
#ifndef BlynkApi_h
#define BlynkApi_h

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
    delay(3000);

    // Set pin modes
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);

    //blynk
    WiFi.begin(SSID, Password);
    while (WiFi.status() != WL_CONNECTED) {
     delay(500);
    Serial.print(".");
    } 
    Blynk.begin(BLYNK_AUTH_TOKEN,SSID,Password);
}

void loop() {
    {
    Blynk.run();
    }
    // Measure distance and print to serial monitor
    float distance = measure_distance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if(distance < 10) {
        digitalWrite(RELAY_PIN, LOW); // turn on the relay
        Serial.println("Relay is ON");
        delay(5000); // wait for a second
        digitalWrite(RELAY_PIN, HIGH); // turn off the relay
        Serial.println("Relay is OFF");
        delay(1000); // wait for a second
    }
     // Wait for 1 second before measuring again
     delay(1000);
}