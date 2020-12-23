ifeq ($(strip $(TRACKBALL_ENABLE)), yes)
	OPT_DEFS += -DTRACKBALL_ENABLE
	SRC += trackball.c timer_hires.c
endif
