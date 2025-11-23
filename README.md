🟦 ESP32 All-In-One IoT Controller

ปุ่มกด • รีเลย์ • RGB LED • VR (ADC) • SHT31 • OLED • Buzzer • MQTT

📌 ภาพรวมโปรเจกต์

โปรเจกต์นี้คือระบบ IoT แบบครบวงจรบน ESP32 ประกอบไปด้วย

ปุ่มกด 2 ปุ่ม พร้อม Debounce + Edge detect

รีเลย์ 2 ช่อง (Active LOW toggle)

RGB LED แสดงสถานะ (WiFi, MQTT, Error)

VR (Potentiometer) อ่านค่า ADC

SHT31 อ่าน Temperature + Humidity

OLED SSD1306 แสดงข้อมูลแบบ Real-Time

Passive Buzzer สำหรับแจ้งเตือน

MQTT ส่งข้อมูลแยก Topic

MQTT SUB ควบคุมรีเลย์ผ่าน Topic ได้

เหมาะสำหรับงาน Smart Home, Automation, Classroom IoT และ Project Showcase

🧩 Features
✔ Input / Output

ปุ่มกด 2 ตัวแบบตรวจจับเฉพาะตอนกด (Falling Edge)

รีเลย์ 2 ช่อง Active LOW (กดครั้งแรกติด ครั้งสองดับ)

RGB LED แสดงสถานะระบบ

กระพริบฟ้า = กำลังเชื่อม WiFi

กระพริบเหลือง = กำลังเชื่อม MQTT

เขียวค้าง = เชื่อม MQTT สำเร็จ

กระพริบแดง = Error

Buzzer ตอบสนองการกดและคำสั่ง MQTT

✔ Sensors

VR ADC บน pin 36 → แสดงผล 0–100%

SHT31 → Temperature & Humidity

✔ Display

OLED SSD1306 → อัปเดตค่า R1, R2, Temp, Hum, VR ทุก Loop

✔ MQTT

ส่งข้อมูลขึ้น EMQX แยกเป็น 5 Topic:

SPU/device1/temp
SPU/device1/hum
SPU/device1/vr
SPU/device1/relay1
SPU/device1/relay2


รับคำสั่งควบคุมรีเลย์:

SPU/device1/cmd/relay1
SPU/device1/cmd/relay2


Payload:

1       → เปิด
0       → ปิด
toggle  → สลับสถานะ

🔌 Wiring (ESP32 Pinout)
🔘 Buttons
Function	Pin	Note
BTN A	0	INPUT_PULLUP
BTN B	4	INPUT_PULLUP
⚡ Relay (Active LOW)
Relay	Pin	State
Relay 1	32	LOW = ON
Relay 2	14	LOW = ON
🌈 RGB LED (Active LOW)
Color	Pin
Red	25
Green	26
Blue	27
🎛 VR (Potentiometer)
Function	Pin
VR ADC	36
🌡 SHT31 + OLED (I2C)
Signal	Pin
SDA	21
SCL	22
🔊 Buzzer
Function	Pin
BUZZER	33

📦 Libraries Required
ติดตั้งผ่าน Arduino IDE → Tools → Manage Libraries
Library	Creator
Adafruit SHT31	Adafruit
Adafruit SSD1306	Adafruit
Adafruit GFX Library	Adafruit
PubSubClient	Nick O'Leary
Wire	Built-in
WiFi	Built-in

🌐 MQTT Server (EMQX)
Host: broker.emqx.io
Port: 1883
Username: none
Password: none

📡 MQTT Topics Map
Publish
Topic	Data
SPU/device1/temp	Temperature (°C)
SPU/device1/hum	Humidity (%)
SPU/device1/vr	VR Percent
SPU/device1/relay1	1/0
SPU/device1/relay2	1/0
Subscribe (Control)
Topic	Payload
SPU/device1/cmd/relay1	1, 0, toggle
SPU/device1/cmd/relay2	1, 0, toggle

🖥 ตัวอย่าง UI บน OLED
R1: ON     R2: OFF
T:  25.3 C
H:  61.0 %
VR: 78 %


อ่านง่าย สวยงาม อัปเดตทุก loop

🧪 การทดสอบระบบ
1️⃣ ทดสอบ WiFi + MQTT

เปิด Serial Monitor ดูสถานะไฟ RGB
ถ้าค้างเขียว → ระบบสมบูรณ์ 🎉

2️⃣ ทดสอบปุ่มกด

กดปุ่ม A → Relay1 → ติด/ดับ
กดปุ่ม B → Relay2 → ติด/ดับ

3️⃣ ทดสอบ MQTT Subscribe (ควบคุมรีเลย์)

ส่งผ่าน EMQX Dashboard หรือ MQTTX:

เปิดรีเลย์ 1

Topic: SPU/device1/cmd/relay1
Payload: 1


Toggle รีเลย์ 2

Topic: SPU/device1/cmd/relay2
Payload: toggle

4️⃣ ทดสอบ MQTT Publish

ดูค่าที่ส่งขึ้นแบบ live ทุก 0.1 วินาที
