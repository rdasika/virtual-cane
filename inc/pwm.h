// pwm.h

#ifndef PWM_H
#define PWM_H

#define CNT_ENA_SHIFT     	0
#define PWM_ENA_SHIFT     	2

#define RST_ON_MR0_SHIFT   	1

#define MR0_ENA_SHIFT      	0
#define MR1_ENA_SHIFT      	1

#define SBIT_PWMENA1   		9

#define PWM_1          		0

// pointer to PWM timer module's PCR
volatile static LPC_PWM_T * pTMR = (LPC_PWM_T*) LPC_PWM1;

// ** PUBLIC ** //


// ** PRIVATE ** //

// input: distance (cm)
// output: pwm sound
// currently usable between 3cm and ~1597 cm
void distance_to_sound(uint16_t in_distance)
{
	// reset pwm counter
    Chip_PWM_Reset(pTMR);

    // Set pins to pwm ONLY MATTERS FIRST TIME
    LPC_IOCON->PINSEL[4] = (1<<PWM_1);

    // Enable counters ONLY MATTERS FIRST TIME
    pTMR->TCR |= (1<<CNT_ENA_SHIFT);
    pTMR->TCR |= (1<<PWM_ENA_SHIFT);

    // Set prescale value to x100 ONLY MATTERS FIRST TIME
    uint32_t prescale = 99;
    pTMR->PR  =  prescale;

    // Timer resets on MR0
    pTMR->MCR = (1<<RST_ON_MR0_SHIFT);

    // make sure we don't have a distance that will scale to inaudible sounds
    if (in_distance < 23) in_distance = 23;

    // Calculate pulse width and period
    uint32_t period =  2 * ((uint32_t)in_distance) + 16 - 2*20 + 15;
    uint32_t width = period / 2;

    // Set match register values
    pTMR->MR0 = period;
    pTMR->MR1 = width;

    // Load new match values
    pTMR->LER |= (1<<MR0_ENA_SHIFT);
	pTMR->LER |= (1<<MR1_ENA_SHIFT);

    // Enable pwm output on pwm pin 1
    pTMR->PCR = (1<<SBIT_PWMENA1);
}

void direct_output(uint32_t period)
{
	// reset pwm counter
    Chip_PWM_Reset(pTMR);

    // Set pins to pwm ONLY MATTERS FIRST TIME
    LPC_IOCON->PINSEL[4] = (1<<PWM_1);

    // Enable counters ONLY MATTERS FIRST TIME
    pTMR->TCR |= (1<<CNT_ENA_SHIFT);
    pTMR->TCR |= (1<<PWM_ENA_SHIFT);

    // Set prescale value to x100 ONLY MATTERS FIRST TIME
    uint32_t prescale = 99;
    pTMR->PR  =  prescale;

    // Timer resets on MR0
    pTMR->MCR = (1<<RST_ON_MR0_SHIFT);

    uint32_t width = period / 2;

    // Set match register values
    pTMR->MR0 = period;
    pTMR->MR1 = width;

    // Load new match values
    pTMR->LER |= (1<<MR0_ENA_SHIFT);
	pTMR->LER |= (1<<MR1_ENA_SHIFT);

    // Enable pwm output on pwm pin 1
    pTMR->PCR = (1<<SBIT_PWMENA1);
}

void pwm_sleep()
{
	//Chip_PWM_DeInit(pTMR);
	pTMR->TCR &= ~(1<<CNT_ENA_SHIFT);
	pTMR->TCR &= ~(1<<PWM_ENA_SHIFT);
}


#endif /* PWM_H */
