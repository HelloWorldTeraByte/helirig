PERIPHERALS = pit adc
DRIVERS = usb_serial rf pacer
TARGET = hat.bin

SRC = hat.c imu.c imu.h joystick.c joystick.h
OPT = -O2

ifneq ($(BOARD), hat)
$(error BOARD should be set to hat!)
endif

include ../../boards/board.mk
