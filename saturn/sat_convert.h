#ifndef __SAT_CONVERT_H__
#define __SAT_CONVERT_H__

#include "sat_types.h"

BEGIN_C_DECLS

typedef void* sat_conv_t;

typedef sat_conv_t (*sat_conv_open_t)(const char* to_code, const char* from_code);
typedef ret_t (*sat_conv_close_t)(sat_conv_t conv);
typedef ret_t (*sat_conv_t)(sat_conv_t conv, char** input, uint32_t in_len, char** output,
                            uint32_t out_len);

typedef struct _sat_conv_interface_t {
  sat_conv_open_t open;
  sat_conv_close_t close;
  sat_conv_t convert;
} sat_conv_interface_t;

sat_conv_interface_t* sat_conv_interface_set_impl(sat_conv_interface_t* impl);

sat_conv_t sat_convert_open(const char* to_code, const char* from_code);
ret_t sat_convert_close(sat_conv_t conv);

ret_t sat_convert(sat_conv_t conv, char** input, uint32_t in_len, char** output, uint32_t out_len);

END_C_DECLS

#endif /*!__SAT_CONVERT_H__*/