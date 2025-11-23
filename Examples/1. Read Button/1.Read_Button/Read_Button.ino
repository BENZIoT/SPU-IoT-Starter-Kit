#define BTN_A 0
#define BTN_B 4

unsigned long debounceDelay = 50;
unsigned long lastA = 0, lastB = 0;

bool stateA = HIGH;
bool stateB = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);

  Serial.println("=== ESP32 Button Reader Started ===");
}

void loop() {
  bool readA = digitalRead(BTN_A);
  bool readB = digitalRead(BTN_B);

  if (readA != stateA && millis() - lastA > debounceDelay) {
    stateA = readA;
    lastA = millis();
    Serial.println(stateA == LOW ? "Button A PRESSED" : "Button A RELEASED");
  }

  if (readB != stateB && millis() - lastB > debounceDelay) {
    stateB = readB;
    lastB = millis();
    Serial.println(stateB == LOW ? "Button B PRESSED" : "Button B RELEASED");
  }
}
