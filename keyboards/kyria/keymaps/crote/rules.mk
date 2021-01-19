# Disable oled, default on Kyria but we don't have one
OLED_DRIVER_ENABLE = no

# Builtin features
CONSOLE_ENABLE = yes

TRACKBALL_ENABLE = yes
POINTING_DEVICE_ENABLE = yes

ifeq ($(strip $(ENCODER_ENABLE)), yes)
	SRC += enc.c
endif
