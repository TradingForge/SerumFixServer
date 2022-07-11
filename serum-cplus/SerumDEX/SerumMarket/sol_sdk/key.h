#pragma once

#include "types.h"

/**
 * Size of Public key in bytes
 */
#define SIZE_PUBKEY 32

/**
 * Public key
 */
typedef struct {
  uint8_t x[SIZE_PUBKEY];
} SolPubkey;

/**
 * Key pair
 */
typedef struct {
  uint8_t x[SIZE_PUBKEY * 2];
} SolKeyPair;