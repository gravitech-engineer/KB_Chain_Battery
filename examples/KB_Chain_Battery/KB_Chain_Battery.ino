/*************************************************************
   Name: Test_KB_Battery_Module
   Device: KidBrigth32 V1.3
   Chip: ESP32
   Compile: NodeMCU-32s
   Create: 25-5-18
   By: Nattkarn.P
   Email: arm_nattkan@hotmail.com
*************************************************************/
#include <Wire.h>
#include "KB_Battery.h"


byte i2cAddress = 0x4E;    // MCP3221 A5 in Dec 77 A0 = 72 A7 = 79)
//byte altI2CAddress = 0x0C // A0 = x48, A1 = x49, A2 = x4A, A3 = x4B,
// A4 = x4C, A5 = x4D, A6 = x4E, A7 = x4F
//This is for our ADC testing variables in mV ie 4948mV or 4.948v
const int I2CadcVRef = 4948; //Measured millivolts of voltage input to ADC (can measure VCC to ground on MinipH for example)
KB_BATTERY batt(i2cAddress, I2CadcVRef);

void setup()
{
  //The library assume you will initialize the wire library mine as well do this here
  Wire.begin(4, 5);
  //initialize and run anything that needs to be performed prior to the main loop executing for the first time
  Serial.begin(115200 );
}

void loop()
{
  int adcRaw, adcAVG, adcEXPAVG, adcCompare;
  float adcCal;
  //You can also use the MCP3221 library at scope level letting instances get created and destroyed as needed
  //Many instances can be ran as well to match the number of devices on the bus.)
  //MCP3221 secondI2CADC(altI2CAddress, I2CadcVRef);

  Serial.println("Grabbing Data");
  adcRaw = batt.readI2CADC();
  adcAVG = batt.calcRollingAVG(); //This accumulates over time dropping the oldest and shifting an array adding int he newest set for 10 in library .h can be upped keep in mind ram usage
  adcEXPAVG = batt.calcEMAVG();   //This approximates a rolling average using power and exponent via power of 2 to make it non floating friendly only needs a current and last so ram compact
  adcCompare = batt.readI2CADC(); //just shows that this will be different even if you have a very stable Vin and ADC in test voltage there is always noise :)
  adcCal = batt.calcMillivolts();
  Serial.println("|ADCRaw|ADCRollingAVG|ADCExpAVG|ADCCOMP|ADCCAL|");
  Serial.print("|");
  Serial.print(adcRaw);
  Serial.print("|");
  Serial.print(adcAVG);
  Serial.print("|");
  Serial.print(adcEXPAVG);
  Serial.print("|");
  Serial.print(adcCompare);
  Serial.print("|");
  Serial.print(adcCal);
  Serial.println("|");
  delay(500); //give us just a bit of time this can be adjust to your needs to test with better to use a non blocking method but this is a simple example
}

