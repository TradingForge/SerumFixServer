#include "Keypair.hpp"
#include <base58/base58.h>

Keypair::Keypair() : key_str(), key_b(bytes(0))
{}

Keypair::Keypair(const string& key) : key_str(key), key_b(_from_base58(key_str))
{}

Keypair::Keypair(const bytes& key)
{
    if (key.size() != SIZE_KEYPAIR) {
        throw -1;
    }

    key_b = key;
    key_str = _to_base58(key_b);
} 

Keypair::Keypair(const Keypair& other) : key_str(other.key_str), key_b(other.key_b)
{}

Keypair::Keypair(Keypair&& other) : key_str(other.key_str), key_b(other.key_b)
{
    other.key_b = bytes(0);
    other.key_str = "";
}

Keypair:: Keypair(const byte key[SIZE_KEYPAIR]) : key_b(key, key + SIZE_KEYPAIR)
{
    key_str = _to_base58(key_b);
}

Keypair::~Keypair()
{}

void Keypair::from_base58(const string& key)
{
    key_str = key;
    key_b = _from_base58(key_str);
}

PublicKey Keypair::get_pubkey() const
{
    auto vec = bytes(32);
    std::copy(key_b.end() - SIZE_PUBKEY, key_b.end(), vec.begin());
    return PublicKey(vec);
}

Keypair::bytes Keypair::_from_base58(const string& key)
{
    auto decoded_key = base58_decode(key);
    auto res = bytes(SIZE_KEYPAIR);
    memcpy(res.data(), decoded_key.data(), SIZE_KEYPAIR);
    return res;
}

Keypair::string Keypair::_to_base58(const bytes& key)
{
    return base58_encode(string((char*)key.data(), key.size()));
}

bool operator==(const Keypair &k1, const Keypair &k2)
{
    if (k1.key_b.size() == 0 || k2.key_b.size() == 0) {
        throw -1;
    }

    return  0 == memcmp(k1.key_b.data(), k2.key_b.data(), SIZE_PUBKEY) ? true : false;
}

bool operator==(const Keypair &k1, const std::string &k2)
{
    if (k1.key_str.size() == 0) {
        throw -1;
    }

    return  k1.key_str == k2;
}

Keypair& Keypair::operator= (const Keypair &key)
{
    if (this == &key)
        return *this;

    key_str = key.key_str;
    key_b = key.key_b;
    return *this;
}

Keypair& Keypair::operator= (Keypair &&key)
{
    if (this == &key)
        return *this;

    key_str = key.key_str;
    key_b = key.key_b;

    key.key_b = bytes(0);
    key.key_str = "";

    return *this;
}