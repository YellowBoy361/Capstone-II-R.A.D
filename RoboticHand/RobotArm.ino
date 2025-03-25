#include <Servo.h>
#define numOfValsRec 5 // Determines the number of values received from the Raspberry Pi 4. (5 fingers)
#define digitsPerValueRec 1 // Digits per value recieved in binary. (1 digit for each value)

/* Objects initialized for the servo's individual fingers. */
Servo servoThumb;
Servo servoIndex;
Servo servoMiddle;
Servo servoRing;
Servo servoPinky;

/* Initialized variables. */
int valsRec[numOfValsRec]; // An array to store the individual values received from the Python script.
int stringLength = numOfValsRec * digitsPerValueRec + 1; // $00000
int counter  = 0; // Necessary to check each digit in a loop.
bool counterStart =  false; // Used in the loop to determine when counter is finished.
String receivedString; // Information recieved from the Python script.

/* Runs on startup and determines the Arduino's operation. */
void setup() {
  Serial.begin(9600); // Determines the clock rate of the Arduino.
  servoThumb.attach(0); // Thumb servo is attached to DIO0 on the Arduino.
  servoIndex.attach(2); // Index finger servo is attached to DIO2 on the Arduino.
  servoMiddle.attach(4); // Middle finger servo is attached to DIO4 on the Arduino.
  servoRing.attach(6); // Ring finger servo is attached to DIO6 on the Arduino.
  servoPinky.attach(8); // Pinky finger servo is attached to DIO8 on the Arduino.
}

/* A function used to parse the data received from the serial input. */
void receiveData() {
  while(Serial.available()) { // While the serial port is active...

    char c = Serial.read(); // Take a bit of data from the serial.

    if (c == '$') { // If the serial input begins with a $... ($ determines the beginning of data being read)
      counterStart = true; // Start the counter.
    }

    if (counterStart) { // If the counter is active...

      if (counter < stringLength) { // ... so long as the end of the string has not been reached...
        receivedString = String(receivedString + c); //  Place the current bit of data receieved into receivedString.
        counter++; // Add to the counter.
      }

      if (counter >= stringLength) { // After the end of the serial input has been reached...

        for (int i = 0; i < numOfValsRec; i++)  { // ... for numOfValsRec iterations...
          int num = i * digitsPerValueRec + 1; // Create a variable to determine the current digit of recieved data.
          valsRec[i] = receivedString.substring(num, num + digitsPerValueRec).toInt(); // Convert the recieved bit into an int for the servo.
        }

        receivedString = ""; // Reset string for repeat.
        counter = 0; // Reset counter for repeat.
        counterStart = false; // Reset counterStart for repeat.
      }
    }
  }
}

/* Runs automatically and sends information to the servos through the pins. */
void loop() {
  receiveData();// Receive the data.
  if (valsRec[0] == 1) {servoThumb.write(180);} else {servoThumb.write(0);}  // Send the command to the thumb.
  if (valsRec[1] == 1) {servoIndex.write(0);} else {servoIndex.write(180);}  // Send the command to the index finger.
  if (valsRec[2] == 1) {servoMiddle.write(0);} else {servoMiddle.write(180);}  // Send the command to the middle finger.
  if (valsRec[3] == 1) {servoRing.write(180);} else {servoRing.write(0);}  // Send the command to the ring finger.
  if (valsRec[4] == 1) {servoPinky.write(0);} else {servoPinky.write(180);}  // Send the command to the pinky finger.
}