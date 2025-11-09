#include <Servo.h>

#define TRIG_F 2
#define ECHO_F 3

#define TRIG_L 4
#define ECHO_L 5

#define TRIG_R 6
#define ECHO_R 7

#define ENA 9
#define IN1 8
#define IN2 10

#define ENB 11
#define IN3 12
#define IN4 13

#define PIN_LIFT A0
#define PIN_GRIP A1

const int LIFT_DOWN  = 5;
const int LIFT_UP    = 95;

const int GRIP_OPEN  = 70;
const int GRIP_CLOSE = 10;

const int PICK_CM   = 6;
const int BLOCK_CM  = 5;

const int SPEED_FULL = 255;

Servo lift;
Servo grip;

bool carrying = false;

void stopM() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED_FULL);
  analogWrite(ENB, SPEED_FULL);
}

void back() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, SPEED_FULL);
  analogWrite(ENB, SPEED_FULL);
}

void rightTurn() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED_FULL);
  analogWrite(ENB, SPEED_FULL);
}

int readUS(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 20000);
  int distance = duration * 0.034 / 2;

  if (distance <= 0) distance = 400;
  return distance;
}

void clawOpen() {
  grip.write(GRIP_OPEN);
  delay(250);
}

void clawClose() {
  grip.write(GRIP_CLOSE);
  delay(300);
}

void liftDown() {
  lift.write(LIFT_DOWN);
  delay(500);
}

void liftUp() {
  lift.write(LIFT_UP);
  delay(500);
}

void pickupSequence() {
  forward();
  delay(200);
  stopM();
  delay(100);

  liftDown();
  delay(100);

  clawClose();
  delay(200);

  liftUp();
  delay(150);

  carrying = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopM();

  lift.attach(PIN_LIFT);
  grip.attach(PIN_GRIP);

  liftUp();
  clawOpen();

  Serial.println("Autonomous Robot: Vacuum + Pickup Mode Ready");
}

void loop() {
  int front = readUS(TRIG_F, ECHO_F);
  int left  = readUS(TRIG_L, ECHO_L);
  int right = readUS(TRIG_R, ECHO_R);

  Serial.print("F: ");
  Serial.print(front);
  Serial.print("  L: ");
  Serial.print(left);
  Serial.print("  R: ");
  Serial.println(right);

  if (!carrying && front <= PICK_CM) {
    stopM();
    delay(100);
    pickupSequence();
    stopM();
    delay(150);
    return;
  }

  if (front <= BLOCK_CM) {
    stopM();
    delay(100);
    back();
    delay(250);
    stopM();
    delay(100);
    rightTurn();
    delay(400);
    stopM();
    delay(200);
    return;
  }

  forward();
  delay(120);
  stopM();
  delay(30);
}
