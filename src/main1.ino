/*************************************************************************************************
    OBD-II_PIDs TEST CODE
    LOOVEE @ JUN24, 2017

    Query
    send id: 0x7df
      dta: 0x02, 0x01, PID_CODE, 0, 0, 0, 0, 0

    Response
    From id: 0x7E9 or 0x7EA or 0x7EB
      dta: len, 0x41, PID_CODE, byte0, byte1(option), byte2(option), byte3(option), byte4(option)

    https://en.wikipedia.org/wiki/OBD-II_PIDs

    Input a PID, then you will get reponse from vehicle, the input should be end with '\n'
***************************************************************************************************/
#include <SPI.h>
#include "mcp_can.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

//If using software SPI (the default case):

#define OLED_CS     5
#define OLED_DC     6
#define OLED_RESET  7
#define OLED_SDA    8
#define OLED_SCL    9

Adafruit_SSD1306 display(OLED_SDA, OLED_SCL, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES 106
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 64 
#define LOGO16_GLCD_WIDTH  128 

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

#define PID_ENGIN_PRM       0x0C
#define PID_VEHICLE_SPEED   0x0D
#define PID_COOLANT_TEMP    0x05

#define CAN_ID_PID          0x7DF

unsigned char PID_INPUT;
unsigned char getPid    = 0;

long _lastPidResponseMillis = 0;


void set_mask_filt() {
    /*
        set mask, set both the mask to 0x3ff
    */
    CAN.init_Mask(0, 0, 0x7FC);
    CAN.init_Mask(1, 0, 0x7FC);

    /*
        set filter, we can receive id from 0x04 ~ 0x09
    */
    CAN.init_Filt(0, 0, 0x7E8);
    CAN.init_Filt(1, 0, 0x7E8);

    CAN.init_Filt(2, 0, 0x7E8);
    CAN.init_Filt(3, 0, 0x7E8);
    CAN.init_Filt(4, 0, 0x7E8);
    CAN.init_Filt(5, 0, 0x7E8);
}

void sendPid(unsigned char __pid) {
    unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0};
    SERIAL.print("SEND PID: 0x");
    SERIAL.println(__pid, HEX);
    CAN.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);
}

void sendPidExtra(unsigned char __pid1,unsigned char __pid2) {
    unsigned char tmp[8] = {0x03, 0x22, __pid1, __pid2, 0, 0, 0, 0};
    SERIAL.print("SEND PID: 0x");
    SERIAL.print(__pid1, HEX);
    SERIAL.println(__pid2, HEX);
    CAN.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);
}

void setup() {
    SERIAL.begin(9600);
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
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

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {  // init can bus : baudrate = 500k
        SERIAL.println("CAN BUS Shield init fail");
        SERIAL.println(" Init CAN BUS Shield again");
        delay(100);
    }
    SERIAL.println("CAN BUS Shield init ok!");
    set_mask_filt();
}


void loop() {

    char *pidName[6]={'\0'};
    pidName[0]="ECT";
    pidName[1]="TFT";
    pidName[2]="RPM";
    pidName[3]="TEST";
    pidName[4]="TEST";
    pidName[5]="TEST";

    float pidValue[6] = {};
    pidValue[0] = 0.0;
    pidValue[1] = 0.0;
    pidValue[2] = 0.0;
    pidValue[3] = 0.0;
    pidValue[4] = 0.0;
    pidValue[5] = 0.0;

    float rpm = readOBD2ByPIDs(0x0C);
    float ect = readOBD2ByPIDs(0x05);
    float tft = readOBD2ByPIDsExtra(0x1E,0x1C);

     pidValue[0] = ect;
     pidValue[1] = tft;
     pidValue[2] = rpm;

    displayGauge(pidName,pidValue);   

    delay(1000);

    
}

float readOBD2ByPIDs(unsigned char pid){

    unsigned char tmp[8] = {0x02, 0x01, pid, 0, 0, 0, 0, 0};
    SERIAL.print("SEND PID: 0x");
    SERIAL.println(pid, HEX);
    CAN.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);
    

    int _responseTimeout = 2000;

  //  for (unsigned long start = millis(); (millis() - start) < _responseTimeout;) {
        unsigned char len = 0;
        unsigned char buf[8];
        if (CAN_MSGAVAIL == CAN.checkReceive()) {                // check if get data
            CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

            SERIAL.println("\r\n------------------------------------------------------------------");
            SERIAL.print("Get Data From id: 0x");
            SERIAL.println(CAN.getCanId(), HEX);
            for (int i = 0; i < len; i++) { // print the data
                SERIAL.print("0x");
                SERIAL.print(buf[i], HEX);
                SERIAL.print("\t");
            }
            SERIAL.println();

            uint8_t A = buf[3];
            uint8_t B = buf[4];
            uint8_t C = buf[5];
            uint8_t D = buf[6];
            

             switch (pid) {
                default:
                case PIDS_SUPPORT_01_20: // raw
                case MONITOR_STATUS_SINCE_DTCS_CLEARED: // raw
                case FREEZE_DTC: // raw
                case PIDS_SUPPORT_21_40: // raw
                case PIDS_SUPPORT_41_60: // raw
                case MONITOR_STATUS_THIS_DRIVE_CYCLE: // raw
                  // NOTE: return value can lose precision!
                  return ((uint32_t)A << 24 | (uint32_t)B << 16 | (uint32_t)C << 8 | (uint32_t)D);

                case FUEL_SYSTEM_STATUS: // raw
                case RUN_TIME_SINCE_ENGINE_START:
                case DISTANCE_TRAVELED_WITH_MIL_ON:
                case DISTANCE_TRAVELED_SINCE_CODES_CLEARED:
                case TIME_RUN_WITH_MIL_ON:
                case TIME_SINCE_TROUBLE_CODES_CLEARED:
                  return (A * 256.0 + B);

                case CALCULATED_ENGINE_LOAD:
                case THROTTLE_POSITION:
                case COMMANDED_EGR:
                case COMMANDED_EVAPORATIVE_PURGE:
                case FUEL_TANK_LEVEL_INPUT:
                case RELATIVE_THROTTLE_POSITION:
                case ABSOLUTE_THROTTLE_POSITION_B:
                case ABSOLUTE_THROTTLE_POSITION_C:
                case ABSOLUTE_THROTTLE_POSITION_D:
                case ABSOLUTE_THROTTLE_POSITION_E:
                case ABSOLUTE_THROTTLE_POSITION_F:
                case COMMANDED_THROTTLE_ACTUATOR:
                case ETHANOL_FUEL_PERCENTAGE:
                case RELATIVE_ACCELERATOR_PEDAL_POSITTION:
                case HYBRID_BATTERY_PACK_REMAINING_LIFE:
                  return (A / 2.55);

                case COMMANDED_SECONDARY_AIR_STATUS: // raw
                case OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO: // raw
                case OXYGEN_SENSORS_PRESENT_IN_2_BANKS: // raw
                case OXYGEN_SENSORS_PRESENT_IN_4_BANKS: // raw
                case AUXILIARY_INPUT_STATUS: // raw
                case FUEL_TYPE: // raw
                case EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED: // raw
                  return (A);

                case OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM:
                case OXYGEN_SENSOR_2_SHORT_TERM_FUEL_TRIM:
                case OXYGEN_SENSOR_3_SHORT_TERM_FUEL_TRIM:
                case OXYGEN_SENSOR_4_SHORT_TERM_FUEL_TRIM:
                case OXYGEN_SENSOR_5_SHORT_TERM_FUEL_TRIM:
                case OXYGEN_SENSOR_6_SHORT_TERM_FUEL_TRIM:
                case OXYGEN_SENSOR_7_SHORT_TERM_FUEL_TRIM:
                case OXYGEN_SENSOR_8_SHORT_TERM_FUEL_TRIM:
                  return ((B / 1.28) - 100.0);
                  break;

                case ENGINE_COOLANT_TEMPERATURE:
                case AIR_INTAKE_TEMPERATURE:
                case AMBIENT_AIR_TEMPERATURE:
                case ENGINE_OIL_TEMPERATURE:
                  return (A - 40.0);

                case SHORT_TERM_FUEL_TRIM_BANK_1:
                case LONG_TERM_FUEL_TRIM_BANK_1:
                case SHORT_TERM_FUEL_TRIM_BANK_2:
                case LONG_TERM_FUEL_TRIM_BANK_2:
                case EGR_ERROR:
                  return ((A / 1.28) - 100.0);

                case FUEL_PRESSURE:
                  return (A * 3.0);

                case INTAKE_MANIFOLD_ABSOLUTE_PRESSURE:
                case VEHICLE_SPEED:
                case WARM_UPS_SINCE_CODES_CLEARED:
                case ABSOLULTE_BAROMETRIC_PRESSURE:
                  return (A);

                case ENGINE_RPM:
                  return ((A * 256.0 + B) / 4.0);

                case TIMING_ADVANCE:
                  return ((A / 2.0) - 64.0);

                case MAF_AIR_FLOW_RATE:
                  return ((A * 256.0 + B) / 100.0);

                case FUEL_RAIL_PRESSURE:
                  return ((A * 256.0 + B) * 0.079);

                case FUEL_RAIL_GAUGE_PRESSURE:
                case FUEL_RAIL_ABSOLUTE_PRESSURE:
                  return ((A * 256.0 + B) * 10.0);

                case OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO:
                case OXYGEN_SENSOR_2_FUEL_AIR_EQUIVALENCE_RATIO:
                case OXYGEN_SENSOR_3_FUEL_AIR_EQUIVALENCE_RATIO:
                case OXYGEN_SENSOR_4_FUEL_AIR_EQUIVALENCE_RATIO:
                case OXYGEN_SENSOR_5_FUEL_AIR_EQUIVALENCE_RATIO:
                case OXYGEN_SENSOR_6_FUEL_AIR_EQUIVALENCE_RATIO:
                case OXYGEN_SENSOR_7_FUEL_AIR_EQUIVALENCE_RATIO:
                case OXYGEN_SENSOR_8_FUEL_AIR_EQUIVALENCE_RATIO:
                case 0x34:
                case 0x35:
                case 0x36:
                case 0x37:
                case 0x38:
                case 0x39:
                case 0x3a:
                case 0x3b:
                  return (((A * 256.0 + B) * 2.0) / 65536.0);

                case EVAP_SYSTEM_VAPOR_PRESSURE:
                  return (((int16_t)(A * 256.0 + B)) / 4.0);

                case CATALYST_TEMPERATURE_BANK_1_SENSOR_1:
                case CATALYST_TEMPERATURE_BANK_2_SENSOR_1:
                case CATALYST_TEMPERATURE_BANK_1_SENSOR_2:
                case CATALYST_TEMPERATURE_BANK_2_SENSOR_2:
                  return (((A * 256.0 + B) / 10.0) - 40.0);

                case CONTROL_MODULE_VOLTAGE:
                  return ((A * 256.0 + B) / 1000.0);

                case ABSOLUTE_LOAD_VALUE:
                  return ((A * 256.0 + B) / 2.55);

                case FUEL_AIR_COMMANDED_EQUIVALENCE_RATE:
                  return (2.0 * (A * 256.0 + B) / 65536.0);

                case ABSOLUTE_EVAP_SYSTEM_VAPOR_PRESSURE:
                  return ((A * 256.0 + B) / 200.0);

                case 0x54:
                  return ((A * 256.0 + B) - 32767.0);

                case FUEL_INJECTION_TIMING:
                  return (((A * 256.0 + B) / 128.0) - 210.0);

                case ENGINE_FUEL_RATE:
                  return ((A * 256.0 + B) / 20.0);
              }
        }
   // }  
}


float readOBD2ByPIDsExtra(unsigned char pid1,unsigned char pid2){


  unsigned char tmp[8] = {0x03, 0x22, pid1, pid2, 0, 0, 0, 0};
    SERIAL.print("SEND PID: 0x");
    SERIAL.print(pid1, HEX);
    SERIAL.println(pid2, HEX);
    CAN.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);

    int _responseTimeout = 2000;

   // for (unsigned long start = millis(); (millis() - start) < _responseTimeout;) {
        unsigned char len = 0;
        unsigned char buf[8];
        if (CAN_MSGAVAIL == CAN.checkReceive()) {                // check if get data
            CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

            SERIAL.println("\r\n------------------------------------------------------------------");
            SERIAL.print("Get Data From id: 0x");
            SERIAL.println(CAN.getCanId(), HEX);
            for (int i = 0; i < len; i++) { // print the data
                SERIAL.print("0x");
                SERIAL.print(buf[i], HEX);
                SERIAL.print("\t");
            }
            SERIAL.println();

            uint8_t A = buf[4];
            uint8_t B = buf[5];
            return(((((signed(A)*256)+B)*(9/8)+320)/10) -32)*(0.55);      
        }
    //} 
}


void taskCanRecv() {
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {                // check if get data
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        SERIAL.println("\r\n------------------------------------------------------------------");
        SERIAL.print("Get Data From id: 0x");
        SERIAL.println(CAN.getCanId(), HEX);
        for (int i = 0; i < len; i++) { // print the data
            SERIAL.print("0x");
            SERIAL.print(buf[i], HEX);
            SERIAL.print("\t");
        }
        SERIAL.println();
    }
}

void taskDbg() {
    while (SERIAL.available()) {
        char c = SERIAL.read();

        if (c >= '0' && c <= '9') {
            PID_INPUT *= 0x10;
            PID_INPUT += c - '0';

        } else if (c >= 'A' && c <= 'F') {
            PID_INPUT *= 0x10;
            PID_INPUT += 10 + c - 'A';
        } else if (c >= 'a' && c <= 'f') {
            PID_INPUT *= 0x10;
            PID_INPUT += 10 + c - 'a';
        } else if (c == '\n') { // END
            getPid = 1;
        }
    }
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
    display.setCursor(0+28,12);
    display.setTextSize(1);
    display.setTextColor(WHITE);
   display.println(pidValue[2]);


     // text row 2 2
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(72,12);
    display.print(pidName[3]);
    display.setCursor(72+28,12);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[3]);
   


     // text row 3 1
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,22);
    display.print(pidName[4]);
    display.setCursor(0+28,22);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[4]);
  

     // text row 3 2
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(72,22);
    display.print(pidName[5]);
    display.setCursor(72+28,22);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(pidValue[5]);
    display.display();

}

// END FILE