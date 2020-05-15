#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

float CTRLA = -0.005;
float CTRLB = 1.5688;
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


Adafruit_VL6180X vl = Adafruit_VL6180X();
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

void setup() {
  Serial.begin(115200);

  Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
  lcd.begin(16, 2);
}

void loop() {

  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    //Serial.print("range: "); Serial.println((range*CTRLA)+CTRLB);
    Serial.println(range);
    // subtract the last reading:
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = range;
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }

    // calculate the average:
    average = total / numReadings;
    // send it to the computer as ASCII digits
    //Serial.println(average);
    //Serial.println((average*CTRLA)+CTRLB);

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    // Print a message to the LCD. We track how long it takes since
    lcd.print("AVG FOV ");
    lcd.print((average * CTRLA) + CTRLB);
    lcd.print("mm");

    uint8_t i = 0;
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the range:
    lcd.print(range);
    lcd.print("mm");
    uint8_t buttons = lcd.readButtons();

    delay(500);
  }
}
