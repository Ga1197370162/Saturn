#ifndef __SAT_COUNTDOWN_LATCH_H__
#define __SAT_COUNTDOWN_LATCH_H__

#include "sat_condition.h"

BEGIN_C_DECLS

struct _sat_countdown_latch_t;
typedef struct _sat_countdown_latch_t sat_countdown_latch_t;

sat_countdown_latch_t* sat_countdown_latch_create(uint32_t count);
ret_t sat_countdown_latch_destory(sat_countdown_latch_t* countdown_latch);

ret_t sat_countdown_latch_wait(sat_countdown_latch_t* countdown_latch);
ret_t sat_countdown_latch_countdown(sat_countdown_latch_t* countdown_latch);
uint32_t sat_countdown_latch_get_count(sat_countdown_latch_t* countdown_latch);


END_C_DECLS

#endif /*!__SAT_COUNTDOWN_LATCH_H__*/