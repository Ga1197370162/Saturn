#include "sat_utils.h"

ret_t sat_dummpy_destory(void* ctx) {
  UNUSE(ctx);
  return RET_OK;
}

int32_t sat_default_compare(const void* ctx, const void* data) {
  return (int32_t)((char*)ctx - (char*)data);
}

void* sat_pointer_from_int(int32_t v) {
  return (char*)NULL + v;
}

int32_t sat_pointer_to_int(void* p) {
  return (char*)p - (char*)NULL;
}