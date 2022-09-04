#pragma once
#include <vector>
#include <string>
#include "PublicKey.hpp"

namespace solana
{
    #define SIZE_KEYPAIR 64

    class Keypair
    {
        typedef std::string string;
        typedef uint8_t byte;
        typedef std::vector<byte> bytes;

        string key_str;
        bytes key_b;

        bytes _from_base58(const string&);
        string _to_base58(const bytes&);
    public:
        Keypair();
        Keypair(const string&);
        Keypair(const bytes&);
        Keypair(const Keypair&);
        Keypair(Keypair&&);
        Keypair(const byte[SIZE_KEYPAIR]);
    

        void from_base58(const string&);
        PublicKey get_pubkey() const;
        const uint8_t* data() const  {return key_b.data();}
        size_t size() const {return key_b.size();}

        friend bool operator==(const Keypair&, const Keypair&);
        friend bool operator==(const Keypair&, const string&);
        Keypair& operator=(const Keypair&);
        Keypair& operator= (Keypair &&key);
        ~Keypair();
    };
} // namespace solana