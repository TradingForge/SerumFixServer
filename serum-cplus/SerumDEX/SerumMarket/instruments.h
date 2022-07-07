#pragma once
#include <string>
#include <vector>
#include <base58/base58.h>
#include "sol_sdk/types.h"
#include "sol_sdk/pubkey.h"
#include "enums.h"
#include "structs.h"

SolPubkey base58str_to_pubkey(const std::string& str_key) {
    std::vector<uint8_t> _key;
    _key = Base58::decode(str_key);
    if (_key.size() > SIZE_PUBKEY) {
        throw std::exception();
    }

    int i = SIZE_PUBKEY - 1;
    SolPubkey key;
    std::for_each(_key.rbegin(), _key.rend(), [&i, &key] (uint8_t byte) 
    {
        key.x[i--] = byte;
    });
    return key;
}

SolKeyPair base58str_to_keypair(const std::string& str_key) {
    std::vector<uint8_t> _key;
    _key = Base58::decode(str_key);
    if (_key.size() > SIZE_PUBKEY * 2) {
        throw std::exception();
    }

    int i = SIZE_PUBKEY * 2 - 1;
    SolKeyPair key;
    std::for_each(_key.rbegin(), _key.rend(), [&i, &key] (uint8_t byte) 
    {
        key.x[i--] = byte;
    });
    return key;
}
