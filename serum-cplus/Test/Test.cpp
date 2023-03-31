#include <iostream>
#include <base64/base64.h>
#include <sol_sdk/PublicKey.hpp>

int main(int argc, char const *argv[])
{
    auto seed1 = solana::PublicKey("1111111QLbz7JHiBTspS962RLKV8GndWFwiEaqKM");
    auto seed2 = solana::PublicKey("1111111ogCyDbaRMvkdsHB3qfdyFYaG1WtRUAfdh");
    auto program_id = solana::PublicKey("11111112D1oxKts8YPdTJRG5FzxTNpMtWmq8hkVx3");

    auto new_key = solana::PublicKey::find_public_key({seed1, seed2}, program_id);
    return 0;
}
