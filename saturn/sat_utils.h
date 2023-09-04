#ifndef __SAT_UTILS_H__
#define __SAT_UTILS_H__

#include "sat_types.h"

BEGIN_C_DECLS

ret_t sat_dummpy_destory(void* ctx);
int32_t sat_default_compare(const void* ctx, const void* data);
void* sat_pointer_from_int(int32_t v);
int32_t sat_pointer_to_int(void* p);

END_C_DECLS

#endif /*!__SAT_UTILS_H__*/