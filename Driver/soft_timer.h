
#ifndef __SOFT_TIMER_H__
#define __SOFT_TIMER_H__

struct soft_timer{
	int flag;											//run flag, which can be SUSPEND,RUN,STOP
	unsigned int tick_count;			//left ticks to be run
	void (*time_over_proc)(void);	//timer over callback function
	struct soft_timer* next;
};
typedef struct soft_timer*  pTimer;

#define TIMER_FLAG_SUSPEND		0
#define TIMER_FLAG_RUN			1
#define TIMER_FLAG_STOP			2
#define TIMER_NULL		((struct soft_timer*)0) 

void soft_timer_list_reset(void);
void add_timer(pTimer timer,void(*call_back)(void),unsigned int time_count);
void remove_timer(pTimer timer);
void start_timer(pTimer timer);
void stop_timer(pTimer timer);
void stop_timer_with_call(pTimer timer);
void suspend_timer(pTimer timer);
void reload_timer(pTimer timer,unsigned int time_count);
void timer_periodic_refresh(void);

#endif /*__SOFT_TIMER_H__*/
