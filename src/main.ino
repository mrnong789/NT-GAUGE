// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h> // the OBD2 library depends on the CAN library
#include <OBD2.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_CS     5
#define OLED_DC     6
#define OLED_RESET  7
#define OLED_SDA    8
#define OLED_SCL    9

Adafruit_SSD1306 display(OLED_SDA, OLED_SCL, OLED_DC, OLED_RESET, OLED_CS);

// array of PID's to print values of
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

    display.begin(SSD1306_SWITCHCAPVCC);
    // init done

    // Clear the buffer.
    display.clearDisplay();


    display.setTextSize(2);
    display.setTextColor(WHITE); // 'inverted' text
    display.setCursor(10,0);
    display.clearDisplay();
    display.println("NT-GARAGE");
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println();
    display.println(">>FORD PERFORMANCE<<");
    display.display();
    delay(2000);
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0,0);
    display.setTextColor(WHITE); // 'inverted' text
    display.println("OBD2 Connecting...");
    display.display();

    // while (true) {
    //     if (!OBD2.begin()) {
    //     display.clearDisplay();
    //     display.setTextSize(1);
    //     display.setCursor(0,0);
    //     display.setTextColor(WHITE); // 'inverted' text
    //     display.println("OBD2 connect fail");
    //     display.display();

    //     delay(1000);
    //     } else {
    //      display.clearDisplay();
    //      display.setTextSize(1);
    //      display.setCursor(0,0);
    //      display.setTextColor(WHITE); // 'inverted' text
    //      display.println("OBD2 connect success!!");
    //      display.display();
    //     break;
    //     }
    // }
}

void loop() {
//   // loop through all the PID's in the array
//   // 
//   for (int i = 0; i < NUM_PIDS; i++) {
//     int pid = PIDS[i];

//     printPID(pid);
//   }
//   Serial.println();

//   delay(1000);

    char *pidName[6]={'\0'};
    pidName[0]=(char*)"ECT";
    pidName[1]=(char*)"TFT";
    pidName[2]=(char*)"RPM";
    pidName[3]=(char*)"EOT";
    pidName[4]=(char*)"DEV";
    pidName[5]=(char*)"DEV";

    float pidValue[6] = {};
    pidValue[0] = 0;
    pidValue[1] = 0;
    pidValue[2] = 0;
    pidValue[3] = 0;
    pidValue[4] = 0;
    pidValue[5] = 0;

    Serial.println("-------------------------------------");

    float ECT = OBD2.pidRead(0x0C);
    Serial.print("ECT:");
     Serial.println(ECT);

    float RPM = OBD2.pidRead(0x05);
    Serial.print("RPM:");
    Serial.println(RPM);

    float EOT = OBD2.pidRead(0x5C);
    Serial.print("EOT:");
    Serial.println(EOT);

    float MAF = OBD2.pidRead(0x10);
    Serial.print("MAF:");
    Serial.println(MAF);

    float CMV = OBD2.pidRead(0x42);
    Serial.print("CMV:");
    Serial.println(CMV);

    float TFT = OBD2.pidReadExtra(0x1E,0x1C);
    Serial.print("TFT:");
    Serial.println(TFT);

   
    


    pidName[0]=(char*)"ECT";
    pidValue[0] = ECT;

    pidName[1]=(char*)"TFT";
    pidValue[1] = TFT;

    pidName[2]=(char*)"RPM";
    pidValue[2] = RPM;

    pidName[3]=(char*)"EOT";
    pidValue[3] = EOT;

    pidName[4]=(char*)"MAF";
    pidValue[4] = MAF;

    pidName[5]=(char*)"CMV";
    pidValue[5] = CMV;
   
    


    displayGauge(pidName,pidValue);   

    delay(1000);


}

void printPID(int pid) {
  // print PID name
  Serial.print(OBD2.pidName(pid));
  Serial.print(F(" = "));

  // read the PID value
  float pidValue = OBD2.pidRead(pid);

  if (isnan(pidValue)) {
    Serial.print("error");
  } else {
    // print value with units
    Serial.print(pidValue);
    Serial.print(F(" "));
    Serial.print(OBD2.pidUnits(pid));
  }

  Serial.println();
}

void displayGauge(char **pidName, float pidValue[]) {
    display.clearDisplay();
    display.fillRect(64, 10, 1, 22, 1);
    display.fillRect(0, 10, 128, 1, 1);

    // text row 1 1
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(pidName[0]);
    display.setCursor(25,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[0]);

     // text row 1 2
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(72,0);
    display.print(pidName[1]);
    display.setCursor(72+25,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[1]);
  

    // text row 2 1
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,12);
    display.print(pidName[2]);
    display.setCursor(0+25,12);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[2]);


     // text row 2 2
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(72,12);
    display.print(pidName[3]);
    display.setCursor(72+25,12);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[3]);
   


     // text row 3 1
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,22);
    display.print(pidName[4]);
    display.setCursor(0+25,22);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[4]);
  

     // text row 3 2
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(72,22);
    display.print(pidName[5]);
    display.setCursor(72+25,22);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[5]);
    display.display();

}

