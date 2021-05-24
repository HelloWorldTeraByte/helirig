PERIPHERALS = pwm pit
DRIVERS = pacer led rf usb_serial ledtape
TARGET = racer.bin

SRC = racer.c motors.c ../utils/radio_comm.c servos.c ../utils/usb_comm.c  ../utils/command.h ../utils/command.c ledbuffer.c
OPT = -O2

ifneq ($(BOARD), racer)
$(error BOARD should be set to racer!)
endif

include ../../boards/board.mk