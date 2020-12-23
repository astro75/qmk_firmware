#pragma once

#if !defined(__AVR__)
	#error "High-resolution timers are currently only supported on AVR"
#endif

#include "timer.h"

uint16_t timer_readhires(void);
uint32_t timer_readhires32(void);
