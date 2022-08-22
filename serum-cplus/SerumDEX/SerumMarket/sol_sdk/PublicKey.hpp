#pragma once
#include <vector>
#include <string>

#define SIZE_PUBKEY 32

class PublicKey 
{
    typedef std::string string;
    typedef uint8_t byte;
    typedef std::vector<byte> bytes;


    string key_str;
    bytes key_b;

    bytes _from_base58(const string&);
public:
    PublicKey();
    PublicKey(const string&);
    PublicKey(const bytes&);
    PublicKey(const PublicKey&);
    PublicKey(PublicKey&&);
  

    void from_base58(const string&);
    const uint8_t* data() const {return key_b.data();}
    size_t size() const {return key_b.size();}

    friend bool operator==(const PublicKey&, const PublicKey&);
    friend bool operator==(const PublicKey&, const string&);
    PublicKey& operator=(const PublicKey&);
    PublicKey& operator=(PublicKey&&);
    ~PublicKey();
};