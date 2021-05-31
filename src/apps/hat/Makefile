PERIPHERALS = pit adc pwm
DRIVERS = usb_serial rf pacer mpu button piezo ledtape
TARGET = hat.bin

SRC = hat.c imu.c joystick.c ../utils/radio_comm.c ../utils/usb_comm.c ../utils/command.c ../utils/led_tape.c buzzer.c 

OPT = -O2

ifneq ($(BOARD), hat)
$(error BOARD should be set to hat!)
endif

include ../../boards/board.mk
