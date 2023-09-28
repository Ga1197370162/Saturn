#include "sat_mem.h"

static inline void* sat_dummy_malloc(size_t size, const char* funtion, uint32_t line) {
  UNUSED(funtion);
  UNUSED(line);
  return malloc(size);
}

static inline void* sat_dummy_calloc(size_t size, const char* funtion, uint32_t line) {
  UNUSED(funtion);
  UNUSED(line);
  return calloc(size);
}

static inline void* sat_dummy_realloc(void* ptr, size_t size, const char* funtion, uint32_t line) {
  UNUSED(funtion);
  UNUSED(line);
  return realloc()
}

static inline void sat_dummy_free(void* ptr) {
}

#ifdef MALLOC_PREFIX
#define malloc GLUE(MALLOC_PREFIX, malloc)
#define calloc GLUE(MALLOC_PREFIX, calloc)
#define realloc GLUE(MALLOC_PREFIX, realloc)
#define free GLUE(MALLOC_PREFIX, free)
#else
#define malloc sat_dummy_malloc

#endif /*!MALLOC_PREFIX*/

void* sat_malloc(size_t size, const char* funtion, uint32_t line) {
}

void* sat_calloc(size_t size, const char* funtion, uint32_t line) {
}

void* sat_realloc(void* ptr, size_t size, const char* funtion, uint32_t line) {
}

void sat_free(void* ptr) {
}