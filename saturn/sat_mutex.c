#include <pthread.h>
#include "sat_mutex.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib, "pthreadVC2.lib")
#endif

struct _sat_mutex_t {
  int32_t holder;
  pthread_mutex_t mutex;
};

sat_mutex_t* sat_mutex_create(void) {
  pthread_mutexattr_t attr;
  sat_mutex_t* mutex = SAT_MEM_ZALLOC(sat_mutex_t);
  goto_error_if_fail(mutex != NULL);

  goto_error_if_fail(pthread_mutexattr_init(&attr) == 0);
  goto_error_if_fail(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_TIMED_NP) == 0);
  goto_error_if_fail(pthread_mutex_init(&mutex->mutex, &attr) == 0);
  goto_error_if_fail(pthread_mutexattr_destroy(&attr) == 0);

  mutex->holder = 0;
  return mutex;
error:
  pthread_mutexattr_destroy(&attr);
  sat_mutex_destory(mutex);
  return NULL;
}

ret_t sat_mutex_destory(sat_mutex_t* mutex) {
  if (mutex != NULL) {
    return_value_if_fail(mutex->holder == 0, RET_FAIL);
    pthread_mutex_destroy(&mutex->mutex);
    SAT_MEM_FREE(mutex);
  }
  return RET_OK;
}

bool_t sat_mutex_lock(sat_mutex_t* mutex, int32_t holder) {
  return_value_if_fail(mutex != NULL, FALSE);
  pthread_mutex_lock(&mutex->mutex);
  mutex->holder = holder;
  return TRUE;
}

bool_t sat_mutex_try_lock(sat_mutex_t* mutex, int32_t holder) {
  return_value_if_fail(mutex != NULL, FALSE);
  pthread_mutex_trylock(&mutex->mutex);
  mutex->holder = holder;
  return TRUE;
}

bool_t sat_mutex_unlock(sat_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, FALSE);
  pthread_mutex_unlock(&mutex->mutex);
  mutex->holder = 0;
  return TRUE;
}