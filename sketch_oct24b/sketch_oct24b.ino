#define BLYNK_TEMPLATE_ID "TMPL6lcCs9c-r"
#define BLYNK_TEMPLATE_NAME "GIAM SAT HE THONGCopy"
#define BLYNK_AUTH_TOKEN "kpJ74yxYFsUJKQKTQFD0NMaGJ46XKPwN"

#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "NNE";
char pass[] = "12345678";

BlynkTimer timer;
WidgetTerminal terminal(V7);

const int trigPin = 23;  // Chân kích hoạt của cảm biến
const int echoPin = 22;  // Chân thu của cảm biến
const int relay = 32;

long setPoint = 0;
int statusPin = 0;
int count = 0;

void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, statusPin);
  Blynk.virtualWrite(V3, setPoint = getValue());
  terminal.println("Đà Nẵng 550000, Việt Nam");
}


long getValue() {
  long duration, distance;

  // Gửi xung kích 10 µs lên chân kích hoạt
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Đọc thời gian trở về của xung kích
  duration = pulseIn(echoPin, HIGH);

  // Chuyển thời gian thành khoảng cách (cm)
  distance = (duration / 2) / 29.1;

  // In giá trị khoảng cách ra Serial Monitor
  Serial.print("Khoảng cách: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay, OUTPUT);

  setPoint = getValue();

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();

  if (getValue() < 5) {
    count++;
    if (count > 5) {
      digitalWrite(relay, 1);
      statusPin = 1;
      Blynk.logEvent("Cảnh báo: Điều kiện cảnh báo xảy ra!");
      Serial.println("on led");
    }
  } else {
    digitalWrite(relay, 0);
    count = 0;
    statusPin = 0;
  }
}
