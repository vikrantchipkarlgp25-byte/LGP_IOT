#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// ---------- Wi-Fi ----------
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ---------- Roboninja UID ----------
const String UID = "YOUR_UID";

// ---------- Pins ----------
const int rainPin = D2;  // Rain sensor digital output
const int ledPin  = D1;  // LED

// ---------- Timing ----------
unsigned long lastSend = 0;
const unsigned long interval = 5000; // Update cloud every 5 seconds

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(rainPin, INPUT_PULLUP);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
  Serial.print("Wemos IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) return;

  // 1️⃣ Read rain sensor
  int rainState = digitalRead(rainPin);
  bool isRaining = (rainState == LOW); // LOW = rain detected

  // 2️⃣ Fetch LED command from cloud (0=OFF,1=ON,2=AUTO)
  int ledCommand = 2; // default AUTO
  HTTPClient http;
  String readURL = "http://iot.roboninja.in/index.php?action=read&UID=" + UID + "&D1";
  http.begin(client, readURL);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    payload.trim();
    if (payload == "0") ledCommand = 0;
    else if (payload == "1") ledCommand = 1;
    else ledCommand = 2;
  } else {
    Serial.println("Failed to read LED command");
  }
  http.end();

  // 3️⃣ Decide LED state
  bool ledState;
  if (ledCommand == 0) ledState = false;       // Manual OFF
  else if (ledCommand == 1) ledState = true;   // Manual ON
  else ledState = isRaining;                   // AUTO

  digitalWrite(ledPin, ledState ? HIGH : LOW);

  // 4️⃣ Debug prints
  Serial.print("Rain: "); Serial.print(isRaining ? "YES" : "NO");
  Serial.print(" | LED command: "); Serial.print(ledCommand);
  Serial.print(" | LED state: "); Serial.println(ledState ? "ON" : "OFF");

  // 5️⃣ Update cloud every 5 seconds
  if (millis() - lastSend > interval) {
    lastSend = millis();

    // Update rain value
    http.begin(client, "http://iot.roboninja.in/index.php?action=write&UID=" + UID + "&Rain=" + String(isRaining ? 1 : 0));
    http.GET();
    http.end();

    // Update LED state
    http.begin(client, "http://iot.roboninja.in/index.php?action=write&UID=" + UID + "&D1=" + String(ledState ? 1 : 0));
    http.GET();
    http.end();
  }

  delay(200); // small delay
}