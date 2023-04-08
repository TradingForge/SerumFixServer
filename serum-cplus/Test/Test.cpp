#include <iostream>
#include <base64/base64.h>
#include <sol_sdk/PublicKey.hpp>
#include <SerumDEX/SerumMarket/constants.hpp>

int main(int argc, char const *argv[])
{
    auto seed1 = solana::PublicKey("5ejVgdkJkgwptewzs5CLYdm6vJxpEL47sErAcnom6i8A");
    auto seed2 = solana::PublicKey("7vfCXTUXx5WJV5JADk17DUJ4ksgau7utNKj4b963voxs");
    auto program_id = solana::PublicKey("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
    // std::vector<uint8_t> t = {154,35,36,17,41,248,250,11,240,220,191,113,111,100,217,22,217,58,214,92,133,23,149,92,51,227,15,17,143,109,14,222};
    // auto program_id = solana::PublicKey(t);
    // auto st = program_id.get_str_key();
    auto new_key = solana::PublicKey::find_public_key({seed1, TOKEN_PROGRAM_ID, seed2}, ASSOCIATED_TOKEN_PROGRAM_ID);

    return 0;
}
