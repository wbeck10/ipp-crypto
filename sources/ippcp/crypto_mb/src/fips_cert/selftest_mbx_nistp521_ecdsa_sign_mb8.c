/*******************************************************************************
 * Copyright 2023 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
#ifdef MBX_FIPS_MODE

#include <crypto_mb/fips_cert.h>
#include <internal/fips_cert/common.h>

#include <crypto_mb/ec_nistp521.h>

/* KAT TEST (taken from FIPS 186-4 examples)*/
// msg ==
// "9ecd500c60e701404922e58ab20cc002651fdee7cbc9336adda33e4c1088fab1"
// "964ecb7904dc6856865d6c8e15041ccf2d5ac302e99d346ff2f686531d255216"
// "78d4fd3f76bbf2c893d246cb4d7693792fe18172108146853103a51f824acc62"
// "1cb7311d2463c3361ea707254f2b052bc22cb8012873dcbb95bf1a5cc53ab89f"

/* msgDigest == SHA-521(msg) */
static const int8u msg_digest[MBX_NISTP521_DATA_BYTE_LEN] = {0x00,0x00,0x65,0xf8,0x34,0x08,0x09,0x22,
                                                         0x61,0xbd,0xa5,0x99,0x38,0x9d,0xf0,0x33,
                                                         0x82,0xc5,0xbe,0x01,0xa8,0x1f,0xe0,0x0a,
                                                         0x36,0xf3,0xf4,0xbb,0x65,0x41,0x26,0x3f,
                                                         0x80,0x16,0x27,0xc4,0x40,0xe5,0x08,0x09,
                                                         0x71,0x2b,0x0c,0xac,0xe7,0xc2,0x17,0xe6,
                                                         0xe5,0x05,0x1a,0xf8,0x1d,0xe9,0xbf,0xec,
                                                         0x32,0x04,0xdc,0xd6,0x3c,0x4f,0x9a,0x74,
                                                         0x10,0x47};
/* key pair */
static const int8u d[MBX_NISTP521_DATA_BYTE_LEN]          = {0x26,0xb9,0x4c,0xa1,0x7a,0x56,0x5e,0x6c,
                                                         0xee,0x9f,0xcf,0xcc,0xc0,0x55,0x86,0x8c,
                                                         0x21,0x0f,0x9a,0xc2,0xcc,0x74,0xf5,0x72,
                                                         0x9b,0x35,0xa2,0xdc,0x80,0x00,0xab,0x2f,
                                                         0xf0,0xfa,0x7f,0x26,0x86,0xb8,0x7d,0xed,
                                                         0x15,0xe5,0x78,0x26,0x8d,0xf0,0x67,0xba,
                                                         0x4f,0x8f,0x3d,0x10,0xcf,0x0a,0xef,0x2c,
                                                         0xa8,0x3c,0x53,0xbc,0x04,0x27,0xd3,0x49,
                                                         0xf7,0x00};

static const int8u k[MBX_NISTP521_DATA_BYTE_LEN]          = {0xf1,0x6d,0x4c,0x86,0x3e,0x59,0xc7,0x83,
                                                         0xbb,0xec,0xd4,0x33,0xa7,0x0a,0x3f,0xec,
                                                         0xfc,0x81,0xf8,0xdc,0xc9,0x9a,0xdb,0xf3,
                                                         0xe1,0xcf,0x46,0xcf,0xfe,0x12,0x2d,0x81,
                                                         0x60,0xac,0xde,0xdd,0x3c,0x05,0xe3,0x9b,
                                                         0x76,0x0c,0xc6,0xb5,0x52,0xcd,0xff,0x17,
                                                         0x6a,0xb1,0xc3,0x83,0xaa,0xb9,0xba,0xa5,
                                                         0x86,0xde,0xa6,0x29,0xaa,0x6c,0xab,0xf5,
                                                         0x3a,0x00};
/* signature */
static const int8u r[MBX_NISTP521_DATA_BYTE_LEN]          = {0x00,0x4d,0xe8,0x26,0xea,0x70,0x4a,0xd1,
                                                         0x0b,0xc0,0xf7,0x53,0x8a,0xf8,0xa3,0x84,
                                                         0x3f,0x28,0x4f,0x55,0xc8,0xb9,0x46,0xaf,
                                                         0x92,0x35,0xaf,0x5a,0xf7,0x4f,0x2b,0x76,
                                                         0xe0,0x99,0xe4,0xbc,0x72,0xfd,0x79,0xd2,
                                                         0x8a,0x38,0x0f,0x8d,0x4b,0x4c,0x91,0x9a,
                                                         0xc2,0x90,0xd2,0x48,0xc3,0x79,0x83,0xba,
                                                         0x05,0xae,0xa4,0x2e,0x2d,0xd7,0x9f,0xdd,
                                                         0x33,0xe8};

static const int8u s[MBX_NISTP521_DATA_BYTE_LEN]          = {0x00,0x87,0x48,0x8c,0x85,0x9a,0x96,0xfe,
                                                         0xa2,0x66,0xea,0x13,0xbf,0x6d,0x11,0x4c,
                                                         0x42,0x9b,0x16,0x3b,0xe9,0x7a,0x57,0x55,
                                                         0x90,0x86,0xed,0xb6,0x4a,0xed,0x4a,0x18,
                                                         0x59,0x4b,0x46,0xfb,0x9e,0xfc,0x7f,0xd2,
                                                         0x5d,0x8b,0x2d,0xe8,0xf0,0x9c,0xa0,0x58,
                                                         0x7f,0x54,0xbd,0x28,0x72,0x99,0xf4,0x7b,
                                                         0x2f,0xf1,0x24,0xaa,0xc5,0x66,0xe8,0xee,
                                                         0x3b,0x43};

DLL_PUBLIC
fips_test_status fips_selftest_mbx_nistp521_ecdsa_sign_mb8(void) {
  fips_test_status test_result = MBX_ALGO_SELFTEST_OK;

  /* output signature */
  int8u out_r[MBX_LANES][MBX_NISTP521_DATA_BYTE_LEN];
  int8u out_s[MBX_LANES][MBX_NISTP521_DATA_BYTE_LEN];

  /* function input parameters */
  // msg digest
  const int8u *const pa_pub_msg_digest[MBX_LANES] = {
      msg_digest, msg_digest, msg_digest, msg_digest,
      msg_digest, msg_digest, msg_digest, msg_digest};
  // key pair
  const int64u *const pa_prv_d[MBX_LANES] = {
      (int64u *)d, (int64u *)d, (int64u *)d, (int64u *)d,
      (int64u *)d, (int64u *)d, (int64u *)d, (int64u *)d};
  const int64u *const pa_prv_k[MBX_LANES] = {
      (int64u *)k, (int64u *)k, (int64u *)k, (int64u *)k,
      (int64u *)k, (int64u *)k, (int64u *)k, (int64u *)k};
  // output signature
  int8u *pa_sign_r[MBX_LANES] = {out_r[0], out_r[1], out_r[2], out_r[3],
                                 out_r[4], out_r[5], out_r[6], out_r[7]};
  int8u *pa_sign_s[MBX_LANES] = {out_s[0], out_s[1], out_s[2], out_s[3],
                                 out_s[4], out_s[5], out_s[6], out_s[7]};

  /* test function */
  mbx_status expected_status_mb8 = MBX_SET_STS_ALL(MBX_STATUS_OK);

  mbx_status sts;
  sts = mbx_nistp521_ecdsa_sign_mb8(pa_sign_r, pa_sign_s, pa_pub_msg_digest, pa_prv_k, pa_prv_d, NULL);
  if (expected_status_mb8 != sts) {
    test_result = MBX_ALGO_SELFTEST_BAD_ARGS_ERR;
  }
  // compare output signature to known answer
  int r_output_status;
  int s_output_status;
  for (int i = 0; (i < MBX_LANES) && (MBX_ALGO_SELFTEST_OK == test_result); ++i) {
    r_output_status = mbx_is_mem_eq(pa_sign_r[i], MBX_NISTP521_DATA_BYTE_LEN, r, MBX_NISTP521_DATA_BYTE_LEN);
    s_output_status = mbx_is_mem_eq(pa_sign_s[i], MBX_NISTP521_DATA_BYTE_LEN, s, MBX_NISTP521_DATA_BYTE_LEN);
    if (!r_output_status || !s_output_status) { // wrong output
      test_result = MBX_ALGO_SELFTEST_KAT_ERR;
    }
  }

  return test_result;
}

#ifndef BN_OPENSSL_DISABLE

#define MEM_FREE(BN_PTR1, BN_PTR2) { \
    BN_free(BN_PTR1);                \
    BN_free(BN_PTR2); }

DLL_PUBLIC
fips_test_status fips_selftest_mbx_nistp521_ecdsa_sign_ssl_mb8(void) {
  fips_test_status test_result = MBX_ALGO_SELFTEST_OK;

  /* ssl key pair */
  BIGNUM *BN_d = BN_new();
  BIGNUM *BN_k = BN_new();
  /* check if allocated memory is valid */
  if(NULL == BN_d || NULL == BN_k) {
    test_result = MBX_ALGO_SELFTEST_BAD_ARGS_ERR;
    MEM_FREE(BN_d, BN_k)
    return test_result;
  }

  /* output signature */
  int8u out_r[MBX_LANES][MBX_NISTP521_DATA_BYTE_LEN];
  int8u out_s[MBX_LANES][MBX_NISTP521_DATA_BYTE_LEN];
  /* function status and expected status */
  mbx_status sts;
  mbx_status expected_status_mb8 = MBX_SET_STS_ALL(MBX_STATUS_OK);
  /* output validity statuses */
  int r_output_status;
  int s_output_status;

  // set ssl key pair
  BN_lebin2bn(d, MBX_NISTP521_DATA_BYTE_LEN, BN_d);
  BN_lebin2bn(k, MBX_NISTP521_DATA_BYTE_LEN, BN_k);

  /* function input parameters */
  // msg digest
  const int8u *const pa_pub_msg_digest[MBX_LANES] = {
      msg_digest, msg_digest, msg_digest, msg_digest,
      msg_digest, msg_digest, msg_digest, msg_digest};
  // key pair
  const BIGNUM *pa_prv_d[MBX_LANES] = {BN_d, BN_d, BN_d, BN_d,
                                       BN_d, BN_d, BN_d, BN_d};
  const BIGNUM *pa_prv_k[MBX_LANES] = {BN_k, BN_k, BN_k, BN_k,
                                       BN_k, BN_k, BN_k, BN_k};
  // output signature
  int8u *pa_sign_r[MBX_LANES] = {out_r[0], out_r[1], out_r[2], out_r[3],
                                 out_r[4], out_r[5], out_r[6], out_r[7]};
  int8u *pa_sign_s[MBX_LANES] = {out_s[0], out_s[1], out_s[2], out_s[3],
                                 out_s[4], out_s[5], out_s[6], out_s[7]};

  /* test function */
  sts = mbx_nistp521_ecdsa_sign_ssl_mb8(pa_sign_r, pa_sign_s, pa_pub_msg_digest, pa_prv_k, pa_prv_d, NULL);
  if (expected_status_mb8 != sts) {
    test_result = MBX_ALGO_SELFTEST_BAD_ARGS_ERR;
  }
  // compare output signature to known answer
  for (int i = 0; (i < MBX_LANES) && (MBX_ALGO_SELFTEST_OK == test_result); ++i) {
    r_output_status = mbx_is_mem_eq(pa_sign_r[i], MBX_NISTP521_DATA_BYTE_LEN, r, MBX_NISTP521_DATA_BYTE_LEN);
    s_output_status = mbx_is_mem_eq(pa_sign_s[i], MBX_NISTP521_DATA_BYTE_LEN, s, MBX_NISTP521_DATA_BYTE_LEN);
    if (!r_output_status || !s_output_status) { // wrong output
      test_result = MBX_ALGO_SELFTEST_KAT_ERR;
    }
  }

  // memory free
  MEM_FREE(BN_d, BN_k)

  return test_result;
}

#endif // BN_OPENSSL_DISABLE
#endif // MBX_FIPS_MODE
