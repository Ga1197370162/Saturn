#ifndef __ALLOCATOR_DEFS_H__
#define __ALLOCATOR_DEFS_H__

#include "saturn/sat_types.h"

#define __USE_ALLOCATOR

/* whether to use memory allocator */
#ifdef __USE_ALLOCATOR
#define MALLOC_PREFIX sat_simple_
#endif /*!__USE_ALLOCATOR*/



/* on malloc/calloc/realloc/free count equit this numb then exec gc strategy */
#define ALLOCATOR_GARBAGE_COLLECTION 25

/* how many chunks are needed to store */
#define ALLOCATOR_ALIGN 8
#define ALLOCATOR_MIN_CHUNK_SIZE 8
#define ALLOCATOR_MAX_CHUNK_SIZE 128
#define ALLOCATOR_CHUNK_NR 45

#endif /*!__ALLOCATOR_DEFS_H__*/