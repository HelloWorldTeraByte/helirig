#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdbool.h>

enum{MUSIC_CANNON, MUSIC_NOKIA, MUSIC_MARIO, MUSIC_STARWAR, MUSIC_LION,MUSIC_DOOM, MUSIC_NUM_MAX};

void buzzer_init(int pacer_rate);
void buzzer_update(void);

void buzzer_music_play(int music);
void buzzer_music_pause(void);


void buzzer_toggle(void);
void buzzer_beep(int ms);


bool buzzer_is_playing(void);




#endif /*BUZZER_H_*/