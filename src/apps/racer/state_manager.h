#ifndef __STATE_MANAGER__H
#define __STATE_MANAGER__H

#define TIME_IN_APE_MODE 5

enum states {NORMAL, APE};

void state_manager_init(void);
void state_manage(void);
void go_ape_mode(void);

#endif

