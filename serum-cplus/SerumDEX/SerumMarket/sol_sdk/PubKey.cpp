#include "PubKey.hpp"
#include <string.h>
#include <base58/base58.h>

PubKey::PubKey() : key_str(), key_b(bytes(0))
{}

PubKey::PubKey(const string& key) : key_str(key), key_b(_from_base58(key_str))
{}

PubKey::PubKey(const bytes& key)
{
    if (key.size() != SIZE_PUBKEY) {
        throw -1;
    }

    key_b = key;
    key_str = base58_encode(string((char*)key.data(), key.size()));
} 

PubKey::PubKey(const PubKey& other) : key_str(other.key_str), key_b(other.key_b)
{}

PubKey::PubKey(PubKey&& other) : key_str(other.key_str), key_b(other.key_b)
{
    other.key_b = bytes(0);
    other.key_str = "";
}

PubKey::~PubKey()
{}

void PubKey::from_base58(const string& key)
{
    key_str = key;
    key_b = _from_base58(key_str);
}

PubKey::bytes PubKey::_from_base58(const string& key)
{
    auto decoded_key = base58_decode(key);
    auto res = bytes(SIZE_PUBKEY);
    memcpy(res.data(), decoded_key.data(), SIZE_PUBKEY);
    return res;
}

bool operator==(const PubKey &k1, const PubKey &k2)
{
    if (k1.key_b.size() == 0 || k2.key_b.size() == 0) {
        throw -1;
    }

    return  0 == memcmp(k1.key_b.data(), k2.key_b.data(), SIZE_PUBKEY) ? true : false;
}

bool operator==(const PubKey &k1, const std::string &k2)
{
    if (k1.key_str.size() == 0) {
        throw -1;
    }

    return  k1.key_str == k2;
}

PubKey& PubKey::operator= (const PubKey &key)
{
    if (this == &key)
        return *this;

    key_str = key.key_str;
    key_b = key.key_b;
    return *this;
}

PubKey& PubKey::operator= (PubKey &&key)
{
    if (this == &key)
        return *this;

    key_str = key.key_str;
    key_b = key.key_b;

    key.key_b = bytes(0);
    key.key_str = "";

    return *this;
}