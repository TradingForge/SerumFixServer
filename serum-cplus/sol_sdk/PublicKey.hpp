#pragma once
#include <vector>
#include <list>
#include <string>
#include <string.h>
#include <base58/base58.h>
#include <tuple>

namespace solana
{
    #define PDA_MARKER "ProgramDerivedAddress"
    #define SIZE_PUBKEY 32

    class PublicKey 
    {
        typedef std::string string;
        typedef uint8_t byte;
        typedef std::vector<byte> bytes;


        string key_str;
        bytes key_b;

        bytes _from_base58(const string&);
        string _to_base58(const bytes&);
        static bytes try_find_public_key(const bytes program_id, byte seed_bump, const std::list<bytes>& seeds);
    public:
        PublicKey();
        PublicKey(const string&);
        PublicKey(const bytes&);
        PublicKey(const PublicKey&);
        PublicKey(PublicKey&&);
        PublicKey(const byte[SIZE_PUBKEY]);
    
        static std::tuple<PublicKey, byte> find_public_key(const std::list<PublicKey>& seeds, const PublicKey& program_id);
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