#ifndef __SAT_THREAD_H__
#define __SAT_THREAD_H__

#include <pthread.h>
#include "sat_types.h"

BEGIN_C_DECLS

typedef void (*sat_thread_entry)(void* args);

struct _sat_thread_t;
typedef struct _sat_thread_t sat_thread_t;

sat_thread_t* sat_thread_create(sat_thread_entry entry, void* args);
void sat_thread_destory(sat_thread_t* thread);

void* sat_thread_get_args(sat_thread_t* thread);
bool_t sat_thread_is_running(sat_thread_t* thread);
const char* sat_thread_get_name(sat_thread_t* thread);
void sat_thread_set_name(sat_thread_t* thread, const char* name);
void sat_thread_set_stack_size(sat_thread_t* thread, uint32_t stack_size);

void sat_thread_start(sat_thread_t* thread);
int32_t sat_thread_join(sat_thread_t* thread, bool_t joinable);

END_C_DECLS

#endif /*!__SAT_THREAD_H__*/