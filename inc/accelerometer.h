#ifndef __ACCELEROMETER_H_
#define __ACCELEROMETER_H_

#include "board.h"

// Accelerometer Address
#define ACCEL_ADDR					0b0011000

// Register Addresses
#define CTRL_REG1					0x20
#define CTRL_REG2					0x21
#define CTRL_REG3					0x22
#define CTRL_REG4					0x23
#define INT1_CFG					0x30
#define INT1_SRC					0x31 //read only
#define INT1_THS					0x32
#define INT1_DUR					0x33

// Register Values
#define CTRL_REG1_VAL				0x27	//ODR = 10Hz in normal mode
#define CTRL_REG2_VAL				0x01	//High pass filter enabled with 0.2 Hz cut off freq for INT1
#define CTRL_REG3_VAL				0x40
#define CTRL_REG4_VAL				0x88	//Full scale = +/-2g with BDU enabled and High Resolution on
#define INT1_CFG_VAL				0x95
#define INT1_THS_VAL				0x02	//+/-125 mg of gravity for threshold
#define INT1_DUR_VAL				0x32	//10 sec duration to trigger sleep mode

void accel_enable_interrupt_output(void) {
	uint8_t out_buff[2];
	//enabling interrupts for all directions and 6D interrupts
	out_buff[0] = INT1_CFG;
	out_buff[1] = INT1_CFG_VAL;
	Chip_I2C_MasterSend(I2C0, ACCEL_ADDR, out_buff, sizeof(out_buff));
	out_buff[0] = INT1_THS;
	out_buff[1] = INT1_THS_VAL;
	Chip_I2C_MasterSend(I2C0, ACCEL_ADDR, out_buff, sizeof(out_buff));
	out_buff[0] = INT1_DUR;
	out_buff[1] = INT1_DUR_VAL;
	Chip_I2C_MasterSend(I2C0, ACCEL_ADDR, out_buff, sizeof(out_buff));
}

// Initialize accelerometer
void accel_init(void) {
	uint8_t out_buff[2];
	out_buff[0] = CTRL_REG1;
	out_buff[1] = CTRL_REG1_VAL;
	Chip_I2C_MasterSend(I2C0, ACCEL_ADDR, out_buff, sizeof(out_buff));
	out_buff[0] = CTRL_REG2;
	out_buff[1] = CTRL_REG2_VAL;
	Chip_I2C_MasterSend(I2C0, ACCEL_ADDR, out_buff, sizeof(out_buff));
	out_buff[0] = CTRL_REG3;
	out_buff[1] = CTRL_REG3_VAL;
	Chip_I2C_MasterSend(I2C0, ACCEL_ADDR, out_buff, sizeof(out_buff));
	out_buff[0] = CTRL_REG4;
	out_buff[1] = CTRL_REG4_VAL;
	Chip_I2C_MasterSend(I2C0, ACCEL_ADDR, out_buff, sizeof(out_buff));

	accel_enable_interrupt_output();
}




#endif
