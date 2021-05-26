#ifndef BUZZER_H_
#define BUZZER_H_

enum{MUSIC_CANNON, MUSIC_NOKIA, MUSIC_MARIO, MUSIC_STARWAR, MUSIC_NUM_MAX};

void buzzer_init(int pacer_rate);
void buzzer_beep(uint16_t ms);





#endif /*BUZZER_H_*/