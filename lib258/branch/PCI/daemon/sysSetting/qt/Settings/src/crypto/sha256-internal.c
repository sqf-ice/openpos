/*
 * SHA-256 hash implementation and interface functions
 * Copyright (c) 2003-2011, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include "includes.h"

#include "common.h"
#include "sha256.h"
#include "crypto.h"

#define SHA256_BLOCK_SIZE 64

struct sha256_state {
	u64 length;
	u32 state[8], curlen;
	u8 buf[SHA256_BLOCK_SIZE];
};

static void sha256_init(struct sha256_state *md);
static int sha256_process(struct sha256_state *md, const unsigned char *in,
			  unsigned long inlen);
static int sha256_done(struct sha256_state *md, unsigned char *out);


/**
 * sha256_vector - SHA256 hash for data vector
 * @num_elem: Number of elements in the data vector
 * @addr: Pointers to the data areas
 * @len: Lengths of the data blocks
 * @mac: Buffer for the hash
 * Returns: 0 on success, -1 of failure
 */
int sha256_vector(size_t num_elem, const u8 *addr[], const size_t *len,
		  u8 *mac)
{
	struct sha256_state ctx;
	size_t i;

	sha256_init(&ctx);
	for (i = 0; i < num_elem; i++)
		if (sha256_process(&ctx, addr[i], len[i]))
			return -1;
	if (sha256_done(&ctx, mac))
		return -1;
	return 0;
}


/* ===== start - public domain SHA256 implementation ===== */

/* This is based on SHA256 implementation in LibTomCrypt that was released into
 * public domain by Tom St Denis. */

/* the K array */
static const unsigned long K[64] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
	0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
	0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
	0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
	0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
	0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
	0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
	0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
	0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
	0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};


/* Various logical functions */
#define RORc(x, y) \
( ((((unsigned long) (x) & 0xFFFFFFFFUL) >> (unsigned long) ((y) & 31)) | \
   ((unsigned long) (x) << (unsigned long) (32 - ((y) & 31)))) & 0xFFFFFFFFUL)
#define Ch(x,y,z)       (z ^ (x & (y ^ z)))
#define Maj(x,y,z)      (((x | y) & z) | (x & y)) 
#define S(x, n)         RORc((x), (n))
#define R(x, n)         (((x)&0xFFFFFFFFUL)>>(n))
#define Sigma0(x)       (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x)       (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0(x)       (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x)       (S(x, 17) ^ S(x, 19) ^ R(x, 10))
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

/* compress 512-bits */
static int sha256_compress(struct sha256_state *md, unsigned char *buf)
{
	u32 S[8], W[64], t0, t1;
	u32 t;
	int i;

	/* copy state into S */
	for (i = 0; i < 8; i++) {
		S[i] = md->state[i];
	}

	/* copy the state into 512-bits into W[0..15] */
	for (i = 0; i < 16; i++)
		W[i] = WPA_GET_BE32(buf + (4 * i));

	/* fill W[16..63] */
	for (i = 16; i < 64; i++) {
		W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) +
			W[i - 16];
	}        

	/* Compress */
#define RND(a,b,c,d,e,f,g,h,i)                          \
	t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];	\
	t1 = Sigma0(a) + Maj(a, b, c);			\
	d += t0;					\
	h  = t0 + t1;

	for (i = 0; i < 64; ++i) {
		RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], i);
		t = S[7]; S[7] = S[6]; S[6] = S[5]; S[5] = S[4]; 
		S[4] = S[3]; S[3] = S[2]; S[2] = S[1]; S[1] = S[0]; ��1fA�6�IA�NI��9D$փ�9l$�1���H�|$ 蟚��H��H[]A\A]A^A_�H��\3 A�D$H�s���L���#����4���H�|$ �   �   蚚��H�|$ 1Ҿ   �ɛ��A�t$�  � H�\$0�����N����������H�IH�ھ  � ��H�|������
�J�JH��H9�u��n���H�|$ H������H�����f�S1�H��H��H�?Y3 �G    f�G  f�G  H�G    H�G     H��H��`���H��[�H������H���j����հ��H�D$�{���H������H�D$H���F���fD  UH��1�SH��H���V�v�����Cf����   �sf����   ������   E1�D  1�A9�}H�CH��t�SA��Hc�H�<RH��E1�A9�}�EA��H�H�@HMf��t*1�1��4��f�4�t@�t�sH��D��A9���CA��D9������H��[]�f�SH��H��H��W3 �G    f�G  f�G  H�G    H�G     H��H������H��[�H��褔��H������臯��H�D$�-���H��赕��H�D$H��������     AT1�I��UH��SH��H���R�s++s�=����U�EH��H���$    �D$    �T$H�ډD$������S�3H�������O����$�L$9���   D�D$D�L$E9���   D  1�E��x)A�D$D9�~I�D$H��tA�t$A��Hc�H�<vH�1�D��sxD�UD9�}�E��Hc�H�vHEHc3H�4vH�9�}9Hc�1�H�IH�H�D  ���f��L�L�L$H��9��D�L$A��E9�}�$�R���f.�     H��[]A\ÐfD  SH��H��H�!V3 �G    f�G  f�G  H�G    H�G     H��H��r���H��[�H���Ԓ��H���L���跭��H�D$�]���H������H�D$H���(����     H�\$�H�l$�H��H��L�d$�L�l$�A��H��(A�������fD�kfD�cfD�cH�kH�k L�d$H�\$H�l$L�l$ H��(ÐfD  H�G H�G     H�    �fff.�     AWAVAUI��ATI��H��USH��h  �q�  -1P  HǄ$@      ��vcI�E L��$@  1�1�1�L���P(L��L��L��$@  �0� H��$@  HǄ$@      H��tH�{讒����t"H��h  []A\A]A^A_�H�1� Hc�H���H���0�����A�   �D$H��$_  L��Ƅ$_  
L��$@  �S���H��$_  L��D$L��$@  �7���H��$_  L��D$L��$@  ����=��  ���b  =�   �$�   ���  N$�   �T$�t$L��L��$@  �$�   O���1������D$����H�5V3 H��$   L��$@  覬���$H��$   1�贬��D��$  E����  ��$  ���t  ��1�������'�    ;�$  �S  ���   ;�$  �@  ��+�$  �����Hc�H�$  9�~	�����������9�u��|$ H��$  ��$  ��  H�A�l$H)��l$E���7  H��$   �lm H�5+U3 1҉�H��H�D$�ڱ��A�D$D�p�A����L  Hc�H�,$�    E1�A9�}I�D$H��tA�T$A��Hc�L�<RIǋ�$8  ���,  ��$<  ���  ��$0  H�$L����Hc�H�$(  H�����  H9$��  �|$�d  1�fA�|$ tZ��E �M�����H��A�G�E�M���H��A�G�E�MH�����H��A�A�D$I��9��A��A����O  A�D$����E1��D$����I�E L��$@  1�1�1�L���P(��   L��$@  �ǎ��1�L��H��H���'���H��$P  H��HǄ$P      L��$@  ����L��$@  H��$P  L������H��$P  HǄ$P      H��tH�{��������w  H��$@  1�L��������
���A�   �D$ �W���E1��D$ �J���D  fA�|$ �����1�f�     �U ���A�W�U�A�W�UH���A�A�T$I��9��A��A��������H�|$蠮��H��$   L��$@  苮���f���A�T$D�z�A��