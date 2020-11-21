# 1 "C:\\Users\\MRNONG~1\\AppData\\Local\\Temp\\tmp6vcrobm7"
#include <Arduino.h>
# 1 "D:/nt-garage-gauge/NT-GAUGE/NT-GAUGE/src/main.ino"



#include <CAN.h>
#include <OBD2.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_CS 5
#define OLED_DC 6
#define OLED_RESET 7
#define OLED_SDA 8
#define OLED_SCL 9

Adafruit_SSD1306 display(OLED_SDA, OLED_SCL, OLED_DC, OLED_RESET, OLED_CS);


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
void setup();
void loop();
void printPID(int pid);
void displayGauge(char **pidName, float pidValue[]);
#line 37 "D:/nt-garage-gauge/NT-GAUGE/NT-GAUGE/src/main.ino"
void setup() {
    Serial.begin(9600);

    while (!Serial);

    display.begin(SSD1306_SWITCHCAPVCC);



    display.clearDisplay();


    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,0);
    display.println("NT-GARAGE");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println();
    display.println(">>Ford Performance<<");
    display.display();
    delay(2000);
    display.clearDisplay();


    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,0);
    display.println("NT-GARAGE");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println();
    display.println("Connecting OBDII....");
    display.display();

    while (true) {
       Serial.println("xxxxxx");

        if (!OBD2.begin()) {


        delay(1000);
        } else {
           display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(10,0);
            display.clearDisplay();
            display.println("NT-GARAGE");
            display.display();

            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.println();
            display.println("Connect OBDII OK!!");
            display.display();
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




    float TFT = OBD2.pidReadExtra(0x1E,0x1C);



    float ECT = OBD2.pidRead(0x05);



    float RPM = OBD2.pidRead(0x0C);



    float AIT = OBD2.pidRead(0x0F);



    float MAF = OBD2.pidRead(0x10);



    float CMV = OBD2.pidRead(0x42);





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

  Serial.print(OBD2.pidName(pid));



  float pidValue = OBD2.pidRead(pid);

  if (isnan(pidValue)) {

  } else {




  }

  Serial.println();
}

void displayGauge(char **pidName, float pidValue[]) {
    display.clearDisplay();
    display.fillRect(64, 10, 1, 22, 1);
    display.fillRect(0, 10, 128, 1, 1);


    uint8_t icon = 30;
    if(millis() % 2 == 0){
        icon = 31;
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(62,0);
    display.write(icon);


    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(pidName[0]);
    display.setCursor(25,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[0]);


    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(72,0);
    display.print(pidName[1]);
    display.setCursor(72+25,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[1]);



    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,12);
    display.print(pidName[2]);
    display.setCursor(0+25,12);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[2]);



    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(72,12);
    display.print(pidName[3]);
    display.setCursor(72+25,12);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[3]);




    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,22);
    display.print(pidName[4]);
    display.setCursor(0+25,22);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[4]);



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