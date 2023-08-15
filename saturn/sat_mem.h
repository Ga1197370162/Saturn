#ifndef __SAT_MEM_H__
#define __SAT_MEM_H__

#include "sat_types.h"

BEGIN_C_DECLS

#ifndef SAT_MEM_MALLOC
#define SAT_MEM_MALLOC(size) sat_malloc(size, __FUNCTION__, __LINE__)
#endif /*!SAT_MEM_MALLOC*/

#ifndef SAT_MEM_CALLOC
#define SAT_MEM_CALLOC(size) sat_calloc(1, size, __FUNCTION__, __LINE__)
#endif /*!SAT_MEM_CALLOC*/

#ifndef SAT_MEM_ZALLOC
#define SAT_MEM_ZALLOC(type) (type*)sat_calloc(1, sizeof(type), __FUNCTION__, __LINE__)
#endif /*!SAT_MEM_ZALLOC*/

#ifndef SAT_MEM_REALLOC
#define SAT_MEM_REALLOC(ptr, size) sat_ralloc(ptr, size, __FUNCTION__, __LINE__)
#endif /*SAT_MEM_REALLOC*/

#ifndef SAT_MEM_REALLOCT
#define SAT_MEM_REALLOCT(type, ptr, n) \
  (type*)sat_ralloc(ptr, (sizeof(type) * n), __FUNCTION__, __LINE__)
#endif /*SAT_MEM_REALLOCT*/

#ifndef SAT_MEM_FREE
#define SAT_MEM_FREE(ptr) \
  {                       \
    sat_free(ptr);        \
    ptr = NULL;           \
  }
#endif /*!SAT_MEM_FREE*/

void* sat_malloc(size_t size, const char* function, int32_t line);
void* sat_calloc(size_t nmemb, size_t size, const char* function, int32_t line);
void* sat_ralloc(void* ptr, size_t size, const char* function, int32_t line);
void sat_free(void* ptr);

typedef void (*sat_mem_out_of_memory_t)(size_t size);
typedef void* (*sat_malloc_t)(size_t size, const char* function, int32_t line);
typedef void* (*sat_calloc_t)(size_t nmemb, size_t size, const char* function, int32_t line);
  typedef void* (*sat_realoc_t)(void* ptr, size_t size, const char* funtion, int32_t line);
  typedef void (*sat_free_t)(void* ptr);

typedef struct _sat_mem_vtable_t {
  sat_malloc_t malloc;
  sat_calloc_t calloc;
  sat_realoc_t realloc;
  sat_free_t free;
} sat_mem_vtable_t;

ret_t sat_mem_set_vtable(sat_mem_vtable_t* vtable);
sat_mem_out_of_memory_t sat_mem_set_oom_callback(sat_mem_out_of_memory_t new_oom_callback);

END_C_DECLS

#endif /*!__SAT_MEM_H__*/