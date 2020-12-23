#include "trackball.h"
#include "i2c_master.h"

#include "rgblight.h"

#include "pointing_device.h"

#include "print.h"

// Data formats as used by the trackball
#define INPUT_BEGIN 0x04
#define MSK_BUTTON_STATE 0b10000000
typedef union {
	struct {
		uint8_t left;
		uint8_t right;
		uint8_t up;
		uint8_t down;
		// bit 1: button changed
		// bit 8: button state
		uint8_t button;
	} e;
	uint8_t arr[5];
} input_raw;

#define LED_BEGIN 0x00
typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t w;
} led_data;
typedef union {
	led_data e;
	uint8_t arr[4];
} led_raw;

#define REG_BEGIN 0xF9
#define MSK_INT_TRIGGERED 0b00000001
#define MSK_INT_OUT_EN    0b00000010
#define MSK_CTRL_SLEEP    0b00000001
#define MSK_CTRL_RESET    0b00000010
#define MSK_CTRL_FREAD    0b00000100
#define MSK_CTRL_FWRITE   0b00001000
typedef union {
	struct {
		uint8_t interrupt;
		uint8_t chip_id_lo; // Should be 0x11
		uint8_t chip_id_hi; // Should be 0xBA
		uint8_t version;    // Should be 0x01
		uint8_t i2c_addr;
		uint8_t ctrl;
	} e;
	uint8_t arr[6];
} reg_raw;

// User-provided callback definitions
void (*press_callback)(bool);
int8_t matrix_row, matrix_column;
void (*move_callback)(tb_xy);
int8_t led_index;

struct {
	uint8_t x;
	uint8_t y;
	bool xy_dirty;
	bool button;
	bool button_dirty;
	led_data led;
	bool led_dirty;
} state;

void trackball_init(void (*press)(bool), int8_t r, int8_t c, void (*move)(tb_xy), int8_t led_idx) {
	if (!is_keyboard_master()) return;

	// set up callbacks
	press_callback = press;
	matrix_row = r;
	matrix_column = c;
	move_callback = move;
	led_index = led_idx;

	// zero trackball accumulators
	input_raw b_i;
	i2c_readReg(TRACKBALL_ADDRESS << 1, INPUT_BEGIN, b_i.arr, sizeof(input_raw), TB_TIMEOUT);

	// zero our state
	state.x = 0;
	state.y = 0;
	state.xy_dirty = false;
	state.button = false;
	state.button_dirty = false;
	state.led.r = 0;
	state.led.g = 0;
	state.led.b = 0;
	state.led.w = 0;
	state.led_dirty = false;

	// zero led
	i2c_writeReg(TRACKBALL_ADDRESS << 1, LED_BEGIN, (uint8_t*)&state.led, sizeof(led_data), TB_TIMEOUT);
}

bool trackball_pressed(void) {
	bool val = state.button;
	state.button_dirty = false;
	return val;
}

tb_xy trackball_movement(void) {
	tb_xy val = {.x = state.x, .y = state.y};
	state.x = 0;
	state.y = 0;
	state.xy_dirty = false;
	return val;
}

void trackball_setrgb(uint8_t r, uint8_t g, uint8_t b) {
	state.led_dirty |= r != state.led.r || g != state.led.g || b != state.led.b;
	state.led.r = r;
	state.led.g = g;
	state.led.b = b;
	state.led.w = 0;
}

void trackball_sethsv(uint8_t h, uint8_t s, uint8_t v) {
	RGB out = hsv_to_rgb((HSV){.h = h, .s = s, .v = v});
	trackball_setrgb(out.r, out.g, out.b);
}


extern matrix_row_t matrix[MATRIX_ROWS]; // Defined in `quantum/matrix_common.c`
extern LED_TYPE led[RGBLED_NUM];         // Defined in `quantum/rgblight.c`
void trackball_scan(void) {
	// Write out LED state

	if (led_index >= 0) {
		// LED copies an underglow one
		trackball_setrgb(led[led_index].r, led[led_index].g, led[led_index].b);
	}
	if (state.led_dirty) {
		i2c_writeReg(TRACKBALL_ADDRESS << 1, LED_BEGIN, (uint8_t*)&state.led, sizeof(led_data), TB_TIMEOUT);
		state.led_dirty = false;
	}

	// Read new data

	// Check if anything actually changed
	// The matrix scan code is bottlenecked by I2C,
	// so we want to use the bus as little as possible
	uint8_t interrupt;
	if (I2C_STATUS_SUCCESS != i2c_readReg(TRACKBALL_ADDRESS << 1, REG_BEGIN + offsetof(reg_raw, e.interrupt), &interrupt, 1, TB_TIMEOUT)) return;
	if (!(interrupt & MSK_INT_TRIGGERED)) return;

	input_raw buf;
	if (I2C_STATUS_SUCCESS != i2c_readReg(TRACKBALL_ADDRESS << 1, INPUT_BEGIN, buf.arr, sizeof(input_raw), TB_TIMEOUT)) return;

	//uprintf("tb: L%d R%d U%d D%d S%d\n", buf.e.left, buf.e.right, buf.e.up, buf.e.down, buf.e.button);

	// Parse trackball data

	#if TRACKBALL_ORIENTATION == 0
        // Pimoroni text is up
        state.y += buf.e.left - buf.e.right,
        state.x += buf.e.down - buf.e.up,
	#elif TRACKBALL_ORIENTATION == 1
        // Pimoroni text is right
        state.y += buf.e.down - buf.e.up,
        state.x += buf.e.right - buf.e.left,
	#elif TRACKBALL_ORIENTATION == 2
        // Pimoroni text is down
        state.y += buf.e.right - buf.e.left,
        state.x += buf.e.up - buf.e.down,
	#else
        // Pimoroni text is left
        state.y += buf.e.up - buf.e.down,
        state.x += buf.e.left - buf.e.right,
	#endif
    state.xy_dirty = state.x != 0 || state.y != 0;

    bool new_button = buf.e.button & MSK_BUTTON_STATE;
    if (new_button != state.button) {
    	state.button = new_button;
    	state.button_dirty = true;
    }

    // Dispense events

	if (move_callback != NULL && state.xy_dirty) {
		(*move_callback)(trackball_movement());
	}

	if (press_callback != NULL && state.button_dirty) {
		(*press_callback)(trackball_pressed());
	} else if (matrix_row >= 0 && matrix_column >= 0) {
		matrix[matrix_row] &= ~(MATRIX_ROW_SHIFTER << matrix_column);
		matrix[matrix_row] |= trackball_pressed() ? (MATRIX_ROW_SHIFTER << matrix_column) : 0;
	}

	//uint32_t current = timer_readhires32();
	//uint32_t elapsed = current - last_update;
	//last_update = current;

	/*uint8_t tb_data[5];
	i2c_status_t status;
	status = i2c_readReg(TRACKBALL_ADDRESS << 1, TB_REG_INT, tb_data, 1, TB_TIMEOUT);
	if (status != I2C_STATUS_SUCCESS) {
		print("tb i2c failure\n");
	} else {
		if (tb_data[0] & MSK_INT_TRIG) {
			status = i2c_readReg(TRACKBALL_ADDRESS << 1, TB_REG_LEFT, tb_data, 5, TB_TIMEOUT);
			if (status != I2C_STATUS_SUCCESS) {
				print("tb i2c failure");
				return;
			} else {
				//uprintf("tb: L%d R%d U%d D%d S%d\n", tb_data[0], tb_data[1], tb_data[2], tb_data[3], tb_data[4]);
				for (int i = 0; i < 4; i++) {
					tb_xy[i] += tb_data[i];
				}
			}
		}
	}

	if (timer_elapsed32(last_update) < 100) return;
	last_update = timer_read32();

	int8_t ticks = tb_xy[0] + tb_xy[2] - tb_xy[1] - tb_xy[3];
	if (ticks == 0) return;

	//uprintf("tb: %d ticks\n", ticks);

	report_mouse_t report = {};
	int8_t x = (tb_xy[0] - tb_xy[1]) * ticks;
	int8_t y = (tb_xy[2] - tb_xy[3]) * ticks;
	report.x += x;
	report.y += y;
	host_mouse_send(&report);

	for (int i = 0; i < 4; i++) {
		tb_xy[i] = 0;
	}*/
}
