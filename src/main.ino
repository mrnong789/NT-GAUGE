// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h> // the OBD2 library depends on the CAN library
#include <OBD2.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// array of P5ID's to print values of
const int PIDS[] = {
  CALCULATED_ENGINE_LOAD,
  ENGINE_COOLANT_TEMPERATURE,
  ENGINE_RPM,
  VEHICLE_SPEED,
  AIR_INTAKE_TEMPERATURE,
  MAF_AIR_FLOW_RATE,
  THROTTLE_POSITION,
  RUN_TIME_SINCE_ENGINE_START,
  FUEL_TANK_LEVEL_INPUT,
  ABSOLULTE_BAROMETRIC_PRESSURE,
  ABSOLUTE_LOAD_VALUE,
  RELATIVE_THROTTLE_POSITION
};

const int NUM_PIDS = sizeof(PIDS) / sizeof(PIDS[0]);


void setup() {
    Serial.begin(9600);

    while (!Serial);
    // init done

    lcd.begin();

    // Turn on the blacklight and print a message.
    lcd.backlight();
  
    while (true) {
      Serial.println("Checking...");
      lcd.print("Checking...");
        if (!OBD2.begin()) {
            // Serial.print("Connecting to OBDII Error!!");
            Serial.println("Connecting to OBDII Error!!");
            delay(1000);
        } else {
          Serial.println("Connect OBDII OK!!");
        break;
        }
    }
}



void loop() {
    char *pidName[4]={'\0'};
    pidName[0]=(char*)"ECT";
    pidName[1]=(char*)"TFT";
    pidName[2]=(char*)"RPM";
    pidName[3]=(char*)"EOT";

    float pidValue[4] = {};
    pidValue[0] = 0;
    pidValue[1] = 0;
    pidValue[2] = 0;
    pidValue[3] = 0;

    float TFT = OBD2.pidReadExtra(0x1E,0x1C);
    float ECT = OBD2.pidRead(0x05);
    float RPM = OBD2.pidRead(0x0C);
    // float AIT = OBD2.pidRead(0x0F);
    // float MAF = OBD2.pidRead(0x10);
    float CMV = OBD2.pidRead(0x42);

    pidName[0]=(char*)"ECT";
    pidValue[0] = ECT;

    pidName[1]=(char*)"TFT";
    pidValue[1] = TFT;

    pidName[2]=(char*)"RPM";
    pidValue[2] = RPM;

    pidName[3]=(char*)"CMV";
    pidValue[3] = CMV;
   
    displayGauge(pidName,pidValue);   

    delay(100); 

}

void displayGauge(char **pidName, float pidValue[]) {

  char valueTemp[10];
  int col = 0;
  int row = 0;
  for (int i = 1; i <= 4; i++) {

    if(i > 2) col = 9;
    row = i % 2;
    lcd.setCursor(col, row);
    lcd.print(String(pidName[i-1]));
    lcd.setCursor(col + 4, row);
    sprintf(valueTemp,"%f",pidValue[i-1]);
    lcd.print(valueTemp); 
   
  }

}