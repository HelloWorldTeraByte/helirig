#ifndef POWER_H_
#define POWER_H_

#include <stdbool.h>

#define POWER_SENSE_R1 82.00 //kohms
#define POWER_SENSE_R2 47.0  //kohms
#define POWER_SENSE_CORRECTION 0
#define LOW_POWER_THRESH 6.0 //volt

void power_sense_init(void);
float power_batt_read(void);
bool power_is_batt_low(void);

#endif /*POWER_H_*/