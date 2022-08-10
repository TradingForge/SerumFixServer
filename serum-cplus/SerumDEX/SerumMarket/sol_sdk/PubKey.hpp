#pragma once
#include <vector>
#include <string>

#define SIZE_PUBKEY 32

class PubKey 
{
    typedef std::string string;
    typedef uint8_t byte;
    typedef std::vector<byte> bytes;


    string key_str;
    bytes key_b;

    bytes _from_base58(const string&);
public:
    PubKey();
    PubKey(const string&);
    PubKey(const bytes&);
    PubKey(const PubKey&);
    PubKey(PubKey&&);
  

    void from_base58(const string&);
    const uint8_t* data() {return key_b.data();}
    size_t size() {return key_b.size();}

    friend bool operator==(const PubKey&, const PubKey&);
    friend bool operator==(const PubKey&, const string&);
    PubKey& operator=(const PubKey&);
    PubKey& operator=(PubKey&&);
    ~PubKey();
};