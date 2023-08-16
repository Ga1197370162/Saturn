#include "sat_mutex.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib, "pthreadVC2.lib")
#endif

struct _sat_mutex_t {
  pthread_mutex_t mutex;

#ifdef FACK_HOLDER_CHECK
  int32_t holder;
#endif
};

static ret_t sat_mutex_holder_set(sat_mutex_t* mutex, int32_t holder) {
#ifdef FACK_HOLDER_CHECK
  return_value_if_fail(mutex != NULL && holder > 0, RET_BAD_PARAM);
  mutex->holder = holder;
#endif
  return RET_OK;
}

static ret_t sat_mutex_holder_reset(sat_mutex_t* mutex) {
#ifdef FACK_HOLDER_CHECK
  return_value_if_fail(mutex != NULL && mutex->holder > 0, RET_BAD_PARAM);
  mutex->holder = 0;
#endif
  return RET_OK;
}

sat_mutex_t* sat_mutex_create(void) {
  pthread_mutexattr_t attr;
  sat_mutex_t* mutex = SAT_MEM_ZALLOC(sat_mutex_t);
  return_value_if_fail(mutex != NULL, NULL);

  goto_error_if_fail(pthread_mutexattr_init(&attr) == 0);
  goto_error_if_fail(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_TIMED_NP) == 0);
  goto_error_if_fail(pthread_mutex_init(&mutex->mutex, &attr) == 0);
  goto_error_if_fail(pthread_mutexattr_destroy(&attr) == 0);

  goto_error_if_fail(sat_mutex_holder_reset(mutex) == RET_OK);
  return mutex;
error:
  pthread_mutexattr_destroy(&attr);
  if (mutex != NULL) {
    sat_mutex_destory(mutex);
  }
  return NULL;
}

ret_t sat_mutex_destory(sat_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAM);
  return_value_if_fail(sat_mutex_holder_reset(mutex) == RET_OK, RET_FAIL);

  pthread_mutex_destroy(&mutex->mutex);
  SAT_MEM_FREE(mutex);
  return RET_OK;
}

bool_t sat_mutex_lock(sat_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, FALSE);
  pthread_mutex_lock(&mutex->mutex);
  return_value_if_fail(sat_mutex_holder_set(mutex, 1) == RET_OK, FALSE);
  return TRUE;
}

bool_t sat_mutex_try_lock(sat_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, FALSE);
  pthread_mutex_trylock(&mutex->mutex);
  return_value_if_fail(sat_mutex_holder_set(mutex, 1) == RET_OK, FALSE);
  return TRUE;
}

bool_t sat_mutex_unlock(sat_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, FALSE);
  pthread_mutex_unlock(&mutex->mutex);
  return_value_if_fail(sat_mutex_holder_reset(mutex) == RET_OK, FALSE);
  return TRUE;
}