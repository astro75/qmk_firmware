ifeq ($(strip $(TRACKBALL_ENABLE_OLD)), yes)
	OPT_DEFS += -DTRACKBALL_ENABLE_OLD
	SRC += trackball.c timer_hires.c
endif
