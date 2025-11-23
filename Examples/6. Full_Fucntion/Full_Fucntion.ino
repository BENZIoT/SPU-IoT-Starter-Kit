#include <WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ----------------------------------------------------------
// PIN DEFINE
// ----------------------------------------------------------
#define BTN_A    0
#define BTN_B    4

#define RELAY_1  32
#define RELAY_2  14

#define VR_PIN   36
#define BUZZER   33

// RGB Active LOW
#define LED_R    25
#define LED_G    26
#define LED_B    27

// ----------------------------------------------------------
// GLOBAL STATE
// ----------------------------------------------------------
bool wifiOK = false;
bool mqttOK = false;

bool relay1State = false;
bool relay2State = false;

// Button Debounce
unsigned long debounceDelay = 150;
unsigned long lastA = 0, lastB = 0;
bool lastStateA = HIGH;
bool lastStateB = HIGH;

// SHT31 / OLED
Adafruit_SHT31 sht31 = Adafruit_SHT31();
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ----------------------------------------------------------
// MQTT SETTINGS
// ----------------------------------------------------------
const char *ssid       = "Teddy Bear"; //WiFi name
const char *password   = "ultraman";   //WiFi Password

const char *mqtt_server = "broker.emqx.io";
int mqtt_port           = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMQTT = 0;
const char *baseTopic  = "SPU/device1/";

// ----------------------------------------------------------
// HELPERS
// ----------------------------------------------------------
void setRGB(bool r, bool g, bool b) {
  // Active LOW: true = ON (LOW), false = OFF (HIGH)
  digitalWrite(LED_R, r ? LOW : HIGH);
  digitalWrite(LED_G, g ? LOW : HIGH);
  digitalWrite(LED_B, b ? LOW : HIGH);
}

void beep(int freq, int duration) {
  tone(BUZZER, freq, duration);
}

void wifiStatusLED() {

  // 1) WiFi Connecting → Blink Blue
  if (!wifiOK) {
    static unsigned long t = 0;
    if (millis() - t > 500) {
      static bool s = false;
      s = !s;
      setRGB(false, false, s);
      t = millis();
    }
    return;
  }

  // 2) WiFi OK but MQTT not connected → Blink Yellow
  if (wifiOK && !mqttOK) {
    static unsigned long t = 0;
    if (millis() - t > 500) {
      static bool s = false;
      s = !s;
      setRGB(s, s, false);   // R+G = Yellow
      t = millis();
    }
    return;
  }

  // 3) MQTT OK → Solid Green
  if (wifiOK && mqttOK) {
    setRGB(false, true, false);  // Green
    return;
  }

  // 4) Fallback → Blink Red
  static unsigned long t = 0;
  if (millis() - t > 500) {
    static bool s = false;
    s = !s;
    setRGB(s, false, false);     // Red
    t = millis();
  }
}

// ----------------------------------------------------------
// MQTT CALLBACK: control relays via MQTT
// ----------------------------------------------------------
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("MQTT CMD → ");
  Serial.print(topic);
  Serial.print(" = ");
  Serial.println(msg);

  // Relay 1: topic SPU/device1/cmd/relay1
  if (String(topic) == "SPU/device1/cmd/relay1") {

    if (msg == "1")      relay1State = true;
    else if (msg == "0") relay1State = false;
    else if (msg == "toggle") relay1State = !relay1State;

    digitalWrite(RELAY_1, relay1State ? LOW : HIGH); // Active LOW
    beep(1600, 60);
  }

  // Relay 2: topic SPU/device1/cmd/relay2
  if (String(topic) == "SPU/device1/cmd/relay2") {

    if (msg == "1")      relay2State = true;
    else if (msg == "0") relay2State = false;
    else if (msg == "toggle") relay2State = !relay2State;

    digitalWrite(RELAY_2, relay2State ? LOW : HIGH); // Active LOW
    beep(1300, 60);
  }
}

// ----------------------------------------------------------
// WIFI & MQTT CONNECT
// ----------------------------------------------------------
void connectWiFi() {
  Serial.println("Connecting WiFi...");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Connecting WiFi");
  display.display();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
    display.print(".");
    display.display();
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected!\n");
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();
  Serial.println("\nWiFi OK!");
  wifiOK = true;
  delay(1000);
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("MQTT...");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Connecting MQTT:\n" + String(mqtt_server));
    display.display();
    if (client.connect("esp32-benz-client")) {
      Serial.println("Connected");
      display.println("\n\nMQTT Connected!.");
      display.display();
      mqttOK = true;

      // Subscribe control topics
      client.subscribe("SPU/device1/cmd/relay1");
      client.subscribe("SPU/device1/cmd/relay2");
      Serial.println("Subscribed to relay control topics");
      delay(1000);
    } else {
      mqttOK = false;
      display.println("\n\nTry Connnect MQTT!!!.");
      display.display();
      Serial.print("Fail: ");
      Serial.println(client.state());
      delay(1000);
    }
  }
}

// ----------------------------------------------------------
// SETUP
// ----------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  digitalWrite(RELAY_1, HIGH);   // OFF (Active LOW)
  digitalWrite(RELAY_2, HIGH);   // OFF

  pinMode(BUZZER, OUTPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  setRGB(false, false, false);

  Wire.begin(21, 22);

  if (!sht31.begin(0x44)) {
    Serial.println("SHT31 not found!");
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(1);        

  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);

  beep(2000, 120);
}

// ----------------------------------------------------------
// LOOP
// ----------------------------------------------------------
void loop() {

  // RGB status LED
  wifiStatusLED();

  // Keep MQTT alive
  if (!client.connected()) {
    mqttOK = false;
    connectMQTT();
  }
  client.loop();

  // ------------------------------------------------------
  // BUTTON A → Relay 1 (edge detect)
  // ------------------------------------------------------
  bool currentA = digitalRead(BTN_A);
  if (lastStateA == HIGH && currentA == LOW && millis() - lastA > debounceDelay) {
    relay1State = !relay1State;
    digitalWrite(RELAY_1, relay1State ? LOW : HIGH); // Active LOW
    beep(1400, 70);
    lastA = millis();
  }
  lastStateA = currentA;

  // ------------------------------------------------------
  // BUTTON B → Relay 2 (edge detect)
  // ------------------------------------------------------
  bool currentB = digitalRead(BTN_B);
  if (lastStateB == HIGH && currentB == LOW && millis() - lastB > debounceDelay) {
    relay2State = !relay2State;
    digitalWrite(RELAY_2, relay2State ? LOW : HIGH); // Active LOW
    beep(1200, 70);
    lastB = millis();
  }
  lastStateB = currentB;

  // ------------------------------------------------------
  // READ VR (ADC 36)
  // ------------------------------------------------------
  int   rawADC  = analogRead(VR_PIN);
  float percent = (rawADC / 4095.0) * 100.0;

  // ------------------------------------------------------
  // READ SHT31 Sensor
  // ------------------------------------------------------
  float temp = sht31.readTemperature();
  float hum  = sht31.readHumidity();

  // ------------------------------------------------------
  // OLED DISPLAY
  // ------------------------------------------------------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("R1: "); display.println(relay1State ? "ON" : "OFF");
  display.setCursor(64, 0);
  display.print("R2: "); display.println(relay2State ? "ON" : "OFF");

  display.setCursor(0, 16);
  display.print("T: "); display.print(temp, 1); display.println("C");

  display.setCursor(0, 30);
  display.print("H: "); display.print(hum, 1); display.println("%");

  display.setCursor(0, 46);
  display.print("VR: "); display.print(percent, 0); display.println("%");

  display.display();

  // ------------------------------------------------------
  // MQTT — SPLIT TOPIC PUBLISH
  // ------------------------------------------------------
  if (millis() - lastMQTT > 1000) {

    char t1[40]; sprintf(t1, "%stemp", baseTopic);
    client.publish(t1, String(temp, 2).c_str());

    char t2[40]; sprintf(t2, "%shum", baseTopic);
    client.publish(t2, String(hum, 2).c_str());

    char t3[40]; sprintf(t3, "%svr", baseTopic);
    client.publish(t3, String(percent, 0).c_str());

    char t4[40]; sprintf(t4, "%srelay1", baseTopic);
    client.publish(t4, relay1State ? "1" : "0");

    char t5[40]; sprintf(t5, "%srelay2", baseTopic);
    client.publish(t5, relay2State ? "1" : "0");

    lastMQTT = millis();
    Serial.println("MQTT Sent");
  }

  delay(100);
}
