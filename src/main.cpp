#include <Arduino.h>
#include <AccelStepper.h>

// Constants for stepper motor connections and end switches
// STEPPER 1 [MAGENTA]
#define DIR_PIN_1 7
#define STEP_PIN_1 6
#define END_SWITCH_PIN_1 22
#define POTENTIOMETER_PIN_1 A2

// STEPPER 2 [YELLOW]
#define DIR_PIN_2 8
#define STEP_PIN_2 9
#define END_SWITCH_PIN_2 23
#define POTENTIOMETER_PIN_2 A1

// STEPPER 3 [CYAN]
#define DIR_PIN_3 10
#define STEP_PIN_3 11
#define END_SWITCH_PIN_3 24
#define POTENTIOMETER_PIN_3 A0

// Stepper motor objects
AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_1, DIR_PIN_1);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_2, DIR_PIN_2);
AccelStepper stepper3(AccelStepper::DRIVER, STEP_PIN_3, DIR_PIN_3);

// Constants for stepper motors
const int stepsPerRevolution = 400 * 16; // Default 1/16 microstepping (steps per revolution)

// Custom range for each motor (in steps)
const int minRange1 = 0;
const int maxRange1 = stepsPerRevolution + (80 * 16); // additional 72 degrees for motor 1

const int minRange2 = 0;
const int maxRange2 = stepsPerRevolution + (95 * 16); // additional 72 degrees for motor 2

const int minRange3 = 0;
const int maxRange3 = stepsPerRevolution + (80 * 16); // additional 72 degrees for motor 3

// Target positions for motors
long targetPosition1 = 0;
long targetPosition2 = 0;
long targetPosition3 = 0;

// Function declarations
void findHome(AccelStepper &stepper, int endSwitchPin);

void setup() {
  // Pin configurations for end switches
  pinMode(END_SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(END_SWITCH_PIN_2, INPUT_PULLUP);
  pinMode(END_SWITCH_PIN_3, INPUT_PULLUP);

  // Stepper motor settings
  stepper1.setMaxSpeed(4500);
  stepper2.setMaxSpeed(4750);
  stepper3.setMaxSpeed(5000);

  stepper1.setAcceleration(1800);
  stepper2.setAcceleration(1900);
  stepper3.setAcceleration(2000);

  // Find home positions for each motor
  findHome(stepper1, END_SWITCH_PIN_1);
  findHome(stepper2, END_SWITCH_PIN_2);
  findHome(stepper3, END_SWITCH_PIN_3);
}

void loop() {
  // Read potentiometer values and map to custom ranges
  int potValue1 = analogRead(POTENTIOMETER_PIN_1);
  targetPosition1 = map(potValue1, 0, 1023, minRange1, maxRange1);

  int potValue2 = analogRead(POTENTIOMETER_PIN_2);
  targetPosition2 = map(potValue2, 0, 1023, minRange2, maxRange2);

  int potValue3 = analogRead(POTENTIOMETER_PIN_3);
  targetPosition3 = map(potValue3, 0, 1023, minRange3, maxRange3);

  // Set target positions
  stepper1.moveTo(targetPosition1);
  delayMicroseconds(100); // Small delay to desynchronize
  stepper1.run();

  stepper2.moveTo(targetPosition2);
  delayMicroseconds(100); // Small delay to desynchronize
  stepper2.run();

  stepper3.moveTo(targetPosition3);
  delayMicroseconds(100); // Small delay to desynchronize
  stepper3.run();
}

// Function to find the home position for a stepper motor
void findHome(AccelStepper &stepper, int endSwitchPin) {
  // Reduce speed for precise detection
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(5000);

  // Move until end switch is triggered
  while (digitalRead(endSwitchPin) == HIGH) {
    stepper.moveTo(stepper.currentPosition() - 16);
    stepper.run();
  }

  // Set current position to 0
  stepper.setCurrentPosition(0);
  stepper.stop();
}