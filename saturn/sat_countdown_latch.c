#include "sat_countdown_latch.h"

struct _sat_countdown_latch_t {
  uint32_t count;
  sat_condition_t* condition;
};

sat_countdown_latch_t* sat_countdown_latch_create(uint32_t count) {
  sat_countdown_latch_t* countdown_latch = SAT_MEM_ZALLOC(sat_countdown_latch_t);
  goto_error_if_fail(countdown_latch != NULL);

  countdown_latch->count = count;
  countdown_latch->condition = sat_condition_create();
  goto_error_if_fail(countdown_latch->condition != NULL);

  return countdown_latch;
error:
  if (countdown_latch != NULL) {
    sat_countdown_latch_destory(countdown_latch);
  }
  return NULL;
}

ret_t sat_countdown_latch_destory(sat_countdown_latch_t* countdown_latch) {
  return_value_if_fail(countdown_latch != NULL, RET_BAD_PARAM);

  if (countdown_latch->condition != NULL) {
    sat_condition_destory(countdown_latch->condition);
  }
  countdown_latch->condition = NULL;

  SAT_MEM_FREE(countdown_latch);
  return RET_OK;
}

ret_t sat_countdown_latch_wait(sat_countdown_latch_t* countdown_latch) {
  return_value_if_fail(countdown_latch != NULL && countdown_latch->condition != NULL,
                       RET_BAD_PARAM);
  sat_condition_lock(countdown_latch->condition);
  while (countdown_latch->count > 0) {
    sat_condition_wait(countdown_latch->condition);
  }
  sat_condition_unlock(countdown_latch->condition);
  return RET_OK;
}

ret_t sat_countdown_latch_countdown(sat_countdown_latch_t* countdown_latch) {
  return_value_if_fail(countdown_latch != NULL && countdown_latch->condition != NULL,
                       RET_BAD_PARAM);
  sat_condition_lock(countdown_latch->condition);
  countdown_latch->count -= 1;
  if (countdown_latch->count <= 0) {
    sat_condition_notify_all(countdown_latch->condition);
  }
  sat_condition_unlock(countdown_latch->condition);
  return RET_OK;
}

uint32_t sat_countdown_latch_get_count(sat_countdown_latch_t* countdown_latch) {
  uint32_t count = 0;
  return_value_if_fail(countdown_latch != NULL && countdown_latch->condition != NULL, -1);
  sat_condition_lock(countdown_latch->condition);
  count = countdown_latch->count;
  sat_condition_unlock(countdown_latch->condition);
  return count;
}