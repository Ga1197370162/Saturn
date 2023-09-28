#ifndef __SAT_MEM_H__
#define __SAT_MEM_H__

#include "sat_types.h"

BEGIN_C_DECLS

#ifndef SAT_MEM_MALLOC
#define SAT_MEM_MALLOC(size) sat_malloc(size, __FUNTION__, __LINE__);
#endif

#ifndef SAT_MEM_CALLOC
#define SAT_MEM_CALLOC(size) sat_calloc(size, __FUNTION__, __LINE__);
#endif

#ifndef SAT_MEM_ZALLOC
#define SAT_MEM_ZALLOC(type) (type*)sat_calloc(sizeof(type), __FUNCTION__, __LINE__);
#endif

#ifndef SAT_MEM_ZALLOCN
#define SAT_MEM_ZALLOCN(type, numb) (type*)sat_calloc(sizeof(type) * (numb), __FUNTION__, __LINE__);
#endif

#ifndef SAT_MEM_REALLOC
#define SAT_MEM_REALLOC(ptr, size) sat_realloc(ptr, size, __FUNTION__, __LINE__);
#endif

#ifndef SAT_MEM_REALLOCT
#define SAT_MEM_REALLOCT(ptr, type, numb) \
  (type*)sat_realloc(ptr, sizeof(type) * (numb), __FUNTION__, __LINE__);
#endif

#ifndef SAT_MEM_FREE
#define SAT_MEM_FREE(ptr) \
  {                       \
    if (ptr != NULL) {    \
      sat_free(ptr);      \
      ptr = NULL;         \
    }                     \
  }

inline void* sat_malloc(size_t size, const char* funtion, uint32_t line);
inline void* sat_calloc(size_t size, const char* funtion, uint32_t line);
inline void* sat_realloc(void* ptr, size_t size, const char* funtion, uint32_t line);
inline void sat_free(void* ptr);

END_C_DECLS

#endif /*__SAT_MEM_H__*/