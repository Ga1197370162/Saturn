#ifndef __SAT_CONDITION_H__
#define __SAT_CONDITION_H__

#include "sat_mutex.h"

BEGIN_C_DECLS

/**
 * sat_condition_t:
 *  同步信号量, pthread_cond_t 的简单封装, 与 mutex 类似。
 *
 * create:
 *  sat_condition_t* cond = sat_condition_create();
 *  // thread1: 等待
 *  sat_condition_wait(cond);
 *  // thread2: 唤醒thread1
 *  sat_condition_notify_all(cond);
 *  sat_condition_destory(cond);
 */

struct _sat_condition_t;
typedef struct _sat_condition_t sat_condition_t;

sat_condition_t* sat_condition_create(void);
ret_t sat_condition_destory(sat_condition_t* condition);

bool_t sat_condition_lock(sat_condition_t* condition);
bool_t sat_condition_unlock(sat_condition_t* condition);

ret_t sat_condition_wait(sat_condition_t* condition);
ret_t sat_condition_notify(sat_condition_t* condition);
ret_t sat_condition_notify_all(sat_condition_t* condition);

END_C_DECLS

#endif /*!__SAT_CONDITION_H__*/