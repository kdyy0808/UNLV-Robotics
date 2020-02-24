// FILE: xl320-functions1.0c.h
// AUTH: P.Oh
// DATE: 12/10/19 09:26
// VERS: 1.0a: has XL320_setLed and XL320_servo functions
//       1.0b: add servo read functions
//       1.0c: corrections to 1.0b servo read functions
// DESC: Functions written for the XL-320 e.g. servoing, LED on/off, etc
// REFS: P:\rs-485\Dynamixel XL-320\norbertoDynamixelXl320Tutorial\
//       paulOhDynamixelXl320HeaderFile-1.0d.h
//       Also need XL-320 defines: xl320-defines1_0a.h

// Referencing http://emanual.robotis.com/docs/en/dxl/protocol2/#status-packet
// one needs an Instruction Packet to Read and/or Write to the XL-320 motor.
// Section 2 describes the expected packet's format:

// Instruction Packet has the form:
// +----------+----------+---------+----------+-----------+
// | Header 1 | Header 2 | Header 3| Reserved | Packet ID |
// + ---------+----------+---------+----------+-----------+ ... cont'd
// |   0xFF   |  0xFF    |  0XFD   |   0x00   |    ID     |
// +----------+----------+---------+----------+-----------+
//
// +----------+----------+-------------+--------+---------+---------+
// | Length 1 | Length 2 | Instruction | Param  |  Param  |  Param  |
// +----------+----------+-------------+--------+---------+---------+ ...cont'd
// |  Len_L   |  Len_H   | Instruction | Param1 |  ...    | Param N |
// +----------+----------+-------------+--------+---------+---------+

// +--------+-------+
// |  CRC1  |  CRC2 |
// +--------+-------+
// | CRC_L  | CRC_H |
// +--------+-------+

// Functions are created by transmitting the Instruction Packet using an
// array that's populated with values using the order above

// Declaration of functions
unsigned short update_crc(unsigned short crc_accum,
                          unsigned char data_blk_ptr[],
                          unsigned short data_blk_size);

// --------------------------------------------------------------------------
// waitForMessageToBeSent Function:
inline void waitForMessageToBeSent()
{
#if 0
  // using low level functions to wait for RS485 message to be sent
  bool sending, avail;
  RS485Status(sending, avail);
  while (sending) {
    Wait(MS_1);
    RS485Status(sending, avail);
  }
#else
  // use hi level API functions (preferred)
  while (RS485SendingData())
    Wait(MS_1);
#endif
}; // end waitForMessageToBeSent function

// ---------------------------------------------------------------------------
// XL320_setBaud Function: assign baud rate for XL-320 motor
void XL320_setBaud(unsigned char XL320_servoId, unsigned char XL320_baudRate) {

  // Control Table Section 2.2 http://emanual.robotis.com/docs/en/dxl/x/xl320/
  // Baud Rate size is 1 byte with values [0, 3]

  // See Section 2.4.4 Baud Rate
  // http://emanual.robotis.com/docs/en/dxl/x/xl320/#baud-rate
  // Values are: 0 (9600 bps); 1 (57,600 bps); 2 (115200 bps); and 3 (1 Mbps)

  // Variables to set Length 1 and Length 2
  unsigned char XL320_setBaudLength_L;
  unsigned char XL320_setBaudLength_H;

  // Variables to set up packet array
  unsigned char tempPacket[];   // temporary packet
  unsigned char finalPacket[];  // final packet to transmit

  // Variables for checksum CRC
  unsigned short setBaud_CRC;
  byte CRC_L, CRC_H;

  // 1. Calculate lengths
  // Recall that Length 1 and Length 2 = number of parameters + 3
  // Setting baud rate requires only 4 parameters: address, 0x00, and baud rate
  // Hence number of paramters + 3 is 3 + 3 = 6 Dec = 0x06

  XL320_setBaudLength_L = 0x06;
  XL320_setBaudLength_H = 0x00;

  // 2. Construct first part of packet
  ArrayBuild(tempPacket, HEADER_1, HEADER_2, HEADER_3, RESERVED, XL320_servoId,
    XL320_setBaudLength_L, XL320_setBaudLength_H, INSTRUCTION_WRITE,
    EEPROM_BAUD_RATE, 0x00, XL320_baudRate);

  // 3. Perform checksum,
  //    see Section 1.2 of http://emanual.robotis.com/docs/en/dxl/crc/
  unsigned int packetLength = (XL320_setBaudLength_H >> 8) + XL320_setBaudLength_L;

  // See last bullet in Section 1.2 "Packet Analysis and CRC Calculation"
  setBaud_CRC = update_crc(0, tempPacket, 5 + packetLength);
  CRC_L = (setBaud_CRC & 0x00FF);
  CRC_H = (setBaud_CRC >> 8) & 0x00FF;

  // 4. Concatenate into final packet and sent thru NXT RS485
  ArrayBuild(finalPacket, tempPacket, CRC_L, CRC_H);
  RS485Write(finalPacket);

  // 5. Call inline function
  waitForMessageToBeSent();

}; // end XL320_setBaud function

// ---------------------------------------------------------------------------
// XL320_setTorqueEnable Function: Enable Torque on or off on XL-320 motor
void XL320_setTorqueEnable(unsigned char XL320_servoId,
                           unsigned char XL320_torqueEnable) {

  // Section 2.1.1 http://emanual.robotis.com/docs/en/dxl/x/xl320/
  // says that changing EEPROM areas in Control table, requires setting
  // Torque Enable to zero (i.e. off).  EG: Baud Rate is under EEPROM Control
  // area.  So, if one wishes to set the baud rate, one probably needs to turn
  // off Torque Enable

  // Torque Enable Section 2.4.13
  // http://emanual.robotis.com/docs/en/dxl/x/xl320/#torque-enable
  // Takes 1 byte. 0 = Off; 1 = On

  // Variables to set Length 1 and Length 2
  unsigned char XL320_setTorqueEnableLength_L;
  unsigned char XL320_setTorqueEnableLength_H;

  // Variables to set up packet array
  unsigned char tempPacket[];   // temporary packet
  unsigned char finalPacket[];  // final packet to transmit

  // Variables for checksum CRC
  unsigned short setTorqueEnable_CRC;
  byte CRC_L, CRC_H;

  // 1. Calculate lengths
  // Recall that Length 1 and Length 2 = number of parameters + 3
  // Setting Torque Enable requires only 3 parameters: address, 0x00 and Torque Enable value
  // Hence number of (paramters + 3) is (3 + 3) = 6 Dec = 0x06

  XL320_setTorqueEnableLength_L = 0x06;
  XL320_setTorqueEnableLength_H = 0x00;

  // 2. Construct first part of packet
  ArrayBuild(tempPacket, HEADER_1, HEADER_2, HEADER_3, RESERVED, XL320_servoId,
    XL320_setTorqueEnableLength_L, XL320_setTorqueEnableLength_H, INSTRUCTION_WRITE,
    RAM_TORQUE_ENABLE, 0x00, XL320_torqueEnable);

  // 3. Perform checksum, see Section 1.2 of http://emanual.robotis.com/docs/en/dxl/crc/
  unsigned int packetLength = (XL320_setTorqueEnableLength_H >> 8) + XL320_setTorqueEnableLength_L;

  // See last bullet in Section 1.2 "Packet Analysis and CRC Calculation"
  setTorqueEnable_CRC = update_crc(0, tempPacket, 5 + packetLength);
  CRC_L = (setTorqueEnable_CRC & 0x00FF);
  CRC_H = (setTorqueEnable_CRC >> 8) & 0x00FF;

  // 4. Concatenate into final packet and sent thru NXT RS485
  ArrayBuild(finalPacket, tempPacket, CRC_L, CRC_H);
  RS485Write(finalPacket);

  // 5. Call inline function
  waitForMessageToBeSent();

}; // end XL320_setTorqueEnable

// ---------------------------------------------------------------------------
// XL320_setLed Function: LED operations on XL-320 motor
void XL320_setLed(unsigned char XL320_servoId, unsigned char XL320_ledColor) {

  // Control Table Section 2.3 http://emanual.robotis.com/docs/en/dxl/x/xl320/
  // LED size is 1 byte with values [0, 7]

  // See Section 2.4.14 LED http://emanual.robotis.com/docs/en/dxl/x/xl320/#led
  // LED values: 0 (Off); 1 (Red); 2 (Green); 3 (Yellow); 4 (Blue); 5 (Purple)
  //             6 (Cyan); 7 (White)

  // Variables to set Length 1 and Length 2
  unsigned char XL320_setLedLength_L;
  unsigned char XL320_setLedLength_H;

  // Variables to set up packet array
  unsigned char tempPacket[];   // temporary packet
  unsigned char finalPacket[];  // final packet to transmit

  // Variables for checksum CRC
  unsigned short setLed_CRC;
  byte CRC_L, CRC_H;

  // 1. Calculate lengths
  // Recall that Length 1 and Length 2 = number of parameters + 3
  // Setting LED requires only 3 parameters: address, 0x00 and output color
  // Hence number of paramters + 3 is 3 + 3 = 6 Dec = 0x06

  XL320_setLedLength_L = 0x06;
  XL320_setLedLength_H = 0x00;

  // 2. Construct first part of packet
  ArrayBuild(tempPacket, HEADER_1, HEADER_2, HEADER_3, RESERVED, XL320_servoId,
    XL320_setLedLength_L, XL320_setLedLength_H, INSTRUCTION_WRITE,
    RAM_LED, 0x00, XL320_ledColor);

  // 3. Perform checksum, see Section 1.2 of http://emanual.robotis.com/docs/en/dxl/crc/
  unsigned int packetLength = (XL320_setLedLength_H >> 8) + XL320_setLedLength_L;

  // See last bullet in Section 1.2 "Packet Analysis and CRC Calculation"
  setLed_CRC = update_crc(0, tempPacket, 5 + packetLength);
  CRC_L = (setLed_CRC & 0x00FF);
  CRC_H = (setLed_CRC >> 8) & 0x00FF;

  // 4. Concatenate into final packet and sent thru NXT RS485
  ArrayBuild(finalPacket, tempPacket, CRC_L, CRC_H);
  RS485Write(finalPacket);

  // 5. Call inline function
  waitForMessageToBeSent();

}; // end XL320_setLed function

// -------------------------------------------------------------------------
// Servo Function: move XL-320 to desired position and desired speed
void XL320_servo(unsigned char XL320_motorId,
                 unsigned int XL320_desiredPosition,
                 unsigned int XL320_desiredSpeed) {

  // Variables to set Length 1 and Length 2
  // unsigned char XL320_setServoLength_L;
  // unsigned char XL320_setServoLength_H;
  byte XL320_setServoLength_L;
  byte XL320_setServoLength_H;

  // Variables for position and speed
  unsigned char XL320_position_L, XL320_position_H;
  unsigned char XL320_speed_L, XL320_speed_H;
  // byte XL320_position_L, XL320_position_H;
  // byte XL320_speed_L, XL320_speed_H;

  // Variables to set up packet array
  unsigned char tempPacket[];   // temporary packet
  unsigned char finalPacket[];  // final packet to transmit

  // Variables for checksum CRC
  unsigned short setServo_CRC;
  byte CRC_L, CRC_H;

  // 1. Calculate lengths
  // Recall that Length 1 and Length 2 = number of parameters + 3
  // Setting Servo requires only 6 parameters: Goal Position, 0x00, Position_L,
  // Position_H, Speed_L, and Speed_H
  // Hence number of paramters + 3 is 6 + 3 = 9 Dec = 0x09

  XL320_setServoLength_L = 0x09;
  XL320_setServoLength_H = 0x00;
  XL320_position_L = XL320_desiredPosition; // Lower byte of 16-bit position
  XL320_position_H = XL320_desiredPosition >> 8; // Upper byte
  XL320_speed_L = XL320_desiredSpeed; // Lower byte of 16-bit speed
  XL320_speed_H = XL320_desiredSpeed >> 8; // Upper byte

  // 2. Construct first part of packet
  ArrayBuild(tempPacket, HEADER_1, HEADER_2, HEADER_3, RESERVED, XL320_motorId,
    XL320_setServoLength_L, XL320_setServoLength_H, INSTRUCTION_WRITE,
    RAM_GOAL_POSITION, 0x00, XL320_position_L, XL320_position_H,
    XL320_speed_L, XL320_speed_H);

  // 3. Perform checksum, see Section 1.2
  // of http://emanual.robotis.com/docs/en/dxl/crc/
  unsigned int packetLength = (XL320_setServoLength_H >> 8) + XL320_setServoLength_L;

  // See last bullet in Section 1.2 "Packet Analysis and CRC Calculation"
  setServo_CRC = update_crc(0, tempPacket, 5 + packetLength);
  CRC_L = (setServo_CRC & 0x00FF);
  CRC_H = (setServo_CRC >> 8) & 0x00FF;

  // 4. Concatenate into final packet and sent thru NXT RS485
  ArrayBuild(finalPacket, tempPacket, CRC_L, CRC_H);
  RS485Write(finalPacket);

  // 5. Call inline function
  waitForMessageToBeSent();

}; // end XL320_servo

/* ========================================= */

// -------------------------------------------------------------------------
// Servo Function: read XL-320 position
void XL320_servoRead(unsigned char XL320_motorId) {

  // Variables to set Length 1 and Length 2
  // unsigned char XL320_setServoLength_L;
  // unsigned char XL320_setServoLength_H;
  byte XL320_setServoReadLength_L;
  byte XL320_setServoReadLength_H;

  // Variables for position and speed
  unsigned char XL320_position_L, XL320_position_H;
  unsigned char XL320_speed_L, XL320_speed_H;
  // byte XL320_position_L, XL320_position_H;
  // byte XL320_speed_L, XL320_speed_H;

  // Variables to set up packet array
  unsigned char tempPacket[];   // temporary packet
  unsigned char finalPacket[];  // final packet to transmit

  // Variables for checksum CRC
  unsigned short setServoRead_CRC;
  byte CRC_L, CRC_H;

  // 1. Calculate lengths
  // Recall that Length 1 and Length 2 = number of parameters + 3
  // Reading servo requires only 4 parameters: RAM_PresentPosition, 0x00,
  // Number of bytes (which is 2), and 0x00
  // Hence number of paramters + 3 is 4 + 3 = 7 Dec = 0x07

  XL320_setServoReadLength_L = 0x07; // Read Instruction
  XL320_setServoReadLength_H = 0x00;

  // 2. Construct first part of packet
  ArrayBuild(tempPacket, HEADER_1, HEADER_2, HEADER_3, RESERVED, XL320_motorId,
    XL320_setServoReadLength_L, XL320_setServoReadLength_H, INSTRUCTION_READ,
    RAM_PRESENT_POSITION, 0x00, 0x02, 0x00);

  // 3. Perform checksum, see Section 1.2
  // of http://emanual.robotis.com/docs/en/dxl/crc/
  unsigned int packetLength = (XL320_setServoReadLength_H >> 8) + XL320_setServoReadLength_L;

  // See last bullet in Section 1.2 "Packet Analysis and CRC Calculation"
  // http://emanual.robotis.com/docs/en/dxl/crc/
  setServoRead_CRC = update_crc(0, tempPacket, 5 + packetLength);
  CRC_L = (setServoRead_CRC & 0x00FF);
  CRC_H = (setServoRead_CRC >> 8) & 0x00FF;

  // 4. Concatenate into final packet and sent thru NXT RS485
  ArrayBuild(finalPacket, tempPacket, CRC_L, CRC_H);
  RS485Write(finalPacket);

  // 5. Call inline function
  waitForMessageToBeSent();

}; // end XL320_servoRead

/* ========================================= */

// -------------------------------------------------------------------------
// Need Checksum CRC (Cyclic Redundancy Cycle)
// Robotis has such a function: http://emanual.robotis.com/docs/en/dxl/crc/
// The code below is simply cut-and-pasted from Robotis

unsigned short update_crc(unsigned short crc_accum,
                          unsigned char data_blk_ptr[],
                          unsigned short data_blk_size) {
    unsigned short i, j;
    unsigned short crc_table[256] = {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
    };

    for(j = 0; j < data_blk_size; j++)
    {
        i =  ((crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        crc_accum = (crc_accum << 8)^crc_table[i];
    }
    return crc_accum;
} // end update_crc



