#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <string.h>

// SHA-256 Constants
#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

// SHA-256 Context structure
typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
} SHA256_CTX;

// Function declarations
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);
void sha256_transform(SHA256_CTX *ctx, const uint8_t data[]);
void sha256(const uint8_t *data, size_t len, uint8_t hash[]);

#endif // SHA256_H 