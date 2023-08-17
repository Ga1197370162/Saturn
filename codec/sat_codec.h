#ifndef __SAT_CODEC_H__
#define __SAT_CODEC_H__

#include "saturn/saturn.h"

BEGIN_C_DECLS

#define SAT_CODEC_MD5 "md5"
#define SAT_CODEC_BASE64 "base64"

#define SAT_CODEC_CREATOR(PROTOCOL) (sat_codec_interface_t*)sat_codec_##PROTOCOL_get_interface()

typedef ret_t (*sat_codec_encode_ex_t)(const char* src, uint32_t length, sat_str_t* output);
typedef ret_t (*sat_codec_decode_ex_t)(const char* src, uint32_t length, sat_str_t* output);

typedef struct _sat_codec_interface_t {
  sat_codec_encode_ex_t encode;
  sat_codec_decode_ex_t decode;
} sat_codec_interface_t;

ret_t sat_codec_str_to_bin(const char* src, uint32_t length, sat_str_t* output);
ret_t sat_codec_bin_to_str(const char* src, uint32_t length, sat_str_t* output);

END_C_DECLS

#endif /*!__SAT_CODEC_H__*/