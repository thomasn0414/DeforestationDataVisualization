#include <Servo.h>

// Define RGB pins for the first LED (common anode setup)
int redPin1 = 9;
int greenPin1 = 10;
int bluePin1 = 11;

// Define RGB pins for the second LED (common anode setup)
int redPin2 = 3;
int greenPin2 = 4;
int bluePin2 = 5;

// Define servo motor and button pins
int servoPin1 = 6;       // First servo motor
int servoPin2 = 7;       // Second servo motor
int buttonUpPin = 12;    // Button to increase years
int buttonDownPin = 13;  // Button to decrease years

// Variables
Servo myServo1;
Servo myServo2;
int years = 0;  // Initialize years to 0

// Deforestation thresholds for each region
const int AMAZON_THRESHOLD = 10;
const int SE_ASIA_THRESHOLD = 8;
const int CONGO_THRESHOLD = 20;

// Select region by setting the threshold
int selectedThreshold = AMAZON_THRESHOLD;

// Function to set color for both LEDs (common anode)
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin1, 255 - redValue);  
  analogWrite(greenPin1, 255 - greenValue);
  analogWrite(bluePin1, 255 - blueValue);

  analogWrite(redPin2, 255 - redValue);  
  analogWrite(greenPin2, 255 - greenValue);
  analogWrite(bluePin2, 255 - blueValue);
}

void setup() {
  // Set RGB pins for both LEDs as outputs
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);

  // Set button pins as inputs with internal pull-up resistors
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);

  // Attach the servos to their respective pins
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);

  // Start LEDs at green (healthy forest)
  setColor(0, 255, 0);

  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  Serial.println("Starting full code test...");
}

void checkButtons() {
  // Check if the "Increase" button is pressed
  if (digitalRead(buttonUpPin) == LOW) {
    if (years < selectedThreshold) {
      years++;  // Increase years if below max
      delay(250);  // Debounce delay
      Serial.print("Increase button pressed. Years: ");
      Serial.println(years);
    }
  }

  // Check if the "Decrease" button is pressed
  if (digitalRead(buttonDownPin) == LOW) {
    if (years > 0) {
      years--;  // Decrease years if above min
      delay(250);  // Debounce delay
      Serial.print("Decrease button pressed. Years: ");
      Serial.println(years);
    }
  }
}

void loop() {
  // Check buttons at the start of each loop
  checkButtons();

  // Check if years has reached the selected deforestation threshold
  if (years >= selectedThreshold) {
    // Turn off LEDs
    setColor(0, 0, 0);

    // Stop servo movement
    myServo1.detach();
    myServo2.detach();

    // Print message and exit loop
    Serial.println("Deforestation complete. LEDs off, servos stopped.");
    while (true) {
    }
  }

  // Set LED color based on years with clear ranges
  if (years < selectedThreshold / 3) {           // Less than one-third threshold
    setColor(0, 255, 0);                         // Green for healthy forest
  } else if (years >= selectedThreshold / 3 && years < (2 * selectedThreshold) / 3) { 
    setColor(255, 140, 0);                       // Orange for moderate deforestation
  } else if (years >= (2 * selectedThreshold) / 3 && years < selectedThreshold) {
    setColor(255, 0, 0);                         // Red for severe deforestation
  }

  // Map years to shake speed delay: slow shake for low years, fast shake as years approach threshold
  int shakeDelay = map(years, 0, selectedThreshold, 100, 10); // Faster shake as years increase

  // Gradually move both servos back and forth
  for (int angle = 70; angle <= 110; angle++) {
    myServo1.write(angle);          // Move first servo gradually
    myServo2.write(180 - angle);    // Move second servo in opposite direction
    delay(shakeDelay);              // Delay based on years
    checkButtons();                 // Check buttons during each servo movement
  }
  
  for (int angle = 110; angle >= 70; angle--) {
    myServo1.write(angle);          // Move first servo back
    myServo2.write(180 - angle);    // Move second servo back in opposite direction
    delay(shakeDelay);              // Delay based on years
    checkButtons();                 // Check buttons during each servo movement
  }
}


