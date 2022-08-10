#pragma once
#include <vector>
#include <string>
#include "PublicKey.hpp"

#define SIZE_KEYPAIR 64

class Keypair
{
    typedef std::string string;
    typedef uint8_t byte;
    typedef std::vector<byte> bytes;

    string key_str;
    bytes key_b;

    bytes _from_base58(const string&);
public:
    Keypair();
    Keypair(const string&);
    Keypair(const bytes&);
    Keypair(const Keypair&);
    Keypair(Keypair&&);
  

    void from_base58(const string&);
    PublicKey get_pubkey();
    const uint8_t* data() {return key_b.data();}
    size_t size() {return key_b.size();}

    friend bool operator==(const Keypair&, const Keypair&);
    friend bool operator==(const Keypair&, const string&);
    Keypair& operator=(const Keypair&);
    Keypair& operator= (Keypair &&key);
    ~Keypair();
};