#define LED_R 25
#define LED_G 26
#define LED_B 27

// Active LOW → 0 = ON, 1 = OFF
void setColor(bool r, bool g, bool b) {
  digitalWrite(LED_R, r ? HIGH : LOW);
  digitalWrite(LED_G, g ? HIGH : LOW);
  digitalWrite(LED_B, b ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // ปิดไฟทั้งหมด
  setColor(false, false, false);
}

void loop() {
  Serial.println("RED");
  setColor(true, false, false);
  delay(500);

  Serial.println("GREEN");
  setColor(false, true, false);
  delay(500);

  Serial.println("BLUE");
  setColor(false, false, true);
  delay(500);

  Serial.println("YELLOW");
  setColor(true, true, false);
  delay(500);

  Serial.println("CYAN");
  setColor(false, true, true);
  delay(500);

  Serial.println("MAGENTA");
  setColor(true, false, true);
  delay(500);

  Serial.println("WHITE");
  setColor(true, true, true);
  delay(500);

  Serial.println("OFF");
  // active LOW → OFF = HIGH
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
  delay(500);
}
