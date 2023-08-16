#include "sat_types.h"
#include "sat_utils.h"

ret_t sat_dummpy_destory(void* ctx) {
  UNUSE(ctx);
  return RET_OK;
}

int32_t sat_default_compare(const void* ctx, const void* data) {
  return (int32_t)((char*)ctx - (char*)data);
}