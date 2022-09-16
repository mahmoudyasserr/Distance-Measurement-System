/*
 * ultrasonic.c
 *
 *  Created on: Jul 12, 2022
 *      Author: Mahmoud Yasser
 */

#include "ultrasonic.h"
#include <util/delay.h>
#include "icu.h"
#include "gpio.h"

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;

void Ultrasonic_init(void){

	/*configure the ICU with Fcpu/8 , detect rising edge initially*/
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

	Icu_setCallBack(Ultrasonic_edgeProcessing);
	Icu_init(&Icu_Config);
	GPIO_setupPinDirection(Ultrasonic_Trig_PORT_ID, Ultrasonic_Trig_PIN_ID, PIN_OUTPUT);
}

void Ultrasonic_Trigger(void){
	/*Trigger the ultrasonic with a pulse of more than 10us*/
	GPIO_writePin(Ultrasonic_Trig_PORT_ID, Ultrasonic_Trig_PIN_ID, LOGIC_HIGH);
	_delay_us(22);
	GPIO_writePin(Ultrasonic_Trig_PORT_ID, Ultrasonic_Trig_PIN_ID, LOGIC_LOW);
}


uint16 Ultrasonic_readDistance(void){
	uint16 distance;
	Ultrasonic_Trigger();

	/*calculating the distance in cm*/
	distance = ((float)g_timeHigh) / 58.8;

	return distance;

}

void Ultrasonic_edgeProcessing(void){
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		g_edgeCount = 0;
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}

}
