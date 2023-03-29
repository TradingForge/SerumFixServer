#include "PublicKey.hpp"

#include <cryptopp/default.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/sha.h>
#include <cryptopp/ecp.h>
#include <cryptopp/asn.h>
#include <cryptopp/eccrypto.h>
// #include <cryptopp/dlies.h>

namespace solana
{
    PublicKey::PublicKey() : key_b(SIZE_PUBKEY, 0)
    {

        key_str= _to_base58(key_b);
    }

    PublicKey::PublicKey(const string& key) : key_str(key), key_b(_from_base58(key_str))
    {
        auto k = _from_base58(key_str);
        if (k.size() != SIZE_PUBKEY) {
            throw -1;
        }

        key_b = k;
    }

    PublicKey::PublicKey(const bytes& key) : key_b(key)
    {
        if (key.size() != SIZE_PUBKEY) {
            throw -1;
        }

        key_b = key;
        key_str = _to_base58(key_b);
    } 

    PublicKey::PublicKey(const PublicKey& other) : key_str(other.key_str), key_b(other.key_b)
    {}

    PublicKey::PublicKey(PublicKey&& other) : key_str(std::move(other.key_str)), key_b(std::move(other.key_b))
    {
        // other.key_b = bytes(0);
        // other.key_str = "";
    }

    PublicKey::PublicKey(const byte key[SIZE_PUBKEY]) : key_b(key, key + SIZE_PUBKEY)
    {
        key_str = _to_base58(key_b);
    }

    PublicKey::~PublicKey()
    {}

    void PublicKey::from_base58(const string& key)
    {
        key_str = key;
        key_b = _from_base58(key_str);
    }

    PublicKey::bytes PublicKey::_from_base58(const string& key)
    {
        auto decoded_key = base58_decode(key);
        auto res = bytes(SIZE_PUBKEY);
        // memcpy(res.data(), decoded_key.data(), SIZE_PUBKEY);
        std::copy(decoded_key.begin(), decoded_key.end(), res.begin());
        return res;
    }

    PublicKey::string PublicKey::_to_base58(const bytes& key)
    {
        return base58_encode(string((char*)key.data(), key.size()));
    }

    bool operator==(const PublicKey &k1, const PublicKey &k2)
    {
        if (k1.key_b.empty() || k2.key_b.empty()) {
            throw -1;
        }

        return  !memcmp(k1.key_b.data(), k2.key_b.data(), SIZE_PUBKEY);
    }

    bool operator==(const PublicKey &k1, const std::string &k2)
    {
        if (k1.key_str.empty()) {
            throw -1;
        }

        return  k1.key_str == k2;
    }

    PublicKey& PublicKey::operator= (const PublicKey &key)
    {
        if (this == &key)
            return *this;

        key_str = key.key_str;
        key_b = key.key_b;
        return *this;
    }

    PublicKey& PublicKey::operator= (PublicKey &&key)
    {
        if (this == &key)
            return *this;

        key_str = key.key_str;
        key_b = key.key_b;

        key.key_b = bytes(0);
        key.key_str = "";

        return *this;
    }


    std::tuple<PublicKey, PublicKey::byte> PublicKey::find_public_key(const std::list<PublicKey>& seeds, const PublicKey& program_id)
    {
        const auto &program_id_bytes = program_id.key_b;
        std::list<bytes> seeds_bytes;
        for ( const auto& s : seeds)
            seeds_bytes.push_back(s.key_b);

        byte seed_bump = UINT8_MAX;

        bool is_curve_point = false;
        bytes hash;
        do{
            hash = try_find_public_key(program_id_bytes, seed_bump, seeds_bytes);
            --seed_bump;
            // is_curve_point = bytes_are_curve_point(hash);
        } while (seed_bump > 0 && is_curve_point);

        if (is_curve_point)
            return std::tuple<PublicKey, byte>(PublicKey(), 0);
        
        return std::tuple<PublicKey, byte>(PublicKey(hash), ++seed_bump);
    }

    // bool bytes_are_curve_point(const CryptoPP::byte* bytes, size_t len) {
    //     // Validate that the given bytes form a valid curve point
    //     // This implementation assumes that the input is a valid ECPoint
    //     CryptoPP::ECIES<CryptoPP::ECP>::Decryptor d;

    //     CryptoPP::OID curve_oid("1.3.132.0.10");
    //     d.AccessKey().AccessGroupParameters().Initialize(CryptoPP::OID("1.3.132.0.10"));
    //     CryptoPP::ECP ec;
    //     CryptoPP::ECP::Point p;
    //     ec.DecodePoint(p, bytes, len);
    //     // ec.DecodePoint(bytes, len);
    //     return ec.VerifyPoint(p);
    //     // CryptoPP::ECP::Point p;
    //     // p.Decode(bytes, len);
    //     // return p.IsValidElement();
    // }

    PublicKey::bytes PublicKey::try_find_public_key(const bytes program_id, byte seed_bump, const std::list<bytes>& seeds)
    {
        CryptoPP::SHA256 hasher;

        for (auto seed : seeds) {
            hasher.Update(seed.data(), seed.size());
        }
        hasher.Update(&seed_bump, 1);
        hasher.Update(program_id.data(), program_id.size());
        hasher.Update((byte*)PDA_MARKER, sizeof(PDA_MARKER) - 1);

        bytes digest(CryptoPP::SHA256::DIGESTSIZE);
        hasher.Final(digest.data());

        return digest;
    }
} // namespace solana