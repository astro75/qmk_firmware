#pragma once
#include "quantum.h"

#ifndef TRACKBALL_ADDRESS
	#define TRACKBALL_ADDRESS 0x0A
#endif

#define TB_TIMEOUT 2000

#define TB_REG_LED_RED 0x00
#define TB_REG_LED_GRN 0x01
#define TB_REG_LED_BLU 0x02
#define TB_REG_LED_WHT 0x03

#define TB_REG_INT     0xF9
#define MSK_INT_TRIG   0b00000001
#define MSK_INT_EN     0b00000010

// User will read press event manually via `trackball_pressed`
#define TB_PRESS_MANUAL      NULL, -1, -1

// Call function on press state change
#define TB_PRESS_CALLBACK(f) f,    -1, -1

// Insert trackball button as a key in the matrix, row and column are zero-indexed
// Note that for a split keyboard, the right half is considered _below_ the left half
//  +---------+   +---------+ 
//  | a b c   |   |   h i j |
//  | d e f g |   | k l m n |
//  +---------+   +---------+
// becomes
//  a b c
//  d e f g
//    h i j
//  k l m n
#define TB_PRESS_MATRIX(r,c) NULL,  r,  c

// User will read movement manually via `trackball_movement`
#define TB_MOVE_MANUAL       NULL

// Call function on movement
#define TB_MOVE_CALLBACK(f)  f

#define TB_LED_MANUAL    -1
#define TB_LED_MIRROR(i)  i

typedef struct {
	int8_t x;
	int8_t y;
} tb_xy;

// Should be called with a combination of TB_PRESS_* and TB_MOVE_*
void trackball_init(void (*press_callback)(bool), int8_t matrix_row, int8_t matrix_column,
					void (*move_callback)(tb_xy),
					int8_t led_index);

bool trackball_pressed(void);
// Note: calling it zeroes the x & y values
tb_xy trackball_movement(void);

// This must be called from either `matrix_scan_kb` or `matrix_scan_user`
void trackball_scan(void);

void trackball_setrgb(uint8_t r, uint8_t g, uint8_t b);
void trackball_sethsv(uint8_t h, uint8_t s, uint8_t v);
