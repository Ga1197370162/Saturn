#ifndef __SAT_MUTEX_H__
#define __SAT_MUTEX_H__

#include "sat_thread.h"

BEGIN_C_DECLS

/**
 * sat_mutex_t:
 *  互斥锁, 基于pthread进行简单包装。
 *  注：面向对象的思维来看可以说是继承, 能够直接强转成 pthread_mutex_t。
 *
 * create:
 *  sat_mutex_t* mutex = sat_mutex_create();
 *  sat_mutex_lock(mutex);
 *  sat_mutex_unlock(mutex);
 *  sat_mutex_destory(mutex);
 */

struct _sat_mutex_t;
typedef struct _sat_mutex_t sat_mutex_t;

sat_mutex_t* sat_mutex_create(void);
ret_t sat_mutex_destory(sat_mutex_t* mutex);

bool_t sat_mutex_lock(sat_mutex_t* mutex);
bool_t sat_mutex_try_lock(sat_mutex_t* mutex);
bool_t sat_mutex_unlock(sat_mutex_t* mutex);

END_C_DECLS

#endif /*!__SAT_MUTEX_H__*/