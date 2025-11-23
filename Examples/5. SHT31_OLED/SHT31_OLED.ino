#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA, SCL

  // --- SHT31 ---
  if (!sht31.begin(0x44)) {  // Default I2C address
    Serial.println("SHT31 not found!");
    while (1) delay(1);
  }
  Serial.println("SHT31 OK");

  // --- OLED ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1) delay(1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("SHT31 + OLED Ready");
  display.display();
  delay(1000);
}

void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print(" C\t Hum: ");
    Serial.print(h);
    Serial.println(" %RH");
  } else {
    Serial.println("SHT31 Error Reading!");
  }

  // --- แสดงผลบน OLED ---
  display.clearDisplay();
  display.setTextSize(2);

  display.setCursor(0, 0);
  display.print("T: ");
  display.print(t, 1);
  display.println("C");

  display.setCursor(0, 30);
  display.print("H: ");
  display.print(h, 1);
  display.println("%");

  display.display();

  delay(1000);
}
