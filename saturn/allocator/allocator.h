#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "allocator_defs.h"
#include "allocator_pool.h"

BEGIN_C_DECLS

inline void* sat_simple_malloc(size_t size, const char* function, uint32_t line);
inline void* sat_simple_calloc(size_t size, const char* function, uint32_t line);
inline void* sat_simple_realloc(void* ptr, size_t size, const char* function, uint32_t line);
inline void sat_simple_free(void* ptr);

/* public for allocator_pool, execute auto gc strategy */
allocator_pool_t* sat_allocator_pool_extend(uint16_t index);
ret_t sat_allocator_cleanup(void* pool);

END_C_DECLS

#endif /*!__ALLOCATOR_H__*/