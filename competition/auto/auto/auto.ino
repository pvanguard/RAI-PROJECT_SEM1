#include <Servo.h>

struct Ultrasonic {
  int trig;
  int echo;
};

Ultrasonic sensors[3] = {
  {2, 3},
  {4, 5},
  {6, 7}
};

#define ENA 9
#define IN1 8
#define IN2 10
#define ENB 11
#define IN3 12
#define IN4 13

Servo lift;
Servo grip;

const int LIFT_DOWN = 5;
const int LIFT_UP = 95;
const int GRIP_OPEN = 70;
const int GRIP_CLOSE = 10;
const int TURN_TIME = 400;
const int PICK_DISTANCE = 6;
const int OBSTACLE_DISTANCE = 5;
const int SPEED_FWD = 160;

bool *isCarrying = new bool(false);

void forward();
void back();
void stopM();
void rightTurn();
int readUS(Ultrasonic s);
void pickupSequence();

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(sensors[i].trig, OUTPUT);
    pinMode(sensors[i].echo, INPUT);
  }
  lift.attach(A0);
  grip.attach(A1);
  lift.write(LIFT_UP);
  grip.write(GRIP_OPEN);
  stopM();
}

void loop() {
  int distance[3];
  for (int i = 0; i < 3; i++) {
    distance[i] = readUS(sensors[i]);
  }
  if (!*isCarrying && distance[0] <= PICK_DISTANCE) {
    stopM(); delay(200);
    pickupSequence();
    *isCarrying = true;
  }
  else if (distance[0] <= OBSTACLE_DISTANCE) {
    stopM(); delay(100);
    back(); delay(250);
    stopM(); delay(100);
    rightTurn(); delay(TURN_TIME);
    stopM(); delay(120);
  }
  else {
    forward(); delay(100);
    stopM(); delay(10);
  }
}

int readUS(Ultrasonic s) {
  digitalWrite(s.trig, LOW); delayMicroseconds(2);
  digitalWrite(s.trig, HIGH); delayMicroseconds(10);
  digitalWrite(s.trig, LOW);
  long duration = pulseIn(s.echo, HIGH, 25000);
  int distance = duration * 0.034 / 2;
  if (distance == 0) distance = 400;
  return distance;
}

void stopM() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, SPEED_FWD);
  analogWrite(ENB, SPEED_FWD);
}

void back() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, SPEED_FWD);
  analogWrite(ENB, SPEED_FWD);
}

void rightTurn() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, SPEED_FWD);
  analogWrite(ENB, SPEED_FWD);
}

void pickupSequence() {
  lift.write(LIFT_DOWN);
  delay(300);
  grip.write(GRIP_CLOSE);
  delay(400);
  lift.write(LIFT_UP);
  delay(500);
}
