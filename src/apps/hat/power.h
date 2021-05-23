#ifndef POWER_H_
#define POWER_H_

#define POWER_SENSE_R1 100.0 //kohms
#define POWER_SENSE_R2 76.8 //kohms
#define POWER_SENSE_CORRECTION 430
#define LOW_POWER_THRESH 6.0 //volt

#include <stdbool.h>

void power_sense_init(void);
float read_bat_voltage(void);
bool is_low_bat(void);


#endif /*POWER_H_*/