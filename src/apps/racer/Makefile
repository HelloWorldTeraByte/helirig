PERIPHERALS = pwm pit
DRIVERS = pacer led rf
TARGET = racer.bin

SRC = racer.c motors.c radio_comm.c
OPT = -O2

ifneq ($(BOARD), racer)
$(error BOARD should be set to racer!)
endif

include ../../boards/board.mk
