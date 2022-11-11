#include <Mouse.h>

#define echoPin 2
#define trigPin 3
#define butPin 4
#define MOV_DELTA 50
#define ERROR_CONTROL 0.5

float prev_distance;
float distance;
float diff_distance;
int buttonState;
int mov;

float get_distance(int trig, int echo) {
  float dist;
  int dur;
  // Clears the trigPin condition
  digitalWrite(trig, LOW);
  delayMicroseconds(2); 
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  noInterrupts();
  dur = pulseIn(echo, HIGH);
  interrupts();
  // Calculating the distance
  dist = dur * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  return dist;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(butPin, INPUT);
  Serial.begin(9600);
  Mouse.begin();

  prev_distance = get_distance(trigPin, echoPin);
}
void loop() {
  buttonState = digitalRead(butPin);
  distance = get_distance(trigPin, echoPin);
  
  Serial.print("ButtonState: ");
  if (buttonState == 1) {
    Serial.println("pressed");
  } else {
    Serial.println("Not pressed");
  }

  //if (buttonState == 1) {Mouse.press();} else {Mouse.release();}

  diff_distance = prev_distance - distance;
  // Ugly error control lol
  if ((diff_distance > 0.0 && diff_distance < ERROR_CONTROL) || (diff_distance < 0.0 && diff_distance > -ERROR_CONTROL)) {diff_distance = 0.0;}
  mov = int(diff_distance * MOV_DELTA);

  Serial.print("Current distance: ");
  Serial.print(distance);
  Serial.print(", difference: ");
  Serial.print(diff_distance);
  Serial.print(", effect: ");
  Serial.println(mov);

  Mouse.move(0, mov, 0);

  prev_distance = distance;

  delay(50);
}
