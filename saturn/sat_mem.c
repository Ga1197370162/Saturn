#include "sat_mem.h"

static void* sat_system_malloc(size_t size, const char* function, int32_t line) {
  UNUSE(function);
  UNUSE(line);
  return malloc(size);
}

static void* sat_system_calloc(size_t nmemb, size_t size, const char* function, int32_t line) {
  UNUSE(function);
  UNUSE(line);
  return calloc(nmemb, size);
}

static void* sat_system_realloc(void* ptr, size_t size, const char* function, int32_t line) {
  UNUSE(function);
  UNUSE(line);
  return realloc(ptr, size);
}

static void sat_system_free(void* ptr) {
  free(ptr);
}

static sat_mem_vtable_t default_mem_vtable = {
  .malloc = sat_system_malloc,
  .calloc = sat_system_calloc,
  .realloc = sat_system_realloc,
  .free = sat_system_free,
};

static sat_mem_vtable_t* sat_mem_profiler_table = &default_mem_vtable;
ret_t sat_mem_set_vtable(sat_mem_vtable_t* mem_table) {
  sat_mem_profiler_table = mem_table;
}

static sat_mem_out_of_memory_t oom_callback = NULL;
sat_mem_out_of_memory_t sat_mem_set_oom_callback(sat_mem_out_of_memory_t new_oom_callback) {
  sat_mem_out_of_memory_t old_oom_callback = oom_callback;
  oom_callback = new_oom_callback;
  return old_oom_callback;
}

void* sat_malloc(size_t size, const char* function, int32_t line) {
  void* mem = NULL;

  if (sat_mem_profiler_table != NULL) {
    mem = sat_mem_profiler_table->malloc(size, function, line);
    if (mem == NULL && oom_callback) {
      oom_callback(size);
    }
  }
  return mem;
}

void* sat_calloc(size_t nmemb, size_t size, const char* function, int32_t line) {
  void* mem = NULL;

  if (sat_mem_profiler_table != NULL) {
    mem = sat_mem_profiler_table->calloc(nmemb, size, function, line);
    if (mem == NULL && oom_callback) {
      oom_callback(size);
    }
  }
  return mem;
}

void* sat_ralloc(void* ptr, size_t size, const char* function, int32_t line) {
  void* mem = NULL;

  if (sat_mem_profiler_table != NULL) {
    mem = sat_mem_profiler_table->realloc(ptr, size, function, line);
    if (mem == NULL && oom_callback) {
      oom_callback(size);
    }
  }
  return mem;
}

void sat_free(void* ptr) {
  if (sat_mem_profiler_table != NULL) {
    sat_mem_profiler_table->free(ptr);
  }
  return;
}