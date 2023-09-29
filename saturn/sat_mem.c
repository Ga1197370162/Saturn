#include "sat_mem.h"
#include "allocator/allocator.h"

static inline void* sat_dummy_malloc(size_t size, const char* function, uint32_t line) {
  UNUSED(function);
  UNUSED(line);
  return malloc(size == 0 ? 1 : size);
}

static inline void* sat_dummy_calloc(size_t size, const char* function, uint32_t line) {
  UNUSED(function);
  UNUSED(line);
  return calloc(size == 0 ? 1 : size);
}

static inline void* sat_dummy_realloc(void* ptr, size_t size, const char* function, uint32_t line) {
  UNUSED(function);
  UNUSED(line);
  return realloc(ptr, size == 0 ? 1 : size);
}

#ifdef MALLOC_PREFIX
#define malloc JOIN(MALLOC_PREFIX, malloc)
#define calloc JOIN(MALLOC_PREFIX, calloc)
#define realloc JOIN(MALLOC_PREFIX, realloc)
#define free JOIN(MALLOC_PREFIX, free)
#else
#define malloc sat_dummy_malloc
#define calloc sat_dummy_calloc
#define realloc sat_dummy_realloc
#endif /*!MALLOC_PREFIX*/

static out_of_memory_hook_t oom_hook = NULL;

out_of_memory_hook_t sat_set_out_of_memory(out_of_memory_hook_t new_hook) {
  out_of_memory_hook_t origin_oom_hook = oom_hook;
  oom_hook = new_hook;
  return origin_oom_hook;
}

static inline void* sat_out_of_memory_ex(size_t size, const char* function, uint32_t line) {
  ret_t ret = RET_OK;
  if (oom_hook != NULL) {
    ret = oom_hook(size, function, line);
  }
  return ret;
}

void* sat_malloc(size_t size, const char* function, uint32_t line) {
  void* ret = malloc(size, function, line);
  if (ret == NULL) {
    sat_out_of_memory_ex(size, function, line);
  }
  return ret;
}

void* sat_calloc(size_t size, const char* function, uint32_t line) {
  void* ret = calloc(size, function, line);
  if (ret == NULL) {
    sat_out_of_memory_ex(size, function, line);
  }
  return ret;
}

void* sat_realloc(void* ptr, size_t size, const char* function, uint32_t line) {
  void* ret = realloc(ptr, size, function, line);
  if (ret == NULL&&) {
    sat_out_of_memory_ex(size, function, line);
  }
  return ret;
}

void sat_free(void* ptr) {
  free(ptr);
  return;
}