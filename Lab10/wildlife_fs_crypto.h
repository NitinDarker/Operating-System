#ifndef WILDLIFE_FS_CRYPTO_H
#define WILDLIFE_FS_CRYPTO_H

#include <stddef.h> // For size_t

// XOR encryption/decryption
// data- pointer to the data to encrypt/decrypt
// len- length of the data
// key- encryption key
void xor_cipher(char *data, size_t len, const char *key);

#endif