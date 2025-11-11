//obstalce avoidance robot
#define TRIG_PIN 17
#define ECHO_PIN 18

long duration;
int distance;

// Motor pins
int enable1 = 16, motor1pin1 = 15, motor1pin2 = 7;
int enable2 = 4, motor2pin1 = 5, motor2pin2 = 6;

// PWM config
const int freq = 30000;
const int resolution = 8;
const int dutyCycle = 200;

void setup() {
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(enable1, OUTPUT);

  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(enable2, OUTPUT);

  ledcAttach(enable1, freq, resolution);
  ledcAttach(enable2, freq, resolution);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);
}

// === Movement Functions ===
void driveForward() {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  ledcWrite(enable1, dutyCycle);
  ledcWrite(enable2, dutyCycle);
}

void stopMotors() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  ledcWrite(enable1, 0);
  ledcWrite(enable2, 0);
}

void driveBackward(int duration_ms) {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  ledcWrite(enable1, dutyCycle);
  ledcWrite(enable2, dutyCycle);
  delay(duration_ms);
  stopMotors();
  delay(100);
}

void turnLeft(int duration_ms) {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  ledcWrite(enable1, 170);
  ledcWrite(enable2, 170);
  delay(duration_ms);
  stopMotors();
  delay(150);
}

void turnRight(int duration_ms) {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  ledcWrite(enable1, 170);
  ledcWrite(enable2, 170);
  delay(duration_ms);
  stopMotors();
  delay(150);
}

int getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

void loop() {
  distance = getDistanceCM();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < 30) {
    stopMotors();

    // Step 1: Reverse
    driveBackward(400);

    // Step 2: Turn Left Longer and Measure
    turnLeft(900);
    int leftDistance = getDistanceCM();
    Serial.print("Left Distance: ");
    Serial.println(leftDistance);

    // Step 3: Return to Center
    turnRight(900);

    // Step 4: Turn Right Longer and Measure
    turnRight(900);
    int rightDistance = getDistanceCM();
    Serial.print("Right Distance: ");
    Serial.println(rightDistance);

    // Step 5: Return to Center
    turnLeft(900);

    // Step 6: Decide Best Direction
    if (rightDistance > leftDistance) {
      turnRight(900);
    } else {
      turnLeft(900);
    }
  }'/'

  // Step 7: Continue Forward
  driveForward();
  delay(100);
}
