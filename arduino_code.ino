#include <Servo.h>

// Ultrasonic Sensor Pins
#define TRIG_PIN 3
#define ECHO_PIN 2

// Servo Pin
#define SERVO_PIN 5

// L298N Motor Driver Pins
#define ENA 11
#define IN1 10
#define IN2 9
#define IN3 8
#define IN4 7
#define ENB 6

// Single Center IR Sensor (Pin 4) & Buzzer
#define IR_CENTER_PIN 4
#define BUZZER_PIN    13

// Sensor Orientation Mode:
// true  = Downward cliff sensor (triggers when green LED turns OFF over an edge)
// false = Forward obstacle sensor (triggers when green LED turns ON near an object)
const bool CLIFF_MODE = true;

// --- Easy Tuning Constants ---
const int STOP_DISTANCE = 50;   // Front ultrasonic trigger distance (cm)
const int BASE_SPEED    = 95;   // Forward/reverse cruising speed
const int TURN_SPEED    = 95;   // Pivot turning speed
const int TURN_TIME_MS  = 240;  // Turn duration in ms
const int MAX_SPEED     = 160;  // Safety speed cap

Servo scanServo;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IR_CENTER_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  scanServo.attach(SERVO_PIN);
  scanServo.write(90);

  stopMotors();
  beep(1500, 120);
  delay(800);
}

void loop() {
  // 1. Check center IR sensor
  int irReading = digitalRead(IR_CENTER_PIN);
  bool irTriggered = CLIFF_MODE ? (irReading == HIGH) : (irReading == LOW);

  if (irTriggered) {
    handleIrTrigger();
    return;
  }

  // 2. Check front ultrasonic distance
  int frontDist = getDistance();

  if (frontDist <= STOP_DISTANCE) {
    handleFrontObstacle();
  } else {
    moveForward();
  }

  delay(30);
}

// --- Center IR Trigger Recovery ---
void handleIrTrigger() {
  stopMotors();

  if (CLIFF_MODE) {
    beep(2200, 90);
    delay(50);
    beep(2200, 90);
  } else {
    beep(1200, 120);
  }

  // Back away before scanning
  moveBackward();
  delay(350);
  stopMotors();
  delay(150);

  chooseClearDirection(TURN_TIME_MS);
}

// --- Front Ultrasonic Obstacle Avoidance ---
void handleFrontObstacle() {
  stopMotors();
  beep(1200, 120);

  moveBackward();
  delay(250);
  stopMotors();
  delay(150);

  chooseClearDirection(TURN_TIME_MS);
}

// --- Servo Scan & Turn Helper ---
void chooseClearDirection(int turnDurationMs) {
  scanServo.write(20);  // Scan right
  delay(350);
  int rightDist = getDistance();

  scanServo.write(160); // Scan left
  delay(350);
  int leftDist = getDistance();

  scanServo.write(90);  // Return to center
  delay(250);

  if (leftDist < STOP_DISTANCE && rightDist < STOP_DISTANCE) {
    // Dead end:
    turnRight();
    delay(turnDurationMs * 1.6);
  } else if (leftDist > rightDist) {
    turnLeft();
    delay(turnDurationMs);
  } else {
    turnRight();
    delay(turnDurationMs);
  }

  stopMotors();
  delay(150);
}

// --- Ultrasonic Distance Measurement (cm) ---
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duration == 0) {
    return 999;
  }
  return duration * 0.034 / 2;
}

// --- Timer-Safe Piezo Buzzer ---
void beep(int freqHz, int durationMs) {
  long halfPeriodUs = 500000L / freqHz;
  long cycles = ((long)freqHz * durationMs) / 1000L;

  for (long i = 0; i < cycles; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(halfPeriodUs);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(halfPeriodUs);
  }
}

// --- Motor Control Functions ---
void setMotorSpeed(int speedA, int speedB) {
  analogWrite(ENA, constrain(speedA, 0, MAX_SPEED));
  analogWrite(ENB, constrain(speedB, 0, MAX_SPEED));
}

void moveForward() {
  setMotorSpeed(BASE_SPEED, BASE_SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  setMotorSpeed(BASE_SPEED, BASE_SPEED);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  setMotorSpeed(TURN_SPEED, TURN_SPEED);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  setMotorSpeed(TURN_SPEED, TURN_SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}