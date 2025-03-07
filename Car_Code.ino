// Car code

// Motor Driver Pins
#define ENA 18  
#define ENB 5
#define IN1 19
#define IN2 21
#define IN3 17
#define IN4 16

// Ultrasonic Sensor Pins
#define TRIG 25  
#define ECHO 26  

// Control Constants
const int STOP_DISTANCE = 15; // Distance threshold in cm
const int MAX_SPEED = 150;    // Adjust speed (0-255)

void setup() {
  Serial.begin(115200);
  
  // Motor Control Pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic Sensor Pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.println("Car Ready");
}

void loop() {
  stopMotorsWhileMeasuring();  // Stop motors before taking measurement
  
  int distance = getDistance(); // Get ultrasonic sensor reading
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  resumeMotors();  // Resume motors after measurement
  
  if (distance > STOP_DISTANCE) {
    moveForward(MAX_SPEED);
  } else {
    stopCar();  // Directly stop if an obstacle is close
  }

  delay(100);
}

// Function to measure distance using Ultrasonic Sensor
int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000); // 30ms timeout
  int distance = duration * 0.034 / 2; // Convert to cm

  if (duration == 0) {
    Serial.println("Error - No Echo Received!");
    return 999; // Return high value if sensor fails
  }

  return distance;
}

// Function to move the car forward
void moveForward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

// Function to completely stop the car
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// Stop motors before taking ultrasonic measurement
void stopMotorsWhileMeasuring() {
  stopCar();
  noInterrupts();  // Disable interrupts to prevent interference
}

// Resume motor operation after measurement
void resumeMotors() {
  interrupts();  // Enable interrupts again
}