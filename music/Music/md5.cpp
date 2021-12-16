#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void to_bytes2(uint32_t val, uint8_t* bytes)
{
	bytes[0] = (uint8_t)val;
	bytes[1] = (uint8_t)(val >> 8);
	bytes[2] = (uint8_t)(val >> 16);
	bytes[3] = (uint8_t)(val >> 24);
}

void md5_algo(const uint8_t* initial_msg, size_t initial_len, uint8_t* digest)
{
	// Переменные которые хранят хэш
	uint32_t h0, h1, h2, h3;

	// Message (to prepare)
	uint8_t* msg = NULL;

	size_t new_len, offset;
	uint32_t w[16];
	uint32_t a, b, c, d, i, f, g, temp;

	h0 = 0x67452301;
	h1 = 0xefcdab89;
	h2 = 0x98badcfe;
	h3 = 0x10325476;

	for (new_len = initial_len + 1; new_len % (512 / 8) != 448 / 8; new_len++)
		;

	msg = (uint8_t*)malloc(new_len + 8);
	memcpy(msg, initial_msg, initial_len);
	msg[initial_len] = 0x80; 
	for (offset = initial_len + 1; offset < new_len; offset++)
		msg[offset] = 0; 

	to_bytes2(initial_len * 8, msg + new_len);
	to_bytes2(initial_len >> 29, msg + new_len + 4);

	for (offset = 0; offset < new_len; offset += (512 / 8)) {

		for (i = 0; i < 16; i++)
			w[i] = to_int32(msg + offset + i * 4);

		a = h0;
		b = h1;
		c = h2;
		d = h3;

		for (i = 0; i < 64; i++) {

			if (i < 16) {
				f = (b & c) | ((~b) & d);
				g = i;
			}
			else if (i < 32) {
				f = (d & b) | ((~d) & c);
				g = (5 * i + 1) % 16;
			}
			else if (i < 48) {
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			}
			else {
				f = c ^ (b | (~d));
				g = (7 * i) % 16;
			}

			temp = d;
			d = c;
			c = b;
			b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
			a = temp;

		}

		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;

	}

	//очищение памяти
	free(msg);

	//digest[16] = h0 + h1 + h2 + h3 
	to_bytes2(h0, digest);
	to_bytes2(h1, digest + 4);
	to_bytes2(h2, digest + 8);
	to_bytes2(h3, digest + 12);
}

uint32_t to_int32(const uint8_t* bytes)
{
	return (uint32_t)bytes[0]
		| ((uint32_t)bytes[1] << 8)
		| ((uint32_t)bytes[2] << 16)
		| ((uint32_t)bytes[3] << 24);
}






