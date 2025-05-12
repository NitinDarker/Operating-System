#include "wildlife_fs_crypto.h"
#include <string.h> // For strlen

void xor_cipher(char *data, size_t len, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        data[i] = data[i] ^ key[i % key_len];
    }
}