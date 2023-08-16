#include "sat_str.h"
#include "sat_thread.h"
#include "sat_countdown_latch.h"

struct _sat_thread_t {
  void* args;
  sat_str_t name;
  bool_t joinable;
  bool_t is_running;
  sat_thread_entry entry;
  pthread_t thread_handler;
  pthread_attr_t thread_attr;
};

typedef struct _sat_thread_data_t {
  void* args;
  sat_str_t name;
  sat_thread_entry entry;
  sat_countdown_latch_t* countdown_latch;
} sat_thread_data_t;

sat_thread_t* sat_thread_create(sat_thread_entry entry, void* args) {
  sat_thread_t* thread = SAT_MEM_ZALLOC(sat_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->args = args;
  thread->entry = entry;

  thread->joinable = FALSE;
  thread->is_running = FALSE;
  thread->name = sat_str_init_set("saturn_thread");

  pthread_attr_init(&thread->thread_attr);
  return thread;
}

ret_t sat_thread_destory(sat_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAM);

  if (thread->name != NULL) {
    sat_str_deinit(thread->name);
  }

  if (thread->is_running && !thread->joinable) {
    pthread_detach(thread->thread_handler);
  }

  pthread_attr_destroy(&thread->thread_attr);
  SAT_MEM_FREE(thread);
  return RET_OK;
}

void* sat_thread_get_args(sat_thread_t* thread) {
  return_value_if_fail(thread != NULL, NULL);
  return thread->args;
}

bool_t sat_thread_is_running(sat_thread_t* thread) {
  return_value_if_fail(thread != NULL, FALSE);
  return thread->is_running;
}

const char* sat_thread_get_name(sat_thread_t* thread) {
  return_value_if_fail(thread != NULL, NULL);
  return thread->name;
}

ret_t sat_thread_set_name(sat_thread_t* thread, const char* name) {
  return_value_if_fail(thread != NULL && name != NULL, RET_BAD_PARAM);
  thread->name = sat_str_cpy(thread->name, name);
  return RET_OK;
}

#ifdef _POSIX_THREAD_ATTR_STACKSIZE
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 1024
#endif
ret_t sat_thread_set_stack_size(sat_thread_t* thread, uint32_t stack_size) {
  uint32_t size = PTHREAD_STACK_MIN;
  return_value_if_fail(thread != NULL && sat_thread_is_running(thread) == TRUE, RET_BAD_PARAM);
  size = size > stack_size ? size : stack_size;
  pthread_attr_setstacksize(&thread->thread_attr, size);
  return RET_OK;
}
#else
ret_t sat_thread_set_stack_size(sat_thread_t* thread, uint32_t stack_size) {
  return RET_OK;
}
#endif

static void run_in_thread(sat_thread_data_t* thread_data) {
  uint32_t name_len = 0;
  return_if_fail(thread_data != NULL);

  sat_countdown_latch_countdown(thread_data->countdown_latch);
  thread_data->countdown_latch = NULL;

  name_len = sat_str_len(thread_data->name);
  if (name_len <= 0) {
    thread_data->name = sat_str_cpy(thread_data->name, "saturn_thread");
  }

#if defined(__linux__)
#include <sys/prctl.h>
  prctl(PR_SET_NAME, thread_data->name);
#endif

  thread_data->entry(thread_data->args);
  return;
}

static void* sat_thread_start_ex(void* ctx) {
  sat_thread_data_t* thread_data = (sat_thread_data_t*)ctx;
  return_value_if_fail(ctx != NULL && thread_data != NULL, NULL);
  run_in_thread(thread_data);
  SAT_MEM_FREE(thread_data);
  return NULL;
}

ret_t sat_thread_start(sat_thread_t* thread) {
  ret_t ret = RET_OK;
  sat_thread_data_t* thread_data = NULL;
  sat_countdown_latch_t* countdown_latch = NULL;
  return_value_if_fail(thread != NULL && !thread->is_running, RET_BAD_PARAM);

  countdown_latch = sat_countdown_latch_create(1);

  thread_data = SAT_MEM_ZALLOC(sat_thread_data_t);
  return_value_if_fail(thread_data != NULL, RET_FAIL);

  thread_data->args = thread->args;
  thread_data->name = thread->name;
  thread_data->entry = thread->entry;
  thread_data->countdown_latch = countdown_latch;

  if (pthread_create(&thread->thread_handler, &thread->thread_attr, sat_thread_start_ex,
                     thread_data)) {
    ret = RET_FAIL;
    thread->is_running = FALSE;
    SAT_MEM_FREE(thread_data);
    printf("thread create error!\n");
  } else {
    sat_countdown_latch_wait(countdown_latch);
    thread->is_running = TRUE;
  }

  sat_countdown_latch_destory(countdown_latch);
  return ret;
}

int32_t sat_thread_join(sat_thread_t* thread, bool_t joinable) {
  return_value_if_fail(thread != NULL && thread->is_running && !thread->joinable, -1);
  thread->joinable = TRUE;
  return pthread_join(thread->thread_handler, NULL);
}
