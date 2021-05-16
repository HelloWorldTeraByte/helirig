#include "motors.h"
#include "target.h"
#include "pwm.h"
#include "pio.h"

static const pwm_cfg_t mtr_l_pwm_cfg =
{
    .pio = MOTOR_LEFT_PWM_PIO_0,
    .period = PWM_PERIOD_DIVISOR (MOTOR_PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (MOTOR_PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t mtr_r_pwm_cfg =
{
    .pio = MOTOR_RIGHT_PWM_PIO_0,
    .period = PWM_PERIOD_DIVISOR (MOTOR_PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (MOTOR_PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

static pwm_t mtr_l_pwm;
static pwm_t mtr_r_pwm;
 
void motors_init(void)
{
    mtr_l_pwm = pwm_init(&mtr_l_pwm_cfg);
    mtr_r_pwm = pwm_init(&mtr_r_pwm_cfg);
 
   pwm_channels_start(pwm_channel_mask(mtr_l_pwm) | pwm_channel_mask(mtr_r_pwm));
   
   pio_config_set(MOTOR_SLEEP_PIO, PIO_OUTPUT_HIGH);
}

void motor_left_set(int8_t speed)
{
    // Speed [-100 - +100] sets the PWM duty cycle and pin modes appropriately
    //Clamps the speed between -100 and 100
    if(speed > 100)
        speed = 100;
    if(speed < -100)
        speed = -100;

    if (speed > 0)
    {
        // update PWM duty for H0 for forward
        pwm_duty_set(mtr_l_pwm, PWM_DUTY_DIVISOR(MOTOR_PWM_FREQ_HZ, speed));
        // set PA23 as PWM
        pio_config_set(MOTOR_LEFT_PWM_PIO_0, PIO_PERIPH_B);
        // set PA0 as HIGH
        pio_config_set(MOTOR_LEFT_PWM_PIO_1, PIO_OUTPUT_HIGH);
    }
    else
    {
        pwm_duty_set(mtr_l_pwm, PWM_DUTY_DIVISOR(MOTOR_PWM_FREQ_HZ, -1*speed));
        // update PWM duty for H0 for reverse
        // set PA23 HIGH
        pio_config_set(MOTOR_LEFT_PWM_PIO_0, PIO_OUTPUT_HIGH);
        // set PA0 as PWM
        pio_config_set(MOTOR_LEFT_PWM_PIO_1, PIO_PERIPH_A);
    }
}

void motor_right_set(int8_t speed)
{
    // Speed [-100 - +100] sets the PWM duty cycle and pin modes appropriately
    //Clamps the speed between -100 and 100
    if(speed > 100)
        speed = 100;
    if(speed < -100)
        speed = -100;

    if (speed > 0)
    {
        // update PWM duty for H0 for forward
        pwm_duty_set(mtr_r_pwm, PWM_DUTY_DIVISOR(MOTOR_PWM_FREQ_HZ, speed));
        // set PA23 as PWM
        pio_config_set(MOTOR_RIGHT_PWM_PIO_0, PIO_PERIPH_A);
        // set PA0 as HIGH
        pio_config_set(MOTOR_RIGHT_PWM_PIO_1, PIO_OUTPUT_HIGH);
    }
    else
    {
        pwm_duty_set(mtr_r_pwm, PWM_DUTY_DIVISOR(MOTOR_PWM_FREQ_HZ, -1*speed));
        // update PWM duty for H0 for reverse
        // set PA23 HIGH
        pio_config_set(MOTOR_RIGHT_PWM_PIO_0, PIO_OUTPUT_HIGH);
        // set PA0 as PWM
        pio_config_set(MOTOR_RIGHT_PWM_PIO_1, PIO_PERIPH_A);
    }
}