#pragma once

#include <windows.h>

void Rijndael_set_key_encrypt(DWORD *rk, const BYTE *key);
void Rijndael_set_key_decrypt(DWORD *rk, const BYTE *key);
void Rijndael_encrypt(const DWORD *rk, const BYTE *plaintext, BYTE *ciphertext);
void Rijndael_decrypt(const DWORD *rk, const BYTE *ciphertext, BYTE *plaintext);
void fake_set_key_encrypt(DWORD *rk, const BYTE *key);
