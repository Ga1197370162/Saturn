#ifndef __SAT_CODEC_H__
#define __SAT_CODEC_H__

#include "sat_convert.h"

BEGIN_C_DECLS

typedef sat_conv_t (*sat_codec_get_conv_instance_t)(void);

typedef enum _SAT_CODEC_COMPAT {
  SAT_CODEC_COMPAT_IN = 0x0001,
  SAT_CODEC_COMPAT_OUT = 0x0002,
} SAT_CODEC_COMPAT;

typedef struct _sat_codec_register_t {
  const char* protocol;
  sat_codec_get_conv_instance_t get_instance;
} sat_codec_register_t;

static const sat_codec_register_t g_sat_codec_table[] = {
#define SAT_CODEC_REGISTER(PROTOCOL, GET_INSTANCE) { PROTOCOL, GET_INSTANCE },
#include "sat_codec_defs.h"
#undef SAT_CODEC_REGISTER
};

sat_conv_t sat_codec_open(const char* in_code, const char* out_code);
ret_t sat_codec_close(sat_conv_t conv);

ret_t sat_codec_convert(sat_conv_t conv, char** input, uint32_t in_len, char** output, uint32_t out_len);

END_C_DECLS

#endif /*!__SAT_CODEC_H__*/