/*
    mcp_can.h
    2012 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author:Loovee (loovee@seeed.cc)
    2014-1-16

    Contributor:

    Cory J. Fowler
    Latonita
    Woodward1
    Mehtajaghvi
    BykeBlast
    TheRo0T
    Tsipizic
    ralfEdmund
    Nathancheek
    BlueAndi
    Adlerweb
    Btetz
    Hurvajs
    ttlappalainen

    The MIT License (MIT)

    Copyright (c) 2013 Seeed Technology Inc.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#ifndef _MCP2515_H_
#define _MCP2515_H_

#include "mcp_can_dfs.h"

#define MAX_CHAR_IN_MESSAGE 8

enum {
  PIDS_SUPPORT_01_20                                = 0x00,
  MONITOR_STATUS_SINCE_DTCS_CLEARED                 = 0x01,
  FREEZE_DTC                                        = 0x02,
  FUEL_SYSTEM_STATUS                                = 0x03,
  CALCULATED_ENGINE_LOAD                            = 0x04,
  ENGINE_COOLANT_TEMPERATURE                        = 0x05,
  SHORT_TERM_FUEL_TRIM_BANK_1                       = 0x06,
  LONG_TERM_FUEL_TRIM_BANK_1                        = 0x07,
  SHORT_TERM_FUEL_TRIM_BANK_2                       = 0x08,
  LONG_TERM_FUEL_TRIM_BANK_2                        = 0x09,
  FUEL_PRESSURE                                     = 0x0a,
  INTAKE_MANIFOLD_ABSOLUTE_PRESSURE                 = 0x0b,
  ENGINE_RPM                                        = 0x0c,
  VEHICLE_SPEED                                     = 0x0d,
  TIMING_ADVANCE                                    = 0x0e,
  AIR_INTAKE_TEMPERATURE                            = 0x0f,
  MAF_AIR_FLOW_RATE                                 = 0x10,
  THROTTLE_POSITION                                 = 0x11,
  COMMANDED_SECONDARY_AIR_STATUS                    = 0x12,
  OXYGEN_SENSORS_PRESENT_IN_2_BANKS                 = 0x13,
  OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM              = 0x14,
  OXYGEN_SENSOR_2_SHORT_TERM_FUEL_TRIM              = 0x15,
  OXYGEN_SENSOR_3_SHORT_TERM_FUEL_TRIM              = 0x16,
  OXYGEN_SENSOR_4_SHORT_TERM_FUEL_TRIM              = 0x17,
  OXYGEN_SENSOR_5_SHORT_TERM_FUEL_TRIM              = 0x18,
  OXYGEN_SENSOR_6_SHORT_TERM_FUEL_TRIM              = 0x19,
  OXYGEN_SENSOR_7_SHORT_TERM_FUEL_TRIM              = 0x1a,
  OXYGEN_SENSOR_8_SHORT_TERM_FUEL_TRIM              = 0x1b,
  OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO            = 0x1c,
  OXYGEN_SENSORS_PRESENT_IN_4_BANKS                 = 0x1d,
  AUXILIARY_INPUT_STATUS                            = 0x1e,
  RUN_TIME_SINCE_ENGINE_START                       = 0x1f,

  PIDS_SUPPORT_21_40                                = 0x20,
  DISTANCE_TRAVELED_WITH_MIL_ON                     = 0x21,
  FUEL_RAIL_PRESSURE                                = 0x22,
  FUEL_RAIL_GAUGE_PRESSURE                          = 0x23,
  OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO        = 0x24,
  OXYGEN_SENSOR_2_FUEL_AIR_EQUIVALENCE_RATIO        = 0x25,
  OXYGEN_SENSOR_3_FUEL_AIR_EQUIVALENCE_RATIO        = 0x26,
  OXYGEN_SENSOR_4_FUEL_AIR_EQUIVALENCE_RATIO        = 0x27,
  OXYGEN_SENSOR_5_FUEL_AIR_EQUIVALENCE_RATIO        = 0x28,
  OXYGEN_SENSOR_6_FUEL_AIR_EQUIVALENCE_RATIO        = 0x29,
  OXYGEN_SENSOR_7_FUEL_AIR_EQUIVALENCE_RATIO        = 0x2a,
  OXYGEN_SENSOR_8_FUEL_AIR_EQUIVALENCE_RATIO        = 0x2b,
  COMMANDED_EGR                                     = 0x2c,
  EGR_ERROR                                         = 0x2d,
  COMMANDED_EVAPORATIVE_PURGE                       = 0x2e,
  FUEL_TANK_LEVEL_INPUT                             = 0x2f,
  WARM_UPS_SINCE_CODES_CLEARED                      = 0x30,
  DISTANCE_TRAVELED_SINCE_CODES_CLEARED             = 0x31,
  EVAP_SYSTEM_VAPOR_PRESSURE                        = 0x32,
  ABSOLULTE_BAROMETRIC_PRESSURE                     = 0x33,
/*OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO        = 0x34,
  OXYGEN_SENSOR_2_FUEL_AIR_EQUIVALENCE_RATIO        = 0x35,
  OXYGEN_SENSOR_3_FUEL_AIR_EQUIVALENCE_RATIO        = 0x36,
  OXYGEN_SENSOR_4_FUEL_AIR_EQUIVALENCE_RATIO        = 0x37,
  OXYGEN_SENSOR_5_FUEL_AIR_EQUIVALENCE_RATIO        = 0x38,
  OXYGEN_SENSOR_6_FUEL_AIR_EQUIVALENCE_RATIO        = 0x39,
  OXYGEN_SENSOR_7_FUEL_AIR_EQUIVALENCE_RATIO        = 0x3a,
  OXYGEN_SENSOR_8_FUEL_AIR_EQUIVALENCE_RATIO        = 0x3b,*/
  CATALYST_TEMPERATURE_BANK_1_SENSOR_1              = 0x3c,
  CATALYST_TEMPERATURE_BANK_2_SENSOR_1              = 0x3d,
  CATALYST_TEMPERATURE_BANK_1_SENSOR_2              = 0x3e,
  CATALYST_TEMPERATURE_BANK_2_SENSOR_2              = 0x3f,

  PIDS_SUPPORT_41_60                                = 0x40,
  MONITOR_STATUS_THIS_DRIVE_CYCLE                   = 0x41,
  CONTROL_MODULE_VOLTAGE                            = 0x42,
  ABSOLUTE_LOAD_VALUE                               = 0x43,
  FUEL_AIR_COMMANDED_EQUIVALENCE_RATE               = 0x44,
  RELATIVE_THROTTLE_POSITION                        = 0x45,
  AMBIENT_AIR_TEMPERATURE                           = 0x46,
  ABSOLUTE_THROTTLE_POSITION_B                      = 0x47,
  ABSOLUTE_THROTTLE_POSITION_C                      = 0x48,
  ABSOLUTE_THROTTLE_POSITION_D                      = 0x49,
  ABSOLUTE_THROTTLE_POSITION_E                      = 0x4a,
  ABSOLUTE_THROTTLE_POSITION_F                      = 0x4b,
  COMMANDED_THROTTLE_ACTUATOR                       = 0x4c,
  TIME_RUN_WITH_MIL_ON                              = 0x4d,
  TIME_SINCE_TROUBLE_CODES_CLEARED                  = 0x4e,
/*                                                  = 0x4f,
                                                    = 0x50,*/
  FUEL_TYPE                                         = 0x51,
  ETHANOL_FUEL_PERCENTAGE                           = 0x52,
  ABSOLUTE_EVAP_SYSTEM_VAPOR_PRESSURE               = 0x53,
/*EVAP_SYSTEM_VAPOR_PRESSURE                        = 0x54,*/
/*                                                  = 0x55,
                                                    = 0x56,
                                                    = 0x57,
                                                    = 0x58,*/
  FUEL_RAIL_ABSOLUTE_PRESSURE                       = 0x59,
  RELATIVE_ACCELERATOR_PEDAL_POSITTION              = 0x5a,
  HYBRID_BATTERY_PACK_REMAINING_LIFE                = 0x5b,
  ENGINE_OIL_TEMPERATURE                            = 0x5c,
  FUEL_INJECTION_TIMING                             = 0x5d,
  ENGINE_FUEL_RATE                                  = 0x5e,
  EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED = 0x5f,

  // more PIDs can be added from: https://en.wikipedia.org/wiki/OBD-II_PIDs
};


class MCP_CAN {
  private:

    byte   ext_flg;                         // identifier xxxID
    // either extended (the 29 LSB) or standard (the 11 LSB)
    unsigned long  can_id;                  // can id
    byte   rtr;                             // rtr
    byte   SPICS;
    SPIClass* pSPI;
    byte   nReservedTx;                     // Count of tx buffers for reserved send
    byte   mcpMode;                         // Current controller mode

    /*
        mcp2515 driver function
    */

  private:

    void mcp2515_reset(void);                                   // reset mcp2515

    byte mcp2515_readRegister(const byte address);              // read mcp2515's register

    void mcp2515_readRegisterS(const byte address,
                               byte values[],
                               const byte n);
    void mcp2515_setRegister(const byte address,                // set mcp2515's register
                             const byte value);

    void mcp2515_setRegisterS(const byte address,               // set mcp2515's registers
                              const byte values[],
                              const byte n);

    void mcp2515_initCANBuffers(void);

    void mcp2515_modifyRegister(const byte address,             // set bit of one register
                                const byte mask,
                                const byte data);

    byte mcp2515_readStatus(void);                              // read mcp2515's Status
    byte mcp2515_setCANCTRL_Mode(const byte newmode);           // set mode
    byte mcp2515_requestNewMode(const byte newmode);                  // Set mode
    byte mcp2515_configRate(const byte canSpeed, const byte clock);  // set baudrate
    byte mcp2515_init(const byte canSpeed, const byte clock);   // mcp2515init

    void mcp2515_write_id(const byte mcp_addr,                  // write can id
                          const byte ext,
                          const unsigned long id);

    void mcp2515_read_id(const byte mcp_addr,                   // read can id
                         byte* ext,
                         unsigned long* id);

    void mcp2515_write_canMsg(const byte buffer_sidh_addr, unsigned long id, byte ext, byte rtr, byte len,
                              volatile const byte* buf);     // read can msg
    void mcp2515_read_canMsg(const byte buffer_load_addr, volatile unsigned long* id, volatile byte* ext,
                             volatile byte* rtr, volatile byte* len, volatile byte* buf);   // write can msg
    void mcp2515_start_transmit(const byte mcp_addr);           // start transmit
    byte mcp2515_getNextFreeTXBuf(byte* txbuf_n);               // get Next free txbuf
    byte mcp2515_isTXBufFree(byte* txbuf_n, byte iBuf);         // is buffer by index free

    /*
        can operator function
    */

    byte sendMsg(unsigned long id, byte ext, byte rtrBit, byte len, const byte* buf, bool wait_sent = true); // send message

  public:
    MCP_CAN(byte _CS = 0);
    void init_CS(byte _CS);                      // define CS after construction before begin()
    void setSPI(SPIClass* _pSPI) {
        pSPI = _pSPI;    // define SPI port to use before begin()
    }
    void enableTxInterrupt(bool enable = true);  // enable transmit interrupt
    void reserveTxBuffers(byte nTxBuf = 0) {
        nReservedTx = (nTxBuf < MCP_N_TXBUFFERS ? nTxBuf : MCP_N_TXBUFFERS - 1);
    }
    byte getLastTxBuffer() {
        return MCP_N_TXBUFFERS - 1;    // read index of last tx buffer
    }

    byte begin(byte speedset, const byte clockset = MCP_16MHz);     // init can
    byte init_Mask(byte num, byte ext, unsigned long ulData);       // init Masks
    byte init_Filt(byte num, byte ext, unsigned long ulData);       // init filters
    void setSleepWakeup(byte
                        enable);                               // Enable or disable the wake up interrupt (If disabled the MCP2515 will not be woken up by CAN bus activity, making it send only)
    byte sleep();													// Put the MCP2515 in sleep mode
    byte wake();													// Wake MCP2515 manually from sleep
    byte setMode(byte opMode);                                      // Set operational mode
    byte getMode();				                                    // Get operational mode
    byte sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, const byte* buf, bool wait_sent = true); // send buf
    byte sendMsgBuf(unsigned long id, byte ext, byte len, const byte* buf, bool wait_sent = true);             // send buf
    byte readMsgBuf(byte* len, byte* buf);                          // read buf
    byte readMsgBufID(unsigned long* ID, byte* len, byte* buf);     // read buf with object ID
    byte checkReceive(void);                                        // if something received
    byte checkError(void);                                          // if something error
    unsigned long getCanId(void);                                   // get can id when receive
    byte isRemoteRequest(void);                                     // get RR flag when receive
    byte isExtendedFrame(void);                                     // did we recieve 29bit frame?

    byte readMsgBufID(byte status, volatile unsigned long* id, volatile byte* ext, volatile byte* rtr, volatile byte* len,
                      volatile byte* buf); // read buf with object ID
    byte trySendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, const byte* buf,
                       byte iTxBuf = 0xff); // as sendMsgBuf, but does not have any wait for free buffer
    byte sendMsgBuf(byte status, unsigned long id, byte ext, byte rtrBit, byte len,
                    volatile const byte* buf); // send message buf by using parsed buffer status
    inline byte trySendExtMsgBuf(unsigned long id, byte len, const byte* buf,
                                 byte iTxBuf = 0xff) { // as trySendMsgBuf, but set ext=1 and rtr=0
        return trySendMsgBuf(id, 1, 0, len, buf, iTxBuf);
    }
    inline byte sendExtMsgBuf(byte status, unsigned long id, byte len,
                              volatile const byte* buf) { // as sendMsgBuf, but set ext=1 and rtr=0
        return sendMsgBuf(status, id, 1, 0, len, buf);
    }
    void clearBufferTransmitIfFlags(byte flags = 0);                // Clear transmit flags according to status
    byte readRxTxStatus(void);                                      // read has something send or received
    byte checkClearRxStatus(byte* status);                          // read and clear and return first found rx status bit
    byte checkClearTxStatus(byte* status,
                            byte iTxBuf = 0xff);      // read and clear and return first found or buffer specified tx status bit

    bool mcpPinMode(const byte pin, const byte
                    mode);                  // switch supported pins between HiZ, interrupt, output or input
    bool mcpDigitalWrite(const byte pin, const byte mode);             // write HIGH or LOW to RX0BF/RX1BF
    byte mcpDigitalRead(const byte pin);                               // read HIGH or LOW from supported pins

};

#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
