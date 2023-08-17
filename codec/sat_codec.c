#include "sat_codec.h"

#include "sat_codec_md5.inc"
#include "sat_codec_base64.inc"

ret_t sat_codec_str_to_bin(const char* src, uint32_t length, sat_str_t* output) {
  int i;
  return_value_if_fail(src != NULL && output != NULL, RET_BAD_PARAM);

  for (i = 0; i < length; ++i) {
    uint8_t j = 0;
    uint8_t bin_temp[8] = {0};
    uint8_t data = (uint8_t)(src[i]);

    for (j = 0; j < 8; ++j) {
      bin_temp[j] = ((data % 2) + '0');
      data >>= 1;
    }

    for (j = 0; j < 8; ++j) {
      *output = sat_str_append_char(*output, (const char)bin_temp[7 - j]);
    }
  }

  return RET_OK;
}

ret_t sat_codec_bin_to_str(const char* src, uint32_t length, sat_str_t* output) {
  int i, j;
  uint8_t data = 0;
  return_value_if_fail(src != NULL && output != NULL, RET_BAD_PARAM);

  for (i = 0, j = 0; i < length; ++i) {
    data <<= 1;
    data += (src[i] - '0');

    j += 1;
    if (j == 8) {
      *output = sat_str_append_char(*output, data);

      j = 0;
      data = 0;
    }
  }

  return RET_OK;
}