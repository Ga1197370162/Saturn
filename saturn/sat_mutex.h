#ifndef __SAT_MUTEX_H__
#define __SAT_MUTEX_H__

#include "sat_thread.h"

BEGIN_C_DECLS

struct _sat_mutex_t;
typedef struct _sat_mutex_t sat_mutex_t;

sat_mutex_t* sat_mutex_create(void);
ret_t sat_mutex_destory(sat_mutex_t* mutex);

bool_t sat_mutex_lock(sat_mutex_t* mutex);
bool_t sat_mutex_try_lock(sat_mutex_t* mutex);
bool_t sat_mutex_unlock(sat_mutex_t* mutex);

END_C_DECLS

#endif /*!__SAT_MUTEX_H__*/