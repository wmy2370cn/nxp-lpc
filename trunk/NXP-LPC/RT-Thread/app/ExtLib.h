
#pragma  once
#include "rtdef.h"

rt_uint8_t  rt_thread_delay_hmsm (rt_uint8_t   hours, rt_uint8_t  minutes,rt_uint8_t   seconds,rt_uint16_t  ms);

#ifdef  _DEBUG

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed.
  *   If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((rt_uint8_t *)__FILE__, __LINE__))
  #define ASSERT(expr)  ((expr) ? (void)0 : assert_failed((rt_uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(rt_uint8_t* file, rt_uint32_t line);

#else
  #define assert_param(expr) ((void)0)
#endif 
