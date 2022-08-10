#pragma once

#include "PubKey.hpp"
#include <vector>

/**
 * Account Meta
 */
typedef struct {
  PubKey pubkey; /** An account's public key */
  bool is_writable; /** True if the `pubkey` can be loaded as a read-write account */
  bool is_signer; /** True if an Instruction requires a Transaction signature matching `pubkey` */
} AccountMeta;

/**
 * Instruction
 */
typedef struct {
  PubKey program_id; /** Pubkey of the instruction processor that executes this instruction */
  AccountMeta *accounts; /** Metadata for what accounts should be passed to the instruction processor */
  uint64_t account_len; /** Number of SolAccountMetas */
  uint8_t *data; /** Opaque data passed to the instruction processor */
  uint64_t data_len; /** Length of the data in bytes */
} Instruction;