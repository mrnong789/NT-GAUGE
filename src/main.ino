// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h> // the OBD2 library depends on the CAN library
#include <OBD2.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);


byte battertyIcon[] = {
  0x10,
  0x18,
  0x18,
  0x1F,
  0x1F,
  0x03,
  0x03,
  0x01
};

byte ectIcon[] = {
  0x0E,
  0x08,
  0x0E,
  0x08,
  0x0E,
  0x08,
  0x0C,
  0x0C
};

byte tftIcon[] = {
  0x00,
  0x00,
  0x04,
  0x0E,
  0x1B,
  0x0E,
  0x04,
  0x00
};


void setup() {
    Serial.begin(9600);

    while (!Serial);
    // init done

    lcd.begin();
    lcd.createChar(0, battertyIcon);
    lcd.createChar(1, ectIcon);
    lcd.createChar(2, tftIcon);

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
    char *pidName[3]={'\0'};
    pidName[0]=(char*)"ECT";
    pidName[1]=(char*)"TFT";
    pidName[2]=(char*)"BAT";

    float pidValue[3] = {};
    pidValue[0] = 0;
    pidValue[1] = 0;
    pidValue[2] = 0;

    float TFT = OBD2.pidReadExtra(0x1E,0x1C);
    float ECT = OBD2.pidRead(0x05);
    float BAT = OBD2.pidRead(0x42);

    pidName[0]=(char*)"ECT";
    pidValue[0] = ECT;

    pidName[1]=(char*)"TFT";
    pidValue[1] = TFT;

    pidName[2]=(char*)"BAT";
    pidValue[2] = BAT;
   
    displayGauge(pidName,pidValue);   

    delay(100); 

}

void displayGauge(char **pidName, float pidValue[]) {

  char valueTemp[10];

  for (int i = 0; i < 3; i++) {
    //Text
    lcd.setCursor((i*5), 0);
    lcd.print(String(pidName[i]));

    //icon
    lcd.setCursor(((i*5)+3), 0);
    lcd.write(i);

    //value
    lcd.setCursor((i*5), 1);
    //pidValue[i]
    sprintf(valueTemp,"%2.2f",(double)pidValue[i]);
    lcd.print(valueTemp); 
  }
  

}