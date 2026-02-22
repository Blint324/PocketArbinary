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

// Define name to buzzer
#define buzzer 10

// Define variables
int bitPointer = 2; // Pointer we will use to select bits
int potentiometerValue = analogRead(A1); // Get potentiometer value
int potDivideAmount = 130; // The amount which we will be dividing in the analogPointer calculation mechanism.
int analogPointer = round(potentiometerValue / potDivideAmount + 2); // Convert the potentiometer range into an integer from 2 to 9
int usageType = 0; // Whether you're using the potentiometer to select LEDs, or the buttons to select LEDs. 0 and 1 respectively.
int valueArray[8] = {128, 64, 32, 16, 8, 4, 2, 1}; // Defines the array of values assigned to each led
int value = 0;

// Define functions
int beep(int length, int freq = 0, int led = 0, int ledState = 0) { // Define my awesome beeping function
  if (led == 0) { // If we're beeping for something OTHER than an LED
    tone(buzzer, freq); // Start beep
    delay(length); // Wait set amount of time
    noTone(buzzer); // Stop the beep
  } else { // If we ARE beeping for an LED
      int ledBeepArray[8] = {100, 1000, 900, 800, 700, 600, 500, 400}; // The array for sounds corresponding to LEDs
    
      // Beeping time!
      if (ledState == 0) { // If we're turning an LED on
        if (led == 8) { // Bandage fix... There's a problem with the increasing pitch and this is the only solution I could come up with.
          tone(buzzer, 300);
          delay(length);
          noTone(buzzer);
        } else {
          tone(buzzer, ledBeepArray[led]); // Beep at a pitch corresponding to the LED
          delay(length);
          noTone(buzzer);
        }

      } else { // If we're turning an LED off
        if (led == 8) { // Bandage fix yet again.
          tone(buzzer, 100);
          delay(length);
          noTone(buzzer);
        } else {
            tone(buzzer, ledBeepArray[led] - 200); // Beep at a pitch corresponding to the LED.. BUT LOWER!!!!!
            delay(length);
            noTone(buzzer);
      }
    }
  }
}

int checkButtons() {
  
  int toggleButtonState = 0;
  toggleButtonState = digitalRead(togglebutton); // Read toggle button state
  
  if (toggleButtonState == HIGH) { // If toggle button is on
    int parsePointer = 2; // Define pointer for calculating sum of bits
    if (digitalRead(bitPointer) == LOW) { // If LED at pointer is off
      digitalWrite(bitPointer, HIGH); // Turn on LED at pointer
      beep(100, 0, bitPointer - 1, 0); // Beep!

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
      beep(100, 0, bitPointer - 1, 0); // Beep! Again!
      
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
      beep(100, 1000);
      delay(200);
    } else { // If not
      analogWrite(bitPointer, 0); // Clear ghost effect
      bitPointer = 2; // Reset pointer
      beep(100, 1000);
      delay(200);
    }
  }
  
  int prevButtonState = 0;
  prevButtonState = digitalRead(prevbutton); // Read previous bit button state

  if (prevButtonState == HIGH) { // If previous bit button is on
    if (bitPointer > 2) { // If pointer is greater than 2
      bitPointer -= 1; // Decrease pointer by 1
      beep(100, 300);
      delay(200);
    } else { // If not
      analogWrite(bitPointer, 0); // Clear ghost effect
      bitPointer = 9; // Reset pointer
      beep(100, 300);
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
        beep(100, 0, analogPointer - 1, 0); // Beep!

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
      beep(100, 0, analogPointer - 1, 1); // Beep! Again!
      
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
      beep(100, 900);
      delay(200);
    } else {
      usageType = 0;
      beep(100, 500);
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
  analogPointer = round(potentiometerValue / potDivideAmount + 2); // Convert the potentiometer range into an integer from 0 to 7

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