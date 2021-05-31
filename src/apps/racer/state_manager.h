#ifndef __STATE_MANAGER__H
#define __STATE_MANAGER__H

#define TIME_IN_APE_MODE 10

enum states {NORMAL, APE, HIT};

void state_manager_init(void);
void state_manage(void);

void go_normal_mode(void);
void go_ape_mode(void);
void go_hit_mode(void);

enum states get_current_state(void);

#endif

