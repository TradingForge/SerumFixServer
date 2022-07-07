#pragma once
#include "sol_sdk/pubkey.h"
#include "instruments.h"

// Data length of a token mint account.
#define MINT_LEN 82

// Data length of a token account.
#define ACCOUNT_LEN 165

// Data length of a multisig token account.
#define MULTISIG_LEN 355

// Program ID for the associated token account program.
#define  ASSOCIATED_TOKEN_PROGRAM_ID base58str_to_pubkey("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL")
// "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL"

// Public key that identifies the SPL token program.
#define  TOKEN_PROGRAM_ID base58str_to_pubkey("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA")
// "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA"

/*
* Public key of the "Native Mint" for wrapping SOL to SPL token.
*
* The Token Program can be used to wrap native SOL. Doing so allows native SOL to be treated like any
* other Token program token type and can be useful when being called from other programs that interact
* with the Token Program's interface.
*/
#define  WRAPPED_SOL_MINT base58str_to_pubkey("So11111111111111111111111111111111111111112")
// "So11111111111111111111111111111111111111112"