#ifndef IMU_H_
#define IMU_H_
#include <stdbool.h>

struct Command imu_get_speed_command(void);
void imu_init(void);
bool get_jump_status(void);




#endif /*IMU_H*/