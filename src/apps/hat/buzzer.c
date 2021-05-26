#include "piezo.h"
#include "piezo_beep.h"
#include "target.h"
#include "mcu.h"
#include "pwm.h"
#include "pio.h"
#include <stdbool.h>
#include "buzzer.h"

#include "music.h"


#define PWM_FREQ_HZ 100e3

static const pwm_cfg_t pwm_cfg =
{
    .pio = PIEZO_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW,
    .stop_state = PIO_OUTPUT_LOW
};

 
static int song_select = 0;
static bool is_playing = false;
static int poll_rate = 0;
static int duration_ms = 0;


pwm_t pwm;

    

void buzzer_init(int pacer_rate){
    mcu_jtag_disable();
    pwm = pwm_init (&pwm_cfg);
    poll_rate = pacer_rate;
}



long get_tempo(int *tempo_array, int size, int index){

}



void buzzer_music_select(int music){
    song_select = music;
}




void buzzer_music_play(void){
    pwm_start(pwm);
    is_playing = true;
}

void buzzer_music_pause(void){
    pwm_stop(pwm);
    is_playing = false;
}

void buzzer_music_stop(struct Music *music){
    pwm_stop(pwm);
    is_playing = false;
    music->pointer = 0;
}

void buzzer_music_restart(struct Music *music){
    music->pointer = 0;
}

void buzzer_beep(uint16_t ms){

}


void play_tone(struct Music* music){


    // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
    // there are two values per note (pitch and duration), so for each note there are four bytes
    int notes = music->notes;

    // this calculates the duration of a whole note in ms
    int wholenote = (60000 * 4) / music->tempo;

    int divider = 0;

    if (music->pointer < notes * 2) {
        // calculates the duration of each note
        divider = music->melody[music->pointer + 1];
        if (divider > 0) {
        // regular note, just proceed
        duration_ms = (wholenote) / divider;
        } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        duration_ms = (wholenote) / abs(divider);
        duration_ms *= 1.5; // increases the duration in half for dotted notes
        }

        pwm_frequency_set(pwm, music->melody[music->pointer]);
        // we only play the note for 90% of the duration, leaving 10% as a pause
        //tone(buzzer, melody[music.pointer], noteDuration * 0.9);

        // Wait for the specief duration before playing the next note.
        //delay(noteDuration);

        // stop the waveform generation before the next note.
        //noTone(buzzer);
        music->pointer = music->pointer + 2;
    }

}


void buzzer_update(void){
    static int tick = 0;

    if (tick >= poll_rate / (1000/(float) duration_ms * 2))
    {
        if(is_playing){
            switch(song_select){
                case MUSIC_CANNON:
                    play_tone(&cannon);
                    break;
                case MUSIC_NOKIA:
                    play_tone(&nokia);
                    break;
                default:
                    break;
            }
        }
        tick = 0;
    }
    tick++;
}