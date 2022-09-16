/*
 * main.c
 *
 *  Created on: Jul 12, 2022
 *      Author: Mahmoud Yasser
 */

#include <avr/common.h>
#include "ultrasonic.h"
#include "gpio.h"
#include "std_types.h"
#include "lcd.h"


int main(void) {
	SREG |= (1 << 7);
	LCD_init();
	Ultrasonic_init();

	while (1) {
		LCD_moveCursor(0, 0);
		LCD_displayString("Distance = ");
		uint16 distance = Ultrasonic_readDistance();
		LCD_intgerToString(distance);
		LCD_displayString("  ");
		LCD_moveCursor(1, 0);
		LCD_displayString("cm");
	}
	return 0;
}

