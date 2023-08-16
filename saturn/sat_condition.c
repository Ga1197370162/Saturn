#include "sat_condition.h"

struct _sat_condition_t {
  pthread_cond_t cond;
  sat_mutex_t* mutex;
};

sat_condition_t* sat_condition_create(void) {
  sat_condition_t* condition = SAT_MEM_ZALLOC(sat_condition_t);
  goto_error_if_fail(condition != NULL);

  condition->mutex = sat_mutex_create();
  goto_error_if_fail(pthread_cond_init(&condition->cond, NULL) == 0);

  return condition;
error:
  if (condition != NULL) {
    sat_condition_destory(condition);
  }
  return NULL;
}

ret_t sat_condition_destory(sat_condition_t* condition) {
  return_value_if_fail(condition != NULL, RET_BAD_PARAM);

  if (condition->mutex != NULL) {
    sat_mutex_destory(condition->mutex);
  }
  condition->mutex = NULL;

  pthread_cond_destroy(&condition->cond);
  SAT_MEM_FREE(condition);
  return RET_OK;
}

bool_t sat_condition_lock(sat_condition_t* condition) {
  return_value_if_fail(condition != NULL && condition->mutex != NULL, FALSE);
  return sat_mutex_lock(condition->mutex);
}

bool_t sat_condition_unlock(sat_condition_t* condition) {
  return_value_if_fail(condition != NULL && condition->mutex != NULL, FALSE);
  return sat_mutex_unlock(condition->mutex);
}

ret_t sat_condition_wait(sat_condition_t* condition) {
  return_value_if_fail(condition != NULL && condition->mutex != NULL, RET_BAD_PARAM);
  pthread_cond_wait(&condition->cond, (pthread_mutex_t*)condition->mutex);
  return RET_OK;
}

ret_t sat_condition_notify(sat_condition_t* condition) {
  return_value_if_fail(condition != NULL, RET_BAD_PARAM);
  pthread_cond_signal(&condition->cond);
  return RET_OK;
}

ret_t sat_condition_notify_all(sat_condition_t* condition) {
  return_value_if_fail(condition != NULL, RET_BAD_PARAM);
  pthread_cond_broadcast(&condition->cond);
  return RET_OK;
}