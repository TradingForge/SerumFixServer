#pragma once
#include <vector>
#include <list>
#include <string>
#include <string.h>
#include <base58/base58.h>
#include <tuple>
#include <cryptopp/sha.h>

// #include <gmpxx.h>

namespace solana
{
    #define SIZE_PUBKEY 32

    class PublicKey 
    {
    public:
        typedef uint8_t byte;
        typedef std::vector<byte> bytes;
    private:
        typedef std::string string;

        string key_str;
        bytes key_b;

        bytes _from_base58(const string&);
        string _to_base58(const bytes&);
        static bytes _try_find_public_key(const bytes&, const std::list<bytes>&);
        static bool _bytes_are_curve_point(const std::vector<byte>&);
        // static string _decompress_point(const bytes&);
    public:
        PublicKey();
        PublicKey(const string&);
        PublicKey(const bytes&);
        PublicKey(const PublicKey&);
        PublicKey(PublicKey&&);
        PublicKey(const byte[SIZE_PUBKEY]);
    
        static std::tuple<PublicKey, byte> find_public_key(const std::list<PublicKey>&, const PublicKey&);
        static PublicKey create_program_address(const std::list<bytes>&, const PublicKey&);
        void from_base58(const string&);
        const uint8_t* data() const {return key_b.data();}
        size_t size() const {return key_b.size();}
        string get_str_key() const {return key_str;}

        friend bool operator==(const PublicKey&, const PublicKey&);
        friend bool operator==(const PublicKey&, const string&);
        PublicKey& operator=(const PublicKey&);
        PublicKey& operator=(PublicKey&&);
        ~PublicKey();
    };
} // namespace solana