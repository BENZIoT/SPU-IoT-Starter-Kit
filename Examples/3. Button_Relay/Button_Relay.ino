#define BTN_A 0
#define BTN_B 4

#define RELAY_1 32
#define RELAY_2 14

// Debounce
unsigned long debounceDelay = 100;
unsigned long lastA = 0, lastB = 0;

// จำสถานะปุ่มก่อนหน้า
bool lastStateA = HIGH;
bool lastStateB = HIGH;

// สถานะรีเลย์ (true = ON, false = OFF)
bool relay1State = false;
bool relay2State = false;

void setup() {
  Serial.begin(115200);

  // ปุ่ม
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);

  // รีเลย์ Active LOW
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);

  // ปิดรีเลย์ตอนเริ่ม (OFF = HIGH)
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);

  Serial.println("=== Button + Relay (Active LOW) Ready ===");
}

void loop() {
  bool currentA = digitalRead(BTN_A);
  bool currentB = digitalRead(BTN_B);

  // ---------------------
  // ปุ่ม A → Relay 1
  // ---------------------
  if (lastStateA == HIGH && currentA == LOW && millis() - lastA > debounceDelay) {
    relay1State = !relay1State;      // Toggle
    digitalWrite(RELAY_1, relay1State ? LOW : HIGH);  // Active LOW

    Serial.print("Relay 1: ");
    Serial.println(relay1State ? "ON" : "OFF");

    lastA = millis();
  }
  lastStateA = currentA;

  // ---------------------
  // ปุ่ม B → Relay 2
  // ---------------------
  if (lastStateB == HIGH && currentB == LOW && millis() - lastB > debounceDelay) {
    relay2State = !relay2State;      // Toggle
    digitalWrite(RELAY_2, relay2State ? LOW : HIGH);  // Active LOW

    Serial.print("Relay 2: ");
    Serial.println(relay2State ? "ON" : "OFF");

    lastB = millis();
  }
  lastStateB = currentB;
}
