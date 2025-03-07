// Robotic arm

// new code for robotic arm 

#include <ESP32Servo.h>

// Servo motor objects
Servo baseServo;     // MG995 (Base rotation)
Servo shoulderServo; // MG995 (Shoulder)
Servo elbowServo;    // MG995 (Elbow)
Servo wristRoll;     // SG90 (Wrist roll)
Servo wristPitch;    // SG90 (Wrist pitch)
Servo gripperServo;  // SG90 (Gripper)

// Define pin connections (Adjust according to your wiring)
const int basePin = 13;
const int shoulderPin = 14;
const int elbowPin = 15;
const int wristRollPin = 16;
const int wristPitchPin = 17;
const int gripperPin = 18;

// Object pick & drop positions (Adjust for your setup)
const int basePickPos = 90;
const int baseDropPos = 180;
const int shoulderPickPos = 120;
const int shoulderDropPos = 80;
const int elbowPickPos = 60;
const int elbowDropPos = 90;
const int wristRollNeutral = 90;
const int wristPitchNeutral = 90;
const int gripperOpen = 90;
const int gripperClosed = 40;

void setup() {
    Serial.begin(115200);

    // Attach servos to the respective GPIO pins
    baseServo.attach(basePin);
    shoulderServo.attach(shoulderPin);
    elbowServo.attach(elbowPin);
    wristRoll.attach(wristRollPin);
    wristPitch.attach(wristPitchPin);
    gripperServo.attach(gripperPin);

    // Set servos to the home position
    homePosition();
    delay(1000);
}

void loop() {
    // Pick up object
    pickUpObject();
    delay(2000);

    // Rotate base 90 degrees
    moveBase(90);
    delay(2000);

    // Drop the object
    dropObject();
    delay(2000);

    // Move arm back to home position
    homePosition();
    delay(3000);
}

// Function to move servos to home position
void homePosition() {
    baseServo.write(90);          // Center base
    shoulderServo.write(90);      // Neutral shoulder
    elbowServo.write(90);         // Neutral elbow
    wristRoll.write(90);          // Neutral wrist roll
    wristPitch.write(90);         // Neutral wrist pitch
    gripperServo.write(gripperOpen); // Open gripper
}

// Function to pick up an object
void pickUpObject() {
    Serial.println("Picking up object...");

    baseServo.write(basePickPos);
    shoulderServo.write(shoulderPickPos);
    elbowServo.write(elbowPickPos);
    wristRoll.write(wristRollNeutral);
    wristPitch.write(wristPitchNeutral);
    delay(1000);

    // Close gripper to grab object
    gripperServo.write(gripperClosed);
    delay(1000);

    // Lift object
    shoulderServo.write(shoulderPickPos - 20);
    elbowServo.write(elbowPickPos + 20);
    delay(1000);
}

// Function to move base rotation
void moveBase(int degrees) {
    Serial.println("Rotating base...");
    baseServo.write(degrees);
    delay(1000);
}

// Function to drop an object
void dropObject() {
    Serial.println("Dropping object...");

    baseServo.write(baseDropPos);
    shoulderServo.write(shoulderDropPos);
    elbowServo.write(elbowDropPos);
    wristRoll.write(wristRollNeutral);
    wristPitch.write(wristPitchNeutral);
    delay(1000);

    // Open gripper to release object
    gripperServo.write(gripperOpen);
    delay(1000);
}