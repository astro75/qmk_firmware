# Disable oled, default on Kyria but we don't have one
OLED_DRIVER_ENABLE = no

# Builtin features
CONSOLE_ENABLE = yes

# Custom features
TRACKBALL_ENABLE = yes

ifeq ($(strip $(ENCODER_ENABLE)), yes)
	SRC += enc.c
endif
