#pragma once

#include <vector>
#include <string>
#include <tuple>


class FieldElement51
{
    typedef uint64_t word;
    typedef __uint128_t dword;
    typedef std::vector<uint64_t> words;
    typedef uint8_t byte;
    typedef std::vector<byte> bytes;

private:
    words element;

    static FieldElement51 reduce(words);

public:
    FieldElement51();
    FieldElement51(words);
    static FieldElement51 one();
    static FieldElement51 from_bytes(const bytes&);
    bytes to_bytes() const;

    FieldElement51 square() const;
    FieldElement51 pow2k(uint64_t) const;
    FieldElement51 pow_p58() const;
    std::tuple<FieldElement51,FieldElement51> pow22501() const;
    FieldElement51 add(const FieldElement51&) const;
    FieldElement51 sub(const FieldElement51&) const;
    FieldElement51 mul(const FieldElement51&) const;
    FieldElement51 negate() const ;

    FieldElement51& operator=(const FieldElement51&);
};