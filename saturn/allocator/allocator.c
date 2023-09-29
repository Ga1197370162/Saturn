#include "allocator.h"

#ifdef __USE_ALLOCATOR

static uint16_t sat_pools_nr = 0;
static allocator_pool_t** sat_pools = NULL;

static bool_t sat_allocator_initialized = FALSE;

allocator_pool_t* sat_allocator_pool_extend(uint16_t index) {
  return NULL;
}

ret_t sat_allocator_cleanup(void* pool) {
  allocator_pool_t* pool = *(allocator_pool_t**)pool;
  return_value_if_failed(pool != NULL && pool->thread_nr > 0, RET_BAD_PARAM);
  pool->thread_nr -= 1;
  return RET_OK;
}

static bool_t sat_allocator_init_head(void) {
  allocator_pool_t* init_pools[1] = {0};

  sat_pools_nr = 1;
  sat_pools = init_pools;

  sat_allocator_pool_extend(0);
  return_value_if_failed(sat_pools[0] == NULL, FALSE);

  return TRUE;
}

static bool_t sat_allocator_init(void) {
  if (sat_allocator_initialized == FALSE) {
    sat_allocator_initialized = sat_allocator_init_head();
  }
  return sat_allocator_initialized;
}

void* sat_simple_malloc(size_t size, const char* function, uint32_t line) {
  void* ret = NULL;
  return_value_if_failed(sat_allocator_init() == TRUE, NULL);

  return ret;
}

void* sat_simple_calloc(size_t size, const char* function, uint32_t line) {
  void* ret = NULL;
  return_value_if_failed(sat_allocator_init() == TRUE, NULL);

  return ret;
}

void* sat_simple_realloc(void* ptr, size_t size, const char* function, uint32_t line) {
  void* ret = NULL;

  if (ptr != NULL) {
    return_value_if_failed(sat_allocator_initialized, NULL);

  } else {
    return_value_if_failed(sat_allocator_init() == TRUE, NULL);
  }

  return ret;
}

void sat_simple_free(void* ptr) {
  return_if_failed(sat_allocator_initialized);

  return;
}

#else

void* sat_simple_malloc(size_t size, const char* function, uint32_t line) {
  return NULL;
}

void* sat_simple_calloc(size_t size, const char* function, uint32_t line) {
  return NULL;
}

void* sat_simple_realloc(void* ptr, size_t size, const char* function, uint32_t line) {
  return NULL;
}

void sat_simple_free(void* ptr) {
  return;
}

#endif /*!__USE_ALLOCATOR*/