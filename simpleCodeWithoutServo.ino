const int trigPin = 13;  // Ultrasonic sensor Trigger Pin
const int echoPin = 12;  // Ultrasonic sensor Echo Pin

// Motor control pins: L298N H-bridge
const int enAPin = 6;   // Left motor PWM speed control
const int in1Pin = 7;   // Left motor Direction 1
const int in2Pin = 5;   // Left motor Direction 2
const int in3Pin = 4;   // Right motor Direction 1
const int in4Pin = 2;   // Right motor Direction 2
const int enBPin = 3;   // Right motor PWM speed control

long duration;
int distance;

void setup() {
  Serial.begin(9600);

  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  moveForward();
}

void loop() {
  distance = measureDistance();
  Serial.println("Distance: " + String(distance) + " cm");

  if (distance < 30) { 
    Serial.println("Obstacle detected! Moving backward...");
    moveBackward(10);
    
    int distanceRight = checkDirection("right");
    delay(250);

    if(distanceRight > 30) {
      Serial.println("Turning right...");
      turnRight();
    } else {
      comebackToCenter("right");
      delay(250);
      turnLeft();
    }  
  } else {
    Serial.println("Moving forward...");
    moveForward();
  }

  delay(100);  // Small delay to stabilize readings
}

// Measure distance using the ultrasonic sensor
unsigned int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long period = pulseIn(echoPin, HIGH);
  return period * 0.0343 / 2;  // Convert to centimeters
}

// Check distance in a specific direction (left or right)
int checkDirection(String direction) {
  if (direction == "right") {
    Serial.println("Checking right...");
    turnRightReverse();
  } else if (direction == "left") {
    Serial.println("Checking left...");
    turnLeftReverse();
  }

  int measuredDistance = measureDistance();
  stopMotors();  // Stop motors after checking
  return measuredDistance;
}

void turnRightReverse() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, 255);

  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enBPin, 255);
}

void comebackToCenter(String direction) {
  if (direction == "right") {
    turnLeft();
  } else {
    turnRight();
  } 
}

void turnLeftReverse() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, 255);

  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enBPin, 255);
}

// Move forward
void moveForward() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, 255);

  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enBPin, 255);
}

// Move backward for a specified distance in cm
void moveBackward(int cm) {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enAPin, 255);

  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enBPin, 255);

  delay(cm * 60);  // Adjust delay based on distance
  stopMotors();
}

// Turn left
void turnLeft() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, 0);

  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enBPin, 255);

  delay(500);  // Adjust based on required turn
  stopMotors();
}

// Turn right
void turnRight() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, 255);

  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  analogWrite(enBPin, 0);

  delay(500);  // Adjust based on required turn
  stopMotors();
}

void stopMotors() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, 0);

  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  analogWrite(enBPin, 0);
}
