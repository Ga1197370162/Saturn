#ifndef __SAT_ERROR_H__
#define __SAT_ERROR_H__

#include "sat_types.h"

BEGIN_C_DECLS

typedef enum _SAT_ERROR_NO {
  RET_OK,
  RET_FAIL,
  RET_BAD_PARAM,
  RET_OOM,
  RET_NOT_FOUND,
} SAT_ERROR_NO;

END_C_DECLS

#endif /*!__SAT_ERROR_H__*/