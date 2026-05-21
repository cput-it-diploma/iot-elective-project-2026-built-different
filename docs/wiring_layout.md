### 🔴 Red cables (Power - 3.3V):
- ESP32 3.3V pin → connected to: power bus on the **left (L+ row)**
- L+15 → Left Sensor VIN
- L+33 → Center Sensor VIN
- L+54 → Right Sensor VIN

---

### ⚫ Black cables (GND - Sensors):
- ESP32 GND pin → connected to: power bus on the **right (R- row)**
- R-14 → Left Sensor GND
- R-29 → Center Sensor GND
- R-50 → Right Sensor GND

---

### 🟡 Yellow cables (SCL - I2C Clock):
- ESP32 GPIO22 → connected to: **F5**
- Then daisy chained to all sensors SCL:
  - J5 → Left Sensor SCL
  - J26 → Center Sensor SCL
  - J46 → Right Sensor SCL

---

### ⚪ White / Green cables (SDA - I2C Data):
- ESP32 GPIO21 → connected to: **H10**
- Then daisy chained to all sensors SDA:
  - J10 → Left Sensor SDA
  - J27 → Center Sensor SDA
  - J49 → Right Sensor SDA

---

### 🔵 XSHUT Wires (Individual sensor control):
- Left Sensor XSHUT → ESP32 GPIO15 (Blue wire)
- Center Sensor XSHUT → ESP32 GPIO16 (Brown wire)
- Right Sensor XSHUT → ESP32 GPIO4 (Purple wire)

---

## BUZZER WIRING 

---

###  Left Buzzer - Signal:
- ESP32 GPIO17 → connected to: **E60**
- E60 → Left Buzzer (+)

---

### Center Buzzer - Signal:
- ESP32 GPIO19 → connected to: **E75**
- E75 → Center Buzzer (+)

---

### Right Buzzer - Signal:
- ESP32 GPIO5 → connected to: **E90**
- E90 → Right Buzzer (+)

---

### ⚫ Black cables (Buzzers GND):
- ESP32 GND pin (same as sensors) → connected to: **ground bus (R- row)**
- R-62 → Left Buzzer (-)
- R-77 → Center Buzzer (-)
- R-92 → Right Buzzer (-)