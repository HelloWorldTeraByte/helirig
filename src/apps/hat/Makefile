PERIPHERALS = pit adc
DRIVERS = usb_serial rf pacer mpu
TARGET = hat.bin

SRC = hat.c imu.c imu.h joystick.c joystick.h radio_comm.c radio_comm.h usb_comm.c usb_comm.h
OPT = -O0

ifneq ($(BOARD), hat)
$(error BOARD should be set to hat!)
endif

include ../../boards/board.mk
