#ifndef __SAT_COUNTDOWN_LATCH_H__
#define __SAT_COUNTDOWN_LATCH_H__

#include "sat_condition.h"

BEGIN_C_DECLS

/**
 * sat_countdown_latch_t:
 *  同步启动。底层是同步量的包装, 多增加一个计数功能, 等待指定计数减到0时, 再同时往下执行。
 *  比如等待多个线程创建完毕后同时启动执行。
 *
 * create:
 *  // thread1: 等待
 *  sat_countdown_latch_t* countdown = sat_countdown_latch_create(1);
 *  sat_thread_create_with_args(countdown);
 *  sat_countdown_latch_wait(countdown);
 *  sat_countdown_latch_destory(countdown);
 *  // thread2: 完成
 *  sat_countdown_latch_t* countdown = (sat_countdown_latch_t*)ctx;
 *  sat_countdown_latch_countdown(countdown);
 */

struct _sat_countdown_latch_t;
typedef struct _sat_countdown_latch_t sat_countdown_latch_t;

sat_countdown_latch_t* sat_countdown_latch_create(uint32_t count);
ret_t sat_countdown_latch_destory(sat_countdown_latch_t* countdown_latch);

ret_t sat_countdown_latch_wait(sat_countdown_latch_t* countdown_latch);
ret_t sat_countdown_latch_countdown(sat_countdown_latch_t* countdown_latch);
uint32_t sat_countdown_latch_get_count(sat_countdown_latch_t* countdown_latch);

END_C_DECLS

#endif /*!__SAT_COUNTDOWN_LATCH_H__*/