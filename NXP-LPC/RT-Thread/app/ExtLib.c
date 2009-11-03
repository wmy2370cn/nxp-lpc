
#include <rtthread.h>


rt_uint8_t  rt_thread_delay_hmsm (rt_uint8_t   hours, rt_uint8_t  minutes,rt_uint8_t   seconds,rt_uint16_t  ms)
{
	extern rt_err_t rt_thread_sleep (rt_tick_t tick);
	rt_uint32_t ticks; 
 
	if (hours == 0 && minutes == 0 && seconds == 0 && ms == 0) 
	{
		return (RT_ERROR);			 
	}
	if (minutes > 59) 
	{
		return (RT_ERROR);    /* Validate arguments to be within range              */
	}
	if (seconds > 59)
	{
		return (RT_ERROR);
	}
	if (ms > 999) 
	{
		return (RT_ERROR);
	}
 
	/* Compute the total number of clock ticks required.. */
	/* .. (rounded to the nearest tick)                   */
	ticks = ((rt_uint32_t)hours * 3600L + (rt_uint32_t)minutes * 60L + (rt_uint32_t)seconds) * RT_TICK_PER_SECOND
		+ RT_TICK_PER_SECOND * ((rt_uint32_t)ms + 500L / RT_TICK_PER_SECOND) / 1000L;
	rt_thread_sleep(ticks);
	return (RT_EOK);
}
