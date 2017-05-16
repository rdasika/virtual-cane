#ifndef HAPTIC_H
#define HAPTIC_H

#include "i2c.h"

// Register addresses
#define DRIVER_ADDR                         0x5A
#define STATUS_REG_ADDR                     0x00
#define MODE_REG_ADDR                       0x01
#define REALTIME_PLAYBACK_INPUT_REG_ADDR    0x02
#define LIBRARY_SELECTION_REG_ADDR          0x03
#define WAVEFORM_SEQUENCER_0_REG_ADDR       0x04
#define GO_REG_ADDR                         0x0C
#define RATED_VOLTAGE_REG_ADDR              0x16 // WRITE ONCE (LOOK INTO THIS)
#define OVERDRIVE_CLAMP_VOLTAGE_REG_ADDR    0x17 // WRITE ONCE
#define FEEDBACK_CONTROL_REG_ADDR           0x1A // WRITE ONCE
#define CONTROL1_REG_ADDR                   0x1B
#define CONTROL2_REG_ADDR                   0x1C
#define CONTROL4_REG_ADDR                   0x1E


// Mode register values
#define TRIG_MODE			0b00000000
#define RTP_MODE            0b00000101 // Real Time Playback
#define CAL_MODE            0b00000111
#define STANDBY_MODE        0b01000000

#define BIDIR_INPUT_MASK    0b01111111 // activate unidirectional input (active low)
#define LIBRARY_6_OR_MASK   0b00000110
#define LIBRARY_6_AND_MASK  0b11111110
#define GO_MASK             0b00000001
#define BUZZ_WAVEFORM       16
// 16 is good - 1 second buzz
// 52 - series of pulses

#define BUZZ_PER_SEC (2)	// buzzes per sec

// forward declaration
void haptic_standby_mode(void);
void haptic_playback_mode(void);
void haptic_calibrate(void);


// Put driver in active mode with intensity of 0
void haptic_trigger_mode(void)
{
    uint8_t out_buff[2];

    // Real Time Playback mode
    out_buff[0] = MODE_REG_ADDR;
    out_buff[1] = TRIG_MODE;
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));
}


// Put driver in active mode with intensity of 0
void haptic_set_waveform_queue(void)
{
    uint8_t out_buff[2];

    // Real Time Playback mode
    out_buff[0] = WAVEFORM_SEQUENCER_0_REG_ADDR;
    out_buff[1] = BUZZ_WAVEFORM;
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));
}


// Put driver in active mode with intensity of 0
void haptic_select_waveform_library(void)
{
    uint8_t in_buff[1];
    uint8_t out_buff[2];

    // Unidirectional input
    // this only needs to be done on startup but I didn't want a seperate init() function
    // Real Time Playback mode
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, LIBRARY_SELECTION_REG_ADDR, in_buff, sizeof(in_buff));
    in_buff[0] &= LIBRARY_6_AND_MASK;
    in_buff[0] |= LIBRARY_6_OR_MASK;
    out_buff[0] = LIBRARY_SELECTION_REG_ADDR;
    out_buff[1] = in_buff[0];
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));
}


void haptic_trigger_waveform(void)
{
    uint8_t in_buff[1];
    uint8_t out_buff[2];

    // Real Time Playback mode
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, GO_REG_ADDR, in_buff, sizeof(in_buff));
    in_buff[0] |= GO_MASK;
    out_buff[0] = GO_REG_ADDR;
    out_buff[1] = in_buff[0];
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));
}


// ** PUBLIC ** //


void haptic_init()
{
	haptic_calibrate();

	volatile int i = 0;
	for (i = 0; i < 4000000; ++i){}

    //haptic_set_waveform_queue(); // move to startup for quicker execution
    //haptic_select_waveform_library(); // move to startup for quicker execution
    //haptic_intensity(0xFF); // set playback register to full
}


// Sends a haptic notification to the user
void haptic_notify(void)
{
    haptic_trigger_mode();
    haptic_trigger_waveform();
}


// Puts driver in playback mode
// Ready to output 8 bit values using haptic_intensity()
void haptic_playback_mode(void)
{
    uint8_t in_buff[1];
    uint8_t out_buff[2];

    // Unidirectional input
    // this only needs to be done on startup but I didn't want a seperate init() function
    // Real Time Playback mode
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, CONTROL2_REG_ADDR, in_buff, sizeof(in_buff));
    in_buff[0] &= BIDIR_INPUT_MASK;
    out_buff[0] = CONTROL2_REG_ADDR;
    out_buff[1] = in_buff[0];
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));

    // Real Time Playback mode
    out_buff[0] = MODE_REG_ADDR;
    out_buff[1] = RTP_MODE;
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));
}


// Put driver in standby mode
void haptic_standby_mode(void)
{
    uint8_t out_buff[2];

    // Standby mode
    out_buff[0] = MODE_REG_ADDR;
    out_buff[1] = STANDBY_MODE;

    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));
}

// Write intensity (voltage amplitude) value between 0-255.
// OD and Breaking are handled by DRV2605L
void haptic_intensity(uint8_t voltage)
{
    uint8_t out_buff[2];

    // Write value to RTP Input Register
    out_buff[0] = REALTIME_PLAYBACK_INPUT_REG_ADDR;
    out_buff[1] = voltage;
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));
}


// Calibrate driver and places it in standby
void haptic_calibrate(void)
{
    uint8_t in_buff[1];
    uint8_t out_buff[2];

    // Check Status
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, STATUS_REG_ADDR, in_buff, sizeof(in_buff));


    // Standby mode (to be safe)
    out_buff[0] = MODE_REG_ADDR;
    out_buff[1] = STANDBY_MODE;
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));

    // Calibration mode
    out_buff[0] = MODE_REG_ADDR;
    out_buff[1] = CAL_MODE;
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));

    // N_ERM_LRA
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, FEEDBACK_CONTROL_REG_ADDR, in_buff, sizeof(in_buff));
    in_buff[0] |= 0b10000000; // set N_ERM_LRA high
    out_buff[0] = FEEDBACK_CONTROL_REG_ADDR;
    out_buff[1] = in_buff[0];
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));

    // FB_BRAKE_FACTOR
    // default
    // LOOP_GAIN
    // default

    // RATED_VOLTAGE
    out_buff[0] = RATED_VOLTAGE_REG_ADDR;
    out_buff[1] = 0x53; // decimal 83, see Tom's Notes
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));

    // OD_CLAMP
    out_buff[0] = OVERDRIVE_CLAMP_VOLTAGE_REG_ADDR;
    out_buff[1] = 0x60; // decimal 96, see Tom's Notes
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));

    // DRIVE_TIME
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, CONTROL1_REG_ADDR, in_buff, sizeof(in_buff));
    in_buff[0] &= 0b11100000;
    in_buff[0] |= 0b00010111; // decimal 23, see Tom's Notes
    out_buff[0] = CONTROL1_REG_ADDR;
    out_buff[1] = in_buff[0];
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));

    // SAMPLE_TIME
    // default (check if you see issues)
    // BLANKING_TIME
    // default
    // IDISS_TIME
    // default
    // ZC_DET_TIME
    // default

    // GO Register
    out_buff[0] = GO_REG_ADDR;
    out_buff[1] = 0x01; // go!
    Chip_I2C_MasterSend(I2C0, DRIVER_ADDR, out_buff, sizeof(out_buff));

    // Check Status
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, STATUS_REG_ADDR, in_buff, sizeof(in_buff));
}


// ** Debug ** //


void driver_read_feedback_control(void)
{
    uint8_t in_buff[1];
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, FEEDBACK_CONTROL_REG_ADDR, in_buff, sizeof(in_buff));
}

// read rated voltage
void driver_read_rated_voltage(void)
{
    uint8_t in_buff[1];
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, RATED_VOLTAGE_REG_ADDR, in_buff, sizeof(in_buff));
}


// read od clamp
void driver_read_od_clamp(void)
{
    uint8_t in_buff[1];
    Chip_I2C_MasterCmdRead(I2C0, DRIVER_ADDR, OVERDRIVE_CLAMP_VOLTAGE_REG_ADDR, in_buff, sizeof(in_buff));
}


#endif /* HAPTIC_H */
