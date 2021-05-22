#ifndef IMU_H_
#define IMU_H_

void imu_read(int* motor_input_1_out, int* motor_input_2_out, bool* mid_air);
void imu_init(void);




#endif /*IMU_H*/