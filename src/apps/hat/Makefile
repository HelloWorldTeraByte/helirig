PERIPHERALS = pit adc
DRIVERS = usb_serial rf pacer mpu button
TARGET = hat.bin

SRC = hat.c imu.c imu.h joystick.c joystick.h ../utils/radio_comm.c ../utils/radio_comm.h ../utils/usb_comm.c ../utils/usb_comm.h ../utils/command.h ../utils/command.c power.c power.h

OPT = -O0

ifneq ($(BOARD), hat)
$(error BOARD should be set to hat!)
endif

include ../../boards/board.mk
