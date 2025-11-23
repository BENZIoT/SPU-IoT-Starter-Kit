#define VR_PIN 36   // ADC pin

void setup() {
  Serial.begin(115200);

  // ไม่ต้อง pinMode สำหรับ ADC1
  Serial.println("=== ADC Reader on Pin 36 ===");
}

void loop() {
  int rawValue = analogRead(VR_PIN);   // 0 - 4095

  // แปลงเป็น 0 - 100%
  float percent = (rawValue / 4095.0) * 100.0;

  // แปลงเป็น 0 - 3.3V
  float voltage = (rawValue / 4095.0) * 3.3;

  Serial.print("RAW: ");
  Serial.print(rawValue);
  Serial.print(" | ");
  
  Serial.print("Percent: ");
  Serial.print(percent, 1);
  Serial.print("% | ");

  Serial.print("Voltage: ");
  Serial.print(voltage, 2);
  Serial.println(" V");

  delay(100);
}
