#pragma once

#define NUM_ROUND 10

#include <windows.h>

void aes_gen_key(BYTE *Key, const BYTE *Seed, const BYTE *Rand);
void aes_gen_key2(BYTE *Key, const BYTE *Seed, const BYTE *Rand);
