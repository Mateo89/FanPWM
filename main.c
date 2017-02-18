/*
 * main.c
 *
 *  Created on: 7 wrz 2016
 *      Author: jasko_m
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "ds18b20/ds18b20.h"
#include "types.h"

#define TEMP_START 30
#define MAX_PWM_VALUE 47

#define MAX_TEMP MAX_PWM_VALUE + TEMP_START

#define PWM_STEP_1 0
#define PWM_STEP_2 10
#define PWM_STEP_3 20
#define PWM_STEP_4 30

int value = 0;

TTEMP TEMP;

void set_pwm_step(int i){
	OCR0B = ( MAX_PWM_VALUE - i);
}


int main(void) {

	if(MCUSR & _BV(WDRF)){            // If a reset was caused by the Watchdog Timer...
		    MCUSR &= ~_BV(WDRF);                 // Clear the WDT reset flag
		    WDTCR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
		    WDTCR = 0x00;                      // Disable the WDT
		}

	DDRB |= (1<<PB1);

	//_SFR_IO8(0x25) |= ( 1 << 0) ; // wy³¹czenie ADC

	WDTCR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
	WDTCR =  _BV(WDP3) | _BV(WDP0);  // usstawienie preskalera dla watchdoga na oko³o 8s


	TCCR0A |= (1 << COM0B1) | ( 1 << COM0B0 ) | (1 << COM0A1) | ( 1 << COM0A0 ) | (1<<  WGM01 ) | (1 << WGM00 );
	TCCR0B |= (1 << WGM02 ) | ( 1 << CS01 );
	OCR0A = MAX_PWM_VALUE;
	OCR0B = MAX_PWM_VALUE;
	while(1){

		ds18b20_gettemp(&TEMP);

		if ( TEMP.cel >= TEMP_START && TEMP.cel < MAX_TEMP)
			set_pwm_step(TEMP.cel - TEMP_START);

		if ( TEMP.cel >= MAX_TEMP )
			set_pwm_step(MAX_PWM_VALUE);

		MCUSR |= _BV(WDRF);

		_delay_ms(1000);
	}
}



