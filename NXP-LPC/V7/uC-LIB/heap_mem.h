
#pragma once

#define RT_USING_HEAP

#ifdef RT_USING_HEAP
/*
* heap memory interface
*/
void rt_system_heap_init(void* begin_addr, void* end_addr);

void* rt_malloc(INT32U nbytes);
void rt_free (void *ptr);
void* rt_realloc(void *ptr, INT32U nbytes);

#ifdef RT_USING_HOOK
void rt_malloc_sethook(void (*hook)(void *ptr, INT32U size));
void rt_free_sethook(void (*hook)(void *ptr));
#endif
#endif
