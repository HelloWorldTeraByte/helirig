#include "servos.h"
#include "pwm.h"
#include "target.h"

static const pwm_cfg_t servo1_cfg =
{
    .pio = SERVO1_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR(SERVO_PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR(SERVO_PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

static pwm_t servo1;

void servos_init(void)
{
    servo1 = pwm_init(&servo1_cfg);
    pwm_channels_start(pwm_channel_mask(servo1));
}

void servo1_duty_set(uint8_t duty)
{
    pwm_duty_set(servo1, PWM_DUTY_DIVISOR(SERVO_PWM_FREQ_HZ, duty));
}

void servo_drop_bananas(void)
{

}

