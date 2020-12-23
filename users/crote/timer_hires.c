#include "timer_hires.h"

// For an ATmega32U4 @ 16Mhz, loops after 262 ms
uint16_t timer_readhires(void) {
	return (timer_read() * TIMER_RAW_TOP) + TIMER_RAW;
}

// For an ATmega32U4 @ 16Mhz, loops after 4h 46m
uint32_t timer_readhires32(void) {
	return (timer_read32() * TIMER_RAW_TOP) + TIMER_RAW;
}
