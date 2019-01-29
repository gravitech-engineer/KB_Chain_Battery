#include <Wire.h>
#include "KB_Battery.h"
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"


byte i2cAddress_battery = 0x4E;
byte i2cAddress_Dot = 0x70;
const int I2CadcVRef = 4948;
float temp_batt;

/***************************************************************/
KB_BATTERY batt(i2cAddress_battery, I2CadcVRef);
Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();
/********************* Wire Begin ******************************/
TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);
/***************************************************************/

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Read_batt();
  Display(temp_batt);
}

void Read_batt() {
  I2Cone.begin(4, 5);
  float adcCal;
  adcCal = batt.calcMillivolts();
  temp_batt = adcCal / 1024;
  Serial.println(temp_batt);
  I2Cone.endTransmission();
}

void Display(float CC)
{
  I2Ctwo.begin(21, 22);
  matrix.begin(i2cAddress_Dot);  // pass in the address
  String text = String(CC);
  matrix.setTextSize(1);
  matrix.setTextWrap(true);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.setRotation(1);
  for (int8_t x = 32; x >= -32; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(text);
    matrix.writeDisplay();
    delay(75);
  }
  I2Ctwo.endTransmission();
}



