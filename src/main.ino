// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h> // the OBD2 library depends on the CAN library
#include <OBD2.h>
#include <SPI.h>
#include <Wire.h>

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

    while (true) {
       Serial.println("Checking...");

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

    // Serial.println("-------------------------------------");

    
    float TFT = OBD2.pidReadExtra(0x1E,0x1C);
 //   Serial.print("TFT:");
  //  Serial.println(TFT);

    float ECT = OBD2.pidRead(0x05);
  //  Serial.print("ECT:");
  //  Serial.println(ECT);

    float RPM = OBD2.pidRead(0x0C);
  //  Serial.print("RPM:");
   // Serial.println(RPM);

    float AIT = OBD2.pidRead(0x0F);
    // Serial.print("AIT:");
    // Serial.println(AIT);

    float MAF = OBD2.pidRead(0x10);
    // Serial.print("MAF:");
    // Serial.println(MAF);

    float CMV = OBD2.pidRead(0x42);
    // Serial.print("CMV:");
    // Serial.println(CMV);



    pidName[0]=(char*)"ECT";
    pidValue[0] = ECT;

    pidName[1]=(char*)"TFT";
    pidValue[1] = TFT;

    pidName[2]=(char*)"RPM";
    pidValue[2] = RPM;

    pidName[3]=(char*)"AIT";
    pidValue[3] = AIT;

    pidName[4]=(char*)"MAF";
    pidValue[4] = MAF;

    pidName[5]=(char*)"CMV";
    pidValue[5] = CMV;
   
    displayGauge(pidName,pidValue);   

    delay(100);

    

}

void printPID(int pid) {
  // print PID name
  Serial.print(OBD2.pidName(pid));
  // Serial.print(F(" = "));

  // read the PID value
  float pidValue = OBD2.pidRead(pid);

  if (isnan(pidValue)) {
    // Serial.print("error");
  } else {
    // print value with units
    // Serial.print(pidValue);
    // Serial.print(F(" "));
    // Serial.print(OBD2.pidUnits(pid));
  }

  Serial.println();
}

void displayGauge(char **pidName, float pidValue[]) {

  Serial.print(pidName[0]);
  Serial.println(pidValue[0]);

}

