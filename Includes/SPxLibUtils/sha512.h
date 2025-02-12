/*
    MIT License

    Copyright (c) 2020 LekKit https://github.com/LekKit

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    26/01/22: Modified by AGC of Cambridge Pixel for SHA512.
*/

#ifndef SHA512_H
#define SHA512_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stddef.h>

struct sha512_buff {
    uint64_t data_size;
    uint64_t h[8];
    uint8_t last_chunk[128];
    uint8_t chunk_size;
};

/* Initialization, must be called before any further use */
void sha512_init(struct sha512_buff* buff);

/* Process block of data of arbitary length, can be used on data streams (files, etc) */
void sha512_update(struct sha512_buff* buff, const void* data, size_t size);

/* Produces final hash values (digest) to be read
   If the buffer is reused later, init must be called again */
void sha512_finalize(struct sha512_buff* buff);

/* Read digest into 32-byte binary array */
void sha512_read(const struct sha512_buff* buff, uint8_t* hash);

/* Read digest into 64-char string as hex (without null-byte) */
void sha512_read_hex(const struct sha512_buff* buff, char* hex);

/* Hashes single contiguous block of data and reads digest into 32-byte binary array */
void sha512_easy_hash(const void* data, size_t size, uint8_t* hash);

/* Hashes single contiguous block of data and reads digest into 64-char string (without null-byte) */
void sha512_easy_hash_hex(const void* data, size_t size, char* hex);

#ifdef __cplusplus
}
#endif

#endif
