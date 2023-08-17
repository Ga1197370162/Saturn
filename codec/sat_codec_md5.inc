#include "sat_codec.h"

#if defined(__linux__)
#include <arpa/inet.h>
#elif defined(WIN32) || defined(_MSC_VER)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#define MD5_F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))

static ret_t sat_codec_encode_md5(const char* src, uint32_t length, sat_str_t* output);

static sat_codec_interface_t md5_interface = {
    .encode = sat_codec_encode_md5,
    .decode = NULL,
};

sat_codec_interface_t* sat_codec_md5_get_interface(void) {
  return &md5_interface;
}

static const uint32_t T[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

static const uint8_t S[] = {
    7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21,
};

static uint32_t MD5_FF(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s,
                       uint32_t t) {
  a += (MD5_F(b, c, d) & 0xFFFFFFFF) + x + t;
  a = ((a & 0xFFFFFFFF) << s) | ((a & 0xFFFFFFFF) >> (32 - s));
  a += b;
  return (a & 0xFFFFFFFF);
}

static uint32_t MD5_GG(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s,
                       uint32_t t) {
  a += (MD5_G(b, c, d) & 0xFFFFFFFF) + x + t;
  a = ((a & 0xFFFFFFFF) << s) | ((a & 0xFFFFFFFF) >> (32 - s));
  a += b;
  return (a & 0xFFFFFFFF);
}

static uint32_t MD5_HH(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s,
                       uint32_t t) {
  a += (MD5_H(b, c, d) & 0xFFFFFFFF) + x + t;
  a = ((a & 0xFFFFFFFF) << s) | ((a & 0xFFFFFFFF) >> (32 - s));
  a += b;
  return (a & 0xFFFFFFFF);
}

static uint32_t MD5_II(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s,
                       uint32_t t) {
  a += (MD5_I(b, c, d) & 0xFFFFFFFF) + x + t;
  a = ((a & 0xFFFFFFFF) << s) | ((a & 0xFFFFFFFF) >> (32 - s));
  a += b;
  return (a & 0xFFFFFFFF);
}

static uint32_t* sat_codec_md5_init_fix(const char* src, uint32_t length, uint32_t* bin_length) {
  uint32_t i = 0;
  uint32_t byte_num = ((length + 8) / 64) + 1;
  uint32_t* str_byte = SAT_MEM_ZALLOCN(uint32_t, byte_num * 16);
  return_value_if_fail(src != NULL && bin_length != NULL && str_byte != NULL, NULL);

  *bin_length = byte_num * 16;
  for (i = 0; i < length; ++i) {
    str_byte[i >> 2] |= (src[i]) << ((i % 4) * 8);
  }
  str_byte[length >> 2] |= 0x80 << (((length % 4)) * 8);
  str_byte[byte_num * 16 - 2] = length * 8;

  return str_byte;
}

ret_t sat_codec_encode_md5(const char* src, uint32_t length, sat_str_t* output) {
  int i = 0;
  uint32_t byte_length = 0;
  uint32_t* str_byte = NULL;
  uint32_t md5_hash_factor[] = {
      0x67452301,
      0xEFCDAB89,
      0x98BADCFE,
      0x10325476,
  };
  return_value_if_fail(src != NULL && output != NULL, RET_BAD_PARAM);

  str_byte = sat_codec_md5_init_fix(src, length, &byte_length);
  return_value_if_fail(str_byte != NULL, RET_OOM);

  for (i = 0; i < (byte_length / 16); ++i) {
    uint32_t* x = &str_byte[i * 16];
    uint32_t a = md5_hash_factor[0];
    uint32_t b = md5_hash_factor[1];
    uint32_t c = md5_hash_factor[2];
    uint32_t d = md5_hash_factor[3];

    a = MD5_FF(a, b, c, d, x[0], S[0], T[0]);
    d = MD5_FF(d, a, b, c, x[1], S[1], T[1]);
    c = MD5_FF(c, d, a, b, x[2], S[2], T[2]);
    b = MD5_FF(b, c, d, a, x[3], S[3], T[3]);
    a = MD5_FF(a, b, c, d, x[4], S[0], T[4]);
    d = MD5_FF(d, a, b, c, x[5], S[1], T[5]);
    c = MD5_FF(c, d, a, b, x[6], S[2], T[6]);
    b = MD5_FF(b, c, d, a, x[7], S[3], T[7]);
    a = MD5_FF(a, b, c, d, x[8], S[0], T[8]);
    d = MD5_FF(d, a, b, c, x[9], S[1], T[9]);
    c = MD5_FF(c, d, a, b, x[10], S[2], T[10]);
    b = MD5_FF(b, c, d, a, x[11], S[3], T[11]);
    a = MD5_FF(a, b, c, d, x[12], S[0], T[12]);
    d = MD5_FF(d, a, b, c, x[13], S[1], T[13]);
    c = MD5_FF(c, d, a, b, x[14], S[2], T[14]);
    b = MD5_FF(b, c, d, a, x[15], S[3], T[15]);

    a = MD5_GG(a, b, c, d, x[1], S[4], T[16]);
    d = MD5_GG(d, a, b, c, x[6], S[5], T[17]);
    c = MD5_GG(c, d, a, b, x[11], S[6], T[18]);
    b = MD5_GG(b, c, d, a, x[0], S[7], T[19]);
    a = MD5_GG(a, b, c, d, x[5], S[4], T[20]);
    d = MD5_GG(d, a, b, c, x[10], S[5], T[21]);
    c = MD5_GG(c, d, a, b, x[15], S[6], T[22]);
    b = MD5_GG(b, c, d, a, x[4], S[7], T[23]);
    a = MD5_GG(a, b, c, d, x[9], S[4], T[24]);
    d = MD5_GG(d, a, b, c, x[14], S[5], T[25]);
    c = MD5_GG(c, d, a, b, x[3], S[6], T[26]);
    b = MD5_GG(b, c, d, a, x[8], S[7], T[27]);
    a = MD5_GG(a, b, c, d, x[13], S[4], T[28]);
    d = MD5_GG(d, a, b, c, x[2], S[5], T[29]);
    c = MD5_GG(c, d, a, b, x[7], S[6], T[30]);
    b = MD5_GG(b, c, d, a, x[12], S[7], T[31]);

    a = MD5_HH(a, b, c, d, x[5], S[8], T[32]);
    d = MD5_HH(d, a, b, c, x[8], S[9], T[33]);
    c = MD5_HH(c, d, a, b, x[11], S[10], T[34]);
    b = MD5_HH(b, c, d, a, x[14], S[11], T[35]);
    a = MD5_HH(a, b, c, d, x[1], S[8], T[36]);
    d = MD5_HH(d, a, b, c, x[4], S[9], T[37]);
    c = MD5_HH(c, d, a, b, x[7], S[10], T[38]);
    b = MD5_HH(b, c, d, a, x[10], S[11], T[39]);
    a = MD5_HH(a, b, c, d, x[13], S[8], T[40]);
    d = MD5_HH(d, a, b, c, x[0], S[9], T[41]);
    c = MD5_HH(c, d, a, b, x[3], S[10], T[42]);
    b = MD5_HH(b, c, d, a, x[6], S[11], T[43]);
    a = MD5_HH(a, b, c, d, x[9], S[8], T[44]);
    d = MD5_HH(d, a, b, c, x[12], S[9], T[45]);
    c = MD5_HH(c, d, a, b, x[15], S[10], T[46]);
    b = MD5_HH(b, c, d, a, x[2], S[11], T[47]);

    a = MD5_II(a, b, c, d, x[0], S[12], T[48]);
    d = MD5_II(d, a, b, c, x[7], S[13], T[49]);
    c = MD5_II(c, d, a, b, x[14], S[14], T[50]);
    b = MD5_II(b, c, d, a, x[5], S[15], T[51]);
    a = MD5_II(a, b, c, d, x[12], S[12], T[52]);
    d = MD5_II(d, a, b, c, x[3], S[13], T[53]);
    c = MD5_II(c, d, a, b, x[10], S[14], T[54]);
    b = MD5_II(b, c, d, a, x[1], S[15], T[55]);
    a = MD5_II(a, b, c, d, x[8], S[12], T[56]);
    d = MD5_II(d, a, b, c, x[15], S[13], T[57]);
    c = MD5_II(c, d, a, b, x[6], S[14], T[58]);
    b = MD5_II(b, c, d, a, x[13], S[15], T[59]);
    a = MD5_II(a, b, c, d, x[4], S[12], T[60]);
    d = MD5_II(d, a, b, c, x[11], S[13], T[61]);
    c = MD5_II(c, d, a, b, x[2], S[14], T[62]);
    b = MD5_II(b, c, d, a, x[9], S[15], T[63]);

    md5_hash_factor[0] += a;
    md5_hash_factor[1] += b;
    md5_hash_factor[2] += c;
    md5_hash_factor[3] += d;
  }
  SAT_MEM_FREE(str_byte);

  for (i = 0; i < 4; ++i) {
    *output = sat_str_append_fmt(*output, "%08x", ntohl(md5_hash_factor[i]));
  }

  return RET_OK;
}
