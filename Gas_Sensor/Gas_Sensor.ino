#include <ESP8266WiFi.h>
#include <NinjaIoT.h>

NinjaIoT iot;

// WiFi credentials
const char* ssid = "Riddhi_4G";
const char* password = "sarita1206";

// Gas sensor & buzzer
const int gasPin = A0;
const int buzzer = D2;

// Your dashboard/class name
const String uid = "VC11";  // VC11 class

// Alarm settings
const int threshold = 50;   // Gas value threshold (set low for testing)
const int beepDuration = 300; // ms
const int beepPause = 200;    // ms
const int burstCount = 3;     // Number of beeps per burst

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);

  // Connect to NinjaIoT cloud
  iot.connect(ssid, password, uid);
  Serial.println("✅ Connected to NinjaIoT!");
}

void loop() {
  int gasValue = analogRead(gasPin);
  Serial.println("Gas Value: " + String(gasValue));

  if (gasValue > threshold) {
    Serial.println("⚠️ Gas detected! Beeping alarm...");
    // Beep in bursts
    for (int i = 0; i < burstCount; i++) {
      tone(buzzer, 1000);  // beep
      delay(beepDuration);
      noTone(buzzer);       // silence
      delay(beepPause);
    }
  }

  delay(1000); // Short delay before next reading
}
