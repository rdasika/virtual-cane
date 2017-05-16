/*
===============================================================================
 Name        : lidar.h
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "board.h"
#include "i2c.h"
#include <cr_section_macros.h>

// Addresses
#define WRITE_ADD 				0xC4
#define READ_ADD				0xC5
#define TARGET_ADD				0x62
#define ACQ_COMMAND_ADD			0x00
#define STATUS_ADD				0x01
#define FULL_DELAY_HIGH_ADD		0X0F
#define FULL_DELAY_LOW_ADD		0x10
#define POWER_ADD				0x65
#define SIG_COUNT_ADD			0x02
#define DISTANCE				0x8f


// Values
#define ACQ_COMMAND_RESET			0x00
#define	ACQ_COMMAND_ACCU			0x04
#define ACQ_COMMAND_NON				0x03
#define POWER_OFF					0x01
#define POWER_ON					0x00
#define SIG_COUNT_VAL				0x45

// Masks
#define MASK					0b00000001

// Power Enable Pin and Pad H[23]
#define PWR_EN_PORT				0
#define PWR_EN_PIN				21

// Boolean
#define LOW						0
#define HIGH					1


// Wake up LIDAR
void lidar_sleep(void)
{
	// set power enable low
	Chip_GPIO_SetPinState(LPC_GPIO, PWR_EN_PORT, PWR_EN_PIN, LOW);
}

// Put LIDAR to sleep
void lidar_wake(void)
{
	// set power enable high
	Chip_GPIO_SetPinState(LPC_GPIO, PWR_EN_PORT, PWR_EN_PIN, HIGH);
}

// Initialize LIDAR
void lidar_init(void)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, PWR_EN_PORT, PWR_EN_PIN);
}

// Request a measurement from LIDAR
void lidar_request(void){
	uint8_t set[2];

	set[0] = ACQ_COMMAND_ADD;
	set[1] = ACQ_COMMAND_NON;

	Chip_I2C_MasterSend(I2C0, TARGET_ADD, set, sizeof(set));
}

// Read a measurement from LIDAR
uint16_t lidar_read(void){

	uint8_t out_buff1[1];
	uint8_t out_buff0[1];
	uint8_t in_buff[2];
	out_buff1[0] = FULL_DELAY_HIGH_ADD;
	out_buff0[0] = FULL_DELAY_LOW_ADD;

	Chip_I2C_MasterSend(I2C0, TARGET_ADD, out_buff1, sizeof(out_buff1));
	Chip_I2C_MasterRead(I2C0, TARGET_ADD, &in_buff[1], sizeof(in_buff[1]));
	Chip_I2C_MasterSend(I2C0, TARGET_ADD, out_buff0, sizeof(out_buff0));
	Chip_I2C_MasterRead(I2C0, TARGET_ADD, &in_buff[0], sizeof(in_buff[0]));

	uint16_t result;
	memcpy(&result, in_buff, sizeof(result));
	//result = in_buff1[0] << 8 | in_buff2[0];

	return result;
}

// Check whether LIDAR is done with measurement
int lidar_status(void){

	uint8_t in_buff[1];
	in_buff[0] = STATUS_ADD;
	int i;

	Chip_I2C_MasterSend(I2C0, TARGET_ADD, in_buff, sizeof(in_buff));
	Chip_I2C_MasterRead(I2C0, TARGET_ADD, in_buff, sizeof(in_buff));

	if((in_buff[0] & MASK) == 1)
		i = 1;
	else
		i = 0;

	return i;
}
