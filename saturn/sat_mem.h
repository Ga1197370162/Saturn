#ifndef __SAT_MEM_H__
#define __SAT_MEM_H__

#include "sat_types.h"

BEGIN_C_DECLS

#ifndef SAT_MEM_MALLOC
#define SAT_MEM_MALLOC(size) sat_malloc(size, __FUNCTION__, __LINE__);
#endif /*!SAT_MEM_MALLOC*/

#ifndef SAT_MEM_CALLOC
#define SAT_MEM_CALLOC(size) sat_calloc(size, __FUNCTION__, __LINE__);
#endif /*!SAT_MEM_CALLOC*/

#ifndef SAT_MEM_ZALLOC
#define SAT_MEM_ZALLOC(type) (type*)sat_calloc(sizeof(type), __FUNCTION__, __LINE__);
#endif /*!SAT_MEM_ZALLOC*/

#ifndef SAT_MEM_ZALLOCN
#define SAT_MEM_ZALLOCN(type, numb) \
  (type*)sat_calloc(sizeof(type) * (numb), __FUNCTION__, __LINE__);
#endif /*!SAT_MEM_ZALLOCN*/

#ifndef SAT_MEM_REALLOC
#define SAT_MEM_REALLOC(ptr, size) sat_realloc(ptr, size, __FUNCTION__, __LINE__);
#endif /*!SAT_MEM_REALLOC*/

#ifndef SAT_MEM_REALLOCT
#define SAT_MEM_REALLOCT(ptr, type, numb) \
  (type*)sat_realloc(ptr, sizeof(type) * (numb), __FUNCTION__, __LINE__);
#endif /*!SAT_MEM_REALLOCT*/

#ifndef SAT_MEM_FREE
#define SAT_MEM_FREE(ptr) \
  {                       \
    if (ptr != NULL) {    \
      sat_free(ptr);      \
      ptr = NULL;         \
    }                     \
  }
#endif /*!SAT_MEM_FREE*/

inline void* sat_malloc(size_t size, const char* function, uint32_t line);
inline void* sat_calloc(size_t size, const char* function, uint32_t line);
inline void* sat_realloc(void* ptr, size_t size, const char* function, uint32_t line);
inline void sat_free(void* ptr);

typedef ret_t (*out_of_memory_hook_t)(size_t size, const char* function, uint32_t line);
out_of_memory_hook_t sat_set_out_of_memory(out_of_memory_hook_t new_hook);

END_C_DECLS

#endif /*__SAT_MEM_H__*/