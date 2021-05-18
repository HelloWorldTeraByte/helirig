PERIPHERALS = pwm pit
DRIVERS = pacer led rf usb_serial 
TARGET = racer.bin

SRC = racer.c motors.c radio_comm.c servos.c usb_comm.c
OPT = -O0

ifneq ($(BOARD), racer)
$(error BOARD should be set to racer!)
endif

include ../../boards/board.mk
