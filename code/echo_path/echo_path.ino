#include <Wire.h>
#include <VL53L1X.h>

//
// PINS
//

// XSHUT - sensors
#define XSHUT_LEFT    15
#define XSHUT_CENTER  16
#define XSHUT_RIGHT   4

// Buzzers
#define BUZZER_LEFT   17
#define BUZZER_CENTER 19
#define BUZZER_RIGHT  5

// Sensor addresses
#define ADDR_LEFT     0x30
#define ADDR_CENTER   0x32
#define ADDR_RIGHT    0x34

//
// THRESHOLDS
// 
#define WARNING_DIST  1500    // 1.5m - slow beep
#define DANGER_DIST   700     // 0.7m - fast beep
#define CRITICAL_DIST 300     // 0.3m - continuous

// 
// SENSOR OBJECTS
//
VL53L1X leftSensor;
VL53L1X centerSensor;
VL53L1X rightSensor;
//
// HELPER FUNCTIONS
//
bool checkDevice(uint8_t addr) {
  Wire.beginTransmission(addr);
  return (Wire.endTransmission() == 0);
}

bool initSensor(VL53L1X &sensor, uint8_t xshutPin, uint8_t addr, const char* name) {
  Serial.print("\n xxx Booting ");
  Serial.println(name);

  digitalWrite(xshutPin, HIGH);
  delay(1000);

  if (!checkDevice(0x29)) {
    Serial.println("X No device at default address (0x29)");
    digitalWrite(xshutPin, LOW);
    return false;
  }

  if (!sensor.init()) {
    Serial.println("X init() failed");
    digitalWrite(xshutPin, LOW);
    return false;
  }

  sensor.setAddress(addr);
  delay(200);

  if (!checkDevice(addr)) {
    Serial.println("X Address change failed");
    digitalWrite(xshutPin, LOW);
    return false;
  }

  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(100000);
  sensor.startContinuous(200);

  Serial.print("v/ ");
  Serial.print(name);
  Serial.print(" ready @ 0x");
  Serial.println(addr, HEX);

  return true;
}

void beepPattern(int buzzerPin, int distance) {
  if (distance <= 0 || distance > WARNING_DIST) {
    // No obstacle
    // invalid reading
    // buzzer OFF
    digitalWrite(buzzerPin, LOW);
    return;
  }
  
  if (distance <= CRITICAL_DIST) {
    // CRITICAL
    // Continuous beep
    digitalWrite(buzzerPin, HIGH);
  } 
  else if (distance <= DANGER_DIST) {
    // DANGER
    // Fast beep (100ms on, 100ms off)
    static unsigned long lastToggle = 0;
    unsigned long now = millis();
    if (now - lastToggle > 100) {
      digitalWrite(buzzerPin, !digitalRead(buzzerPin));
      lastToggle = now;
    }
  } 
  else if (distance <= WARNING_DIST) {
    // WARNING
    // Slow beep (300ms on, 300ms off)
    static unsigned long lastToggle = 0;
    unsigned long now = millis();
    if (now - lastToggle > 300) {
      digitalWrite(buzzerPin, !digitalRead(buzzerPin));
      lastToggle = now;
    }
  }
}

// 
// SETUP
//
void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n╔════════════════════════════════════════════╗");
  Serial.println("║     VL53L1X + BUZZER SYSTEM READY          ║");
  Serial.println("║     EchoPath - Visual Impairment Aid       ║");
  Serial.println("╚════════════════════════════════════════════╝\n");

  Wire.begin(21, 22);
  Wire.setClock(50000);

  // Initialize XSHUT pins
  pinMode(XSHUT_LEFT, OUTPUT);
  pinMode(XSHUT_CENTER, OUTPUT);
  pinMode(XSHUT_RIGHT, OUTPUT);

  // Initialize buzzer pins
  pinMode(BUZZER_LEFT, OUTPUT);
  pinMode(BUZZER_CENTER, OUTPUT);
  pinMode(BUZZER_RIGHT, OUTPUT);

  // Start with everything OFF
  digitalWrite(XSHUT_LEFT, LOW);
  digitalWrite(XSHUT_CENTER, LOW);
  digitalWrite(XSHUT_RIGHT, LOW);
  digitalWrite(BUZZER_LEFT, LOW);
  digitalWrite(BUZZER_CENTER, LOW);
  digitalWrite(BUZZER_RIGHT, LOW);
  
  delay(1000);

  // Initialize sensors one by one
  bool ok1 = initSensor(leftSensor, XSHUT_LEFT, ADDR_LEFT, "LEFT");
  delay(1000);
  bool ok2 = initSensor(centerSensor, XSHUT_CENTER, ADDR_CENTER, "CENTER");
  delay(1000);
  bool ok3 = initSensor(rightSensor, XSHUT_RIGHT, ADDR_RIGHT, "RIGHT");

  // Wake all sensors
  digitalWrite(XSHUT_LEFT, HIGH);
  digitalWrite(XSHUT_CENTER, HIGH);
  digitalWrite(XSHUT_RIGHT, HIGH);
  delay(300);

  Serial.println("\n=== STARTUP SUMMARY ===");
  Serial.print("LEFT: "); Serial.println(ok1 ? "OK v/" : "FAIL X");
  Serial.print("CENTER: "); Serial.println(ok2 ? "OK v/" : "FAIL X");
  Serial.print("RIGHT: "); Serial.println(ok3 ? "OK v/" : "FAIL X");

  Serial.println("\noo-oo SYSTEM READY - Buzzers active\n");
  delay(1000);
}

// 
// LOOP
//
void loop() {
  // Read all sensors
  int leftDist = leftSensor.read();
  int centerDist = centerSensor.read();
  int rightDist = rightSensor.read();

  // Print readings to Serial
  Serial.print("L:");
  Serial.print(leftDist);
  Serial.print("mm  C:");
  Serial.print(centerDist);
  Serial.print("mm  R:");
  Serial.print(rightDist);
  Serial.println("mm");

  // Trigger buzzers based on distances
  beepPattern(BUZZER_LEFT, leftDist);
  beepPattern(BUZZER_CENTER, centerDist);
  beepPattern(BUZZER_RIGHT, rightDist);

  // Delay for vibes (stability)
  delay(100);
}


// XSHUT pin used to change addresses from 0x29 to 0x30, 0x32, 0x34