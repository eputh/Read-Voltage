/*
 * project4.c
 *
 * Created: 5/19/2015 3:18:19 PM
 *  Author: Emily Puth and Chelsea Faustino
 */ 


#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include "keypad.h"


unsigned short get_sample() {
	unsigned short s;
	ADMUX |= (1 << REFS0);			// assigning 1 to REFS0
	ADCSRA |= (1 << ADEN);			// turn on ADC
	ADCSRA |= (1 << ADSC);			// ADSC: Start Conversion
	while (ADCSRA & (1 << ADSC));	// wait for a change in the voltage
	s = ADC;
	return s;
}


int main(void)
{
    ini_lcd();
    clr_lcd();
	
	char resetLine1 = "ins:----max:----", resetLine2 = "min:----avg:----";
	char instVoltage[9]; char maxVoltage[9]; char minVoltage[9]; char totalVoltage[9];
	int maxVolt, minVolt, avgVolt, totalVolt, voltCount;

	while(1) {		
		// button "D" is pressed to initiate sampling and display results
		if (get_key() == 16) {
			while(1) {
				// button "C" is pressed to clear and reset data
				if (get_key() == 12) {
					maxVolt = 0;
					minVolt = 0;
					avgVolt = 0;
					totalVolt = 0;
					voltCount = 0;
					
					pos_lcd(0,0);
					puts_lcd2(resetLine1);
					pos_lcd(1,0);
					puts_lcd2(resetLine2);
					
					break;
				}
				
				unsigned short s = get_sample();
				float x = (float)s;
				float y = x/1023.0 * 5.0;
				int z = y * 100;
				
				//sprintf(instVoltage, "%i", z);
				//pos_lcd(0,0);
				//puts_lcd2(instVoltage);
				
				voltCount++;
				
				if (voltCount == 1) {
					maxVolt = z;
					minVolt = z;
				}
				
				if (maxVolt < z) {
					maxVolt = z;
					} else if (z < minVolt) {
					minVolt = z;
				}
				
				totalVolt += z;
				avgVolt = totalVolt/voltCount;
				
				wait_avr(500);
				clr_lcd();

				 //Print all voltages to LCD
				 sprintf(instVoltage, "ins:%i.%02i", z/100, z%100);
				 pos_lcd(0,0);
				 puts_lcd2(instVoltage);
				 
				 sprintf(maxVoltage, "max:%i.%02i", maxVolt/100, maxVolt%100);
				 pos_lcd(0,8);
				 puts_lcd2(maxVoltage);
				 
				 sprintf(minVoltage, "min:%i.%02i", minVolt/100, minVolt%100);
				 pos_lcd(1,0);
				 puts_lcd2(minVoltage);
				 
				 sprintf(totalVoltage, "avg:%i.%02i", avgVolt/100, avgVolt%100);
				 pos_lcd(1,8);
				 puts_lcd2(totalVoltage);
			}
		}
	}
}

