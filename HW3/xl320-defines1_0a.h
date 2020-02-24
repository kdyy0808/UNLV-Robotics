// FILE: xl320-defines1_0a.h
// AUTH: P.Oh
// DATE: 09/19/19 12:24
// VERS: 1.0a: XL-320 motor defines in Control Table; no functions in this file
// DESC: Refers to Section 2
//       http://emanual.robotis.com/docs/en/dxl/x/xl320/#control-table
//       Section 2.2 is EEPROM Control Table defines
//       Section 2.3 is the RAM Control Table defines
// REFS: F:\nationalInstruments\nxcProjects\rs-485\dynamixel\Dynamixel XL-320\
//       paulOhDynamixelXl320HeaderFile-1.0d.h

// Instruction related Defines

#define HEADER_1			0xFF // For Instruction Packet Header 1
#define HEADER_2			0xFF // For Instruction Packet Header 2
#define HEADER_3			0xFD // For Instruction Packet Header 3
#define RESERVED			0x00 // For Instruction Packet Reserved

// EEPROM Address related Defines
// See Robotis Section 2.2 http://emanual.robotis.com/docs/en/dxl/x/xl320/

#define EEPROM_MODEL_NUMBER        0x00 // 2 bytes; motor's model number
#define EEPROM_FIRMWARE_VERSION	   0x02 // 1 byte; motor's firmare version
#define EEPROM_ID			             0x03 // 1 byte; motor's ID number [0-252]
#define EEPROM_BAUD_RATE		       0x04 // 1 byte; baud [0-3]
#define EEPROM_RETURN_DELAY_TIME	 0x05 // 1 byte; instruction packet send time
#define EEPROM_CW_ANGLE_LIMIT	     0x06 // 2 bytes; minimum value of
                                        // Goal Position
#define EEPROM_CCW_ANGLE_LIMIT	   0X08 // 2 bytes; maximum value of
                                        // Goal Position
#define EEPROM_CONTROL_MODE        0x0B // 1 byte; Wheel (1) or Joint (2) modes
#define EEPROM_TEMPERATURE_LIMIT   0x0C // 1 byte; overheat shutdown
                                        // value [0-100]
#define EEPROM_MIN_VOLTAGE_LIMIT   0x0D // 1 byte; minimum operational voltage
#define EEPROM_MAX_VOLTAGE_LIMIT   0x0E // 1 byte; maximum operational voltage
#define EEPROM_MAX_TORQUE          0x0F // 2 bytes; maximum torque value
#define EEPROM_STATUS_RETURN_LEVEL 0x11 // 1 byte; how to send status packet
#define EEPROM_SHUTDOWN		         0x12 // 1 byte; when to shutdown motor

// RAM Address related Defines
// See Robotis Section 2.3 http://emanual.robotis.com/docs/en/dxl/x/xl320/

#define RAM_TORQUE_ENABLE          0x18 // 1 byte; turns on/off torque control
#define RAM_LED                    0x19 // 1 byte; changes motor's LED color
#define RAM_D_GAIN			           0x1B // 1 byte; motor's derivative gain
#define RAM_I_GAIN			           0x1C // 1 byte; motor's integral gain
#define RAM_P_GAIN			           0x1D // 1 byte; motor's proportional gain
#define RAM_GOAL_POSITION		       0x1E // 2 bytes; destination position value
                                        // from [0, 1023] with 0 most CW and
                                        // 1023 most CCW
#define RAM_MOVING_SPEED	         0x20 // 2 bytes; Wheel or Joint dependent
#define RAM_TORQUE_LIMIT	         0x23 // 2 bytes; maximum torque limit value
#define RAM_PRESENT_POSITION	     0x25 // 2 bytes; motor's present position
                                        // value [0-1023]
#define RAM_PRESENT_SPEED		       0x27 // 2 bytes; Wheel or Joint mode
                                        // dependent [0-2047]
#define RAM_PRESENT_LOAD		       0x29 // 2 bytes; currently applied load
                                        // value is [0-2047]
#define RAM_PRESENT_VOLTAGE		     0x2D // 1 byte; present supply voltage
#define RAM_PRESENT_TEMPERATURE	   0x2E // 1 byte; motor's internal temperature
                                        // in Celsius
#define RAM_REGISTERED		         0x2F // 1 byte; REG_WRITE instruction
                                        // received or not
#define RAM_MOVING			           0x31 // 1 byte; Goal Position completed
                                        // or in-progress
#define RAM_HARDWARE_ERROR_STATUS	 0x32 // 1 byte; present hardware error status
#define RAM_PUNCH			             0x33 // 2 bytes; minimum current to
                                        // drive motor

// Instruction related Defines
// See Section 2.5
// http://emanual.robotis.com/docs/en/dxl/protocol2/#instruction-packet

#define INSTRUCTION_PING           0x01 // checks if arriving packet ID
                                        // is same as packet ID
#define INSTRUCTION_READ		       0x02 // read data from device
#define INSTRUCTION_WRITE		       0x03 // write data to device
#define INSTRUCTION_REG_WRITE	     0x04 // registers instruction packet to
                                        // set for standby status
#define INSTRUCTION_ACTION	       0x05 // executes packet by
                                        // INSTRUCTION_ REG_WRITE
#define INSTRUCTION_FACTORY_RESET	 0x06 // reset Control Table to factory
                                        // default
#define INSTRUCTION_REBOOT	       0x08 // reboot device
#define INSTRUCTION_CLEAR		       0x10 // reset certain information
#define INSTRUCTION_STATUS_RETURN	 0x55 // return instruction for the
                                        // Instruction packet
#define INSTRUCTION_SYNC_READ	     0x82 // multiple devices: read all devices
#define INSTRUCTION_SYNC_WRITE     0x83 // multiple devices: write all devices
#define INSTRUCTION_BULK_READ	     0x92 // multiple devices: read different
                                        // devices
#define INSTRUCTION_BULK_WRITE 	   0x93 // multiple devices: write different
                                        // devices


