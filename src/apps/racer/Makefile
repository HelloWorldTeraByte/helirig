PERIPHERALS = pwm pit adc
DRIVERS = pacer led rf usb_serial ledtape button
TARGET = racer.bin

SRC = racer.c motors.c servos.c bumper.c power.c buttons.c servos.c state_manager.c ./utils/radio_comm.c ../utils/usb_comm.c ../utils/command.c ../utils/led_tape.c 
OPT = -O2

ifneq ($(BOARD), racer)
$(error BOARD should be set to racer!)
endif

include ../../boards/board.mk