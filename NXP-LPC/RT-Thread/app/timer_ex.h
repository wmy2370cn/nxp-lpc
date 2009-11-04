
#pragma  once
 
#define TIMER_THREAD_STACK_SIZE	   512
#define  TIMER_EX_THREAD_PRIO       8
#define RT_TIMER_EX_TICKS_PER_SEC   10    /*     Rate at which timer management task runs (Hz)            */


void  rt_timer_ex_tick_hook (void);

void rt_system_timer_ex_init(void);
void rt_timer_ex_init(rt_timer_t timer,
				   const char* name,
				   void (*timeout)(void* parameter), void* parameter,
				   rt_tick_t time, rt_uint8_t flag);
rt_err_t rt_timer_ex_detach(rt_timer_t timer);
rt_timer_t rt_timer_ex_create(const char* name,
						   void (*timeout)(void* parameter), void* parameter,
						   rt_tick_t time, rt_uint8_t flag);
rt_err_t rt_timer_ex_delete(rt_timer_t timer);
rt_err_t rt_timer_ex_start(rt_timer_t timer);
rt_err_t rt_timer_ex_stop(rt_timer_t timer);
rt_err_t rt_timer_ex_control(rt_timer_t timer, rt_uint8_t cmd, void* arg);
