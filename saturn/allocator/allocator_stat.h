#ifndef __ALLOCATOR_STAT_H__
#define __ALLOCATOR_STAT_H__

#include "allocator_defs.h"

BEGIN_C_DECLS

/**
 * @class allocator_stat_t
 * part of memory pool, for record memory chunk stat
 */
typedef struct _allocator_stat_t {
  size_t total;

  /* next stat */
  struct _allocator_stat_t* next;
} allocator_stat_t;

END_C_DECLS

#endif /*!__ALLOCATOR_STAT_H__*/