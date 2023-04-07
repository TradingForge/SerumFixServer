#include "PublicKey.hpp"
// #include <gcrypt.h>
// #include <openssl/ec.h>
// #include <openssl/obj_mac.h>

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

    // bool is_on_curve_compressed(const std::vector<uint8_t>& compressed_point) {
    //     // // Convert the compressed point to OpenSSL's format
    //     // std::vector<unsigned char> point_bytes(compressed_point.begin(), compressed_point.end());
    //     // point_bytes.insert(point_bytes.begin(), 0x02); // Set the "y" sign bit to 0, as required by the format

    //     // // Create a new EC_GROUP with the parameters of the ed25519 curve
    //     // EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_ED25519);

    //     // // Create a new EC_POINT and decode the compressed point
    //     // EC_POINT* point = EC_POINT_new(group);
    //     // EC_POINT_oct2point(group, point, point_bytes.data(), point_bytes.size(), NULL);

    //     // // Check if the point is on the curve
    //     // bool result = EC_POINT_is_on_curve(group, point, NULL) == 1;

    //     // // Free the memory allocated for the point and group
    //     // EC_POINT_free(point);
    //     // EC_GROUP_free(group);
    //     uint8_t tt[50] = {0}; 
    //     crypto_core_ristretto255_from_hash(tt, compressed_point.data());
    //     auto a = crypto_core_ristretto255_is_valid_point(tt);
    //     return 0;
    // }



// int decompress_and_check_point(const unsigned char *y_bytes) {
//     gcry_ctx_t ctx;
//     gcry_mpi_t x, y, z;
//     gcry_error_t err;

//     err = gcry_mpi_ec_new(&ctx, NULL, "Ed25519");
//     if (err) {
//         fprintf(stderr, "Error creating context: %s\n", gcry_strerror(err));
//         return 0;
//     }

//     y = gcry_mpi_new(256);
//     gcry_mpi_scan(&y, GCRYMPI_FMT_USG, y_bytes, 32, NULL);

//     x = gcry_mpi_new(256);
//     z = gcry_mpi_new(256);

//     // Розрахунок X-координати вручну
//     gcry_mpi_t p, d, q, one, yy, u, v, check;
//     p = gcry_mpi_ec_get_mpi("p", ctx, 0);
//     d = gcry_mpi_ec_get_mpi("d", ctx, 0);
//     q = gcry_mpi_ec_get_mpi("q", ctx, 0);
//     one = gcry_mpi_new(256);
//     yy = gcry_mpi_new(256);
//     u = gcry_mpi_new(256);
//     v = gcry_mpi_new(256);
//     check = gcry_mpi_new(256);

//     gcry_mpi_set_ui(one, 1);
//     gcry_mpi_powm(yy, y, one, p);
//     gcry_mpi_subm(u, yy, one, p);
//     gcry_mpi_mulm(v, yy, d, p);
//     gcry_mpi_addm(v, v, one, p);
//     gcry_mpi_powm(x, v, q, p);
//     gcry_mpi_mulm(check, x, x, p);
//     gcry_mpi_mulm(check, check, v, p);
//     gcry_mpi_subm(check, check, u, p);

//     // Перевірка належності кривій
//     int is_on_curve = gcry_mpi_cmp_ui(check, 0) == 0;

//     gcry_mpi_release(y);
//     gcry_mpi_release(x);
//     gcry_mpi_release(z);
//     gcry_mpi_release(p);
//     gcry_mpi_release(d);
//     gcry_mpi_release(q);
//     gcry_mpi_release(one);
//     gcry_mpi_release(yy);
//     gcry_mpi_release(u);
//     gcry_mpi_release(v);
//     gcry_mpi_release(check);
//     gcry_ctx_release(ctx);

//     return is_on_curve;
// }


    std::tuple<PublicKey, PublicKey::byte> PublicKey::find_public_key(const std::list<PublicKey>& seeds, const PublicKey& program_id)
    {
        // auto tt = decompress_and_check_point(program_id.data());
        // const auto &program_id_bytes = program_id.key_b;
        // std::list<bytes> seeds_bytes;
        // for ( const auto& s : seeds)
        //     seeds_bytes.push_back(s.key_b);

        // byte seed_bump = UINT8_MAX;

        // bool is_curve_point = true;
        // bytes hash;
        // do{
        //     hash = _try_find_public_key(program_id_bytes, seed_bump, seeds_bytes);
        //     --seed_bump;
        //     is_curve_point = _bytes_are_curve_point(hash);
        // } while (seed_bump > 0 && is_curve_point);

        // if (is_curve_point)
        //     return std::tuple<PublicKey, byte>(PublicKey(), 0);
        
        // return std::tuple<PublicKey, byte>(PublicKey(hash), ++seed_bump);
    }

    bool PublicKey::_bytes_are_curve_point(const bytes &point) 
    {
        // if (point.size() != crypto_core_ed25519_BYTES) {
        //     return false;
        // }

        // return crypto_core_ed25519_is_valid_point(point.data());
    }

    PublicKey::bytes PublicKey::_try_find_public_key(const bytes& program_id, byte seed_bump, const std::list<bytes>& seeds)
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

    // uint64_t load8(const uint8_t* input) {
    //     return (static_cast<uint64_t>(input[0]))
    //         | (static_cast<uint64_t>(input[1]) << 8)
    //         | (static_cast<uint64_t>(input[2]) << 16)
    //         | (static_cast<uint64_t>(input[3]) << 24)
    //         | (static_cast<uint64_t>(input[4]) << 32)
    //         | (static_cast<uint64_t>(input[5]) << 40)
    //         | (static_cast<uint64_t>(input[6]) << 48)
    //         | (static_cast<uint64_t>(input[7]) << 56);
    // }

    // PublicKey::string PublicKey::_decompress_point(const bytes& point_)
    // {
    //     // bytes Y = point;
    //     // bytes Z = bytes(crypto_core_ed25519_BYTES, 0); Z[0] = 1;
    //     // bytes YY = bytes(crypto_core_ed25519_BYTES);
    //     // crypto_core_ed25519_scalar_mul(YY.data(), Y.data(), Y.data());
        
    //     // bytes point = point_;
    //     // std::reverse(point.begin(),point.end());
    //     mpz_class Y, Z(1), T, YY, u, v;
    //     // YY = Y*Y;
    //     // auto tt = 0;

    //     uint64_t low_51_bit_mask = (1UL << 51) - 1;
    //     std::vector<uint64_t> t = {
    //         load8(point_.data()) & low_51_bit_mask,
    //         (load8(point_.data() + 6) >> 3) & low_51_bit_mask,
    //         (load8(point_.data() + 12) >> 6) & low_51_bit_mask,
    //         (load8(point_.data() + 19) >> 1) & low_51_bit_mask,
    //         (load8(point_.data() + 24) >> 12) & low_51_bit_mask
    //     };
    //     std::reverse(t.begin(), t.end());

    //     mpz_import(Y.get_mpz_t(), t.size(), 1, sizeof(t[0]), 0, 0, t.data());
    //     YY = Y*Y;
    //     auto tt = 0;
        
    // }
} // namespace solana