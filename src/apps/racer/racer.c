#include "target.h"

#include "pwm.h"
#include "pio.h"
#include "delay.h"

#define PWM_FREQ_HZ 100e3

static const pwm_cfg_t mtr0_pwm_cfg_0 =
{
    .pio = MOTOR_LEFT_PWM_PIO_0,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t mtr0_pwm_cfg_1 =
{
    .pio = MOTOR_LEFT_PWM_PIO_1,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

void
motor_left_set(int8_t speed) {
    pwm_t mtr0_pwm_0;
    pwm_t mtr0_pwm_1;

    // Speed [-100 - +100] sets the PWM duty cycle and pin modes appropriately
    if (speed > 0)
    {
        // update PWM duty for H0 for forward
        // set PA23 as PWM
        // set PA0 as HIGH
        pwm_channels_stop(pwm_channel_mask(mtr0_pwm_0));
        pio_config_set(PA0_PIO, PIO_OUTPUT_HIGH);
        mtr0_pwm_1 = pwm_init(&mtr0_pwm_cfg_1);
        pwm_channels_start(pwm_channel_mask(mtr0_pwm_1));
    }
    else
    {
        // update PWM duty for H0 for reverse
        // set PA23 HIGH
        // set PA0 as PWM
        pwm_channels_stop(pwm_channel_mask(mtr0_pwm_1));
        pio_config_set(PA23_PIO, PIO_OUTPUT_HIGH);
        mtr0_pwm_0 = pwm_init(&mtr0_pwm_cfg_0);
        pwm_channels_start(pwm_channel_mask(mtr0_pwm_0));
    }
}

int main(void)
{
    while (1) {
        motor_left_set(100);
        delay_ms(2000);
        motor_left_set(-100);
        delay_ms(2000);
    }
    return 0;
}
