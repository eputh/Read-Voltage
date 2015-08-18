#include "avr.h"

int key_pressed(r,c) {
	if (!GET_BIT(PINC, r)) {
		if (!GET_BIT(PORTC, c))
		return 1;
	}
	return 0;
}


void set_to_NC(c) {
	for (int x=4; x < 8; x++) {
		if (x != c) {
			CLR_BIT(DDRC, x);
			CLR_BIT(PORTC, x);
		}
	}
}


unsigned char get_key(void) {
	for (int r=0; r < 4; r++) {
		CLR_BIT(DDRC, r);
		SET_BIT(PORTC, r);
		for (int c=4; c < 8; c++) {
			SET_BIT(DDRC, c);
			CLR_BIT(PORTC, c);
			set_to_NC(c);
			if (key_pressed(r,c)) {
				return (r*4) + c - 3;
			}
		}
	}
	return 0;
}