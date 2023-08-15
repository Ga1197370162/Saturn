#ifndef __SAT_ERROR_H__
#define __SAT_ERROR_H__

#include "sat_types.h"

BEGIN_C_DECLS

typedef enum _SAT_ERROR_NO {
  SAT_RET_OK,
  SAT_RET_FAIL,
  SAT_RET_BAD_PARAM
} SAT_ERROR_NO;

END_C_DECLS

#endif /*!__SAT_ERROR_H__*/