// Define names to all of the LED pins
#define bit1led 2
#define bit2led 3
#define bit3led 4
#define bit4led 5
#define bit5led 6
#define bit6led 7
#define bit7led 8
#define bit8led 9

// Define names to all button pins
#define togglebutton 22
#define nextbutton 23
#define prevbutton 24
#define pottogglebutton 25
#define typeswitchbutton 26

// Define variables
int bitPointer = 2; // Pointer we will use to select bits
int potentiometerValue = analogRead(A1); // Get potentiometer value
int analogPointer = round(potentiometerValue / 145 + 2); // Convert the potentiometer range into an integer from 2 to 9
int usageType = 0; // Whether you're using the potentiometer to select LEDs, or the buttons to select LEDs. 0 and 1 respectively.
int valueArray[8] = {1, 2, 4, 8, 16, 32, 64, 128}; // Defines the array of values assigned to each led
int value = 0;

// Define functions
int checkButtons() {
  int toggleButtonState = 0;
  toggleButtonState = digitalRead(togglebutton); // Read toggle button state
  
  if (toggleButtonState == HIGH) { // If toggle button is on
    int parsePointer = 2; // Define pointer for calculating sum of bits
    if (digitalRead(bitPointer) == LOW) { // If LED at pointer is off
      digitalWrite(bitPointer, HIGH); // Turn on LED at pointer

      // Parsing bits and summing them
      for (int i=0; i<=8; i++) {
        if (digitalRead(parsePointer) == HIGH) {
          // Add value corresponding to LED
          value += valueArray[parsePointer - 2];
        }
        parsePointer += 1;
      }
      Serial.println(value);
      
      value = 0;
      delay(200);

    } else { // If not
      digitalWrite(bitPointer, LOW); // Turn off LED at pointer
      
      // Parsing bits and summing them
      for (int i=0; i<=8; i++) {
        if (digitalRead(parsePointer) == HIGH) {
          // Add value corresponding to LED
          value += valueArray[parsePointer - 2];
        }
        parsePointer += 1;
      }
      Serial.println(value);

      value = 0;
      delay(200);

    }
  }
  
  int nextButtonState = 0;
  nextButtonState = digitalRead(nextbutton); // Read next bit button state

  if (nextButtonState == HIGH) { // If next bit button is on
    if (bitPointer < 9) { // If pointer is less than 9
      bitPointer += 1; // Increase pointer by 1
      delay(200);
    } else { // If not
      analogWrite(bitPointer, 0);
      bitPointer = 2; // Reset pointer
      delay(200);
    }
  }
  
  int prevButtonState = 0;
  prevButtonState = digitalRead(prevbutton); // Read previous bit button state

  if (prevButtonState == HIGH) { // If previous bit button is on
    if (bitPointer > 2) { // If pointer is greater than 2
      bitPointer -= 1; // Decrease pointer by 1
      delay(200);
    } else { // If not
      analogWrite(bitPointer, 0); // Clear ghost effect
      bitPointer = 9; // Reset pointer
      delay(200);
    }
  }
}

int checkPotButtons() { // Function to check the potentiometer button
  int potToggleButtonState = digitalRead(pottogglebutton); // Define state for the potentiometers toggle button as... well... I dont have to say it, right?

  if (potToggleButtonState == HIGH) { // Dude, you read the last function. I'm sure you did. I don't have to explain.
    if (usageType == 1) { // If we're using the potentiometer mode
      
      // Time to steal code from the earlier function!
      
      int parsePointer = 2; // Define pointer for calculating sum of bits
      if (digitalRead(analogPointer) == LOW) { // If LED at pointer is off
        digitalWrite(analogPointer, HIGH); // Turn on LED at pointer

        // Parsing bits and summing them
        for (int i=0; i<=8; i++) {
          if (digitalRead(parsePointer) == HIGH) {
            // Add value corresponding to LED
            value += valueArray[parsePointer - 2];
          }
          parsePointer += 1;
        }
        Serial.println(value);
      
        value = 0;
        delay(200);
    } else { // If not
      digitalWrite(analogPointer, LOW); // Turn off LED at pointer
      
      // Parsing bits and summing them
      for (int i=0; i<=8; i++) {
        if (digitalRead(parsePointer) == HIGH) {
          // Add value corresponding to LED
          value += valueArray[parsePointer - 2];
        }
        parsePointer += 1;
      }
      Serial.println(value);

      value = 0;
      delay(200);
      }
    }
  }

  // Onto the usage type switch button now!
  int typeSwitchButtonState = digitalRead(typeswitchbutton);

  if (typeSwitchButtonState == HIGH) {
    if (usageType == 0) {
      usageType = 1;
      delay(200);
    } else {
      usageType = 0;
      delay(200);
    }
  }
}

void setup() {
  // Start serial communication with PC
  Serial.begin(9600);

  // Define all bit LEDs as output
  for (int i=2; i<=9; i++) {
    pinMode(i, OUTPUT);
  }

  // Define all buttons as input
  pinMode(22, INPUT);
  pinMode(23, INPUT);
  pinMode(24, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  
  // Turn all LEDs off
  for (int i=2; i<=9; i++) {
    digitalWrite(i, LOW);
  }
  
}

void loop() {
  potentiometerValue = analogRead(A1); // Get potentiometer value
  analogPointer = round(potentiometerValue / 145 + 2); // Convert the potentiometer range into an integer from 0 to 7

  checkPotButtons();

  if (usageType == 0) {
    checkButtons();
  }

  if (usageType == 0) {
    if (digitalRead(bitPointer) == LOW) { // Ghost effect
      analogWrite(bitPointer, 1);
      delay(40);
      analogWrite(bitPointer, 0);
      delay(40);
    }
  } else {
    if (digitalRead(analogPointer) == LOW) { // Ghost effect... BUT WITH A POTENTIOMETER!!!!!!!
      analogWrite(analogPointer, 1);
      delay(40);
      analogWrite(analogPointer, 0);
      delay(40);
    }
  }

}
