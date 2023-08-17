#ifndef __SAT_THREAD_H__
#define __SAT_THREAD_H__

#include <pthread.h>
#include "sat_types.h"

BEGIN_C_DECLS

/**
 * sat_thread_t：
 *  创建实际线程所需要的信息对象, 它不直接创建线程, 而是通过 sat_thread_data_t 去创建。
 *
 * create:
 *  sat_thread_t* thread = sat_thread_create(loop_func, ctx);
 *  sat_thread_set_name(thread, "saturn_loop");
 *  sat_thread_set_stack_size(thread, 1024);
 *  sat_thread_start(thread);
 *  sat_thread_join(thread, TRUE);
 *  sat_thread_destory(thread); // 该操作不影响线程, 只是销毁信息数据
 */

typedef void (*sat_thread_entry)(void* args);

struct _sat_thread_t;
typedef struct _sat_thread_t sat_thread_t;

sat_thread_t* sat_thread_create(sat_thread_entry entry, void* args);
ret_t sat_thread_destory(sat_thread_t* thread);

void* sat_thread_get_args(sat_thread_t* thread);
bool_t sat_thread_is_running(sat_thread_t* thread);
const char* sat_thread_get_name(sat_thread_t* thread);
ret_t sat_thread_set_name(sat_thread_t* thread, const char* name);
ret_t sat_thread_set_stack_size(sat_thread_t* thread, uint32_t stack_size);

ret_t sat_thread_start(sat_thread_t* thread);
int32_t sat_thread_join(sat_thread_t* thread, bool_t joinable);

END_C_DECLS

#endif /*!__SAT_THREAD_H__*/