#include "sat_codec.h"
#include "sat_convert.h"

static sat_conv_interface_t s_sat_conv_dummy_impl = {
    .open = sat_codec_open,
    .close = sat_codec_close,
    .convert = sat_codec_convert,
};

static sat_conv_interface_t* s_sat_conv_interface = &s_sat_conv_dummy_impl;
sat_conv_interface_t* sat_conv_interface_set_impl(sat_conv_interface_t* impl) {
  sat_conv_interface_t* old_impl = s_sat_conv_interface;
  s_sat_conv_interface = impl;
  return old_impl;
}

sat_conv_t sat_convert_open(const char* to_code, const char* from_code) {
  return s_sat_conv_interface->open(to_code, from_code);
}

ret_t sat_convert_close(sat_conv_t conv) {
  return s_sat_conv_interface->close(conv);
}

ret_t sat_convert(sat_conv_t conv, char** input, uint32_t in_len, char** output, uint32_t out_len) {
  return s_sat_conv_interface->convert(conv, input, in_len, output, out_len);
}