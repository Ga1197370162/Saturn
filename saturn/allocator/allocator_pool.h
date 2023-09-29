#ifndef __ALLOCATOR_POOL_H__
#define __ALLOCATOR_POOL_H__

#include "allocator_defs.h"
#include "allocator_stat.h"

BEGIN_C_DECLS

/**
 * @class allocator_pool_t
 * memory pool
 */
typedef struct _allocator_pool_t {
  uint8_t index;
  uint8_t thread_nr;

  allocator_stat_t* stat;
} allocator_pool_t;

END_C_DECLS

#endif /*!__ALLOCATOR_POOL_H__*/