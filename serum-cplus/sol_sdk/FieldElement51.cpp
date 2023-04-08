#include "FieldElement51.hpp"

FieldElement51::FieldElement51(): element(5,0) 
{}

FieldElement51::FieldElement51(words number): element(number)
{
    if (element.size() != 5)
        throw -1;
}

FieldElement51 FieldElement51::one() 
{
    return FieldElement51({1, 0, 0, 0, 0});
}

FieldElement51 FieldElement51::from_bytes(const bytes& b)
{
    if (b.size() != 32)
        throw -1;

    auto load8 = [](const byte* input) {
        return (static_cast<uint64_t>(input[0]))
            | (static_cast<uint64_t>(input[1]) << 8)
            | (static_cast<uint64_t>(input[2]) << 16)
            | (static_cast<uint64_t>(input[3]) << 24)
            | (static_cast<uint64_t>(input[4]) << 32)
            | (static_cast<uint64_t>(input[5]) << 40)
            | (static_cast<uint64_t>(input[6]) << 48)
            | (static_cast<uint64_t>(input[7]) << 56);
    };

    constexpr word low_51_bit_mask = (static_cast<word>(1) << 51) - 1;

    return FieldElement51({
        load8(b.data()) & low_51_bit_mask,
        (load8(b.data() + 6) >> 3) & low_51_bit_mask,
        (load8(b.data() + 12) >> 6) & low_51_bit_mask,
        (load8(b.data() + 19) >> 1) & low_51_bit_mask,
        (load8(b.data() + 24) >> 12) & low_51_bit_mask
    });
}

FieldElement51::bytes FieldElement51::to_bytes() const
{
    auto limbs = FieldElement51::reduce(element).element;

    word q = (limbs[0] + 19) >> 51;
    q = (limbs[1] + q) >> 51;
    q = (limbs[2] + q) >> 51;
    q = (limbs[3] + q) >> 51;
    q = (limbs[4] + q) >> 51;

    limbs[0] += 19*q;

    constexpr word low_51_bit_mask = ( static_cast<word>(1) << 51) - 1;
    limbs[1] +=  limbs[0] >> 51;
    limbs[0] = limbs[0] & low_51_bit_mask;
    limbs[2] +=  limbs[1] >> 51;
    limbs[1] = limbs[1] & low_51_bit_mask;
    limbs[3] +=  limbs[2] >> 51;
    limbs[2] = limbs[2] & low_51_bit_mask;
    limbs[4] +=  limbs[3] >> 51;
    limbs[3] = limbs[3] & low_51_bit_mask;

    limbs[4] = limbs[4] & low_51_bit_mask;

    bytes s(32,0);
    s[ 0] =   static_cast<byte>(limbs[0]);
    s[ 1] =   static_cast<byte>(limbs[0] >>  8);
    s[ 2] =   static_cast<byte>(limbs[0] >> 16);
    s[ 3] =   static_cast<byte>(limbs[0] >> 24);
    s[ 4] =   static_cast<byte>(limbs[0] >> 32);
    s[ 5] =   static_cast<byte>(limbs[0] >> 40);
    s[ 6] =   static_cast<byte>((limbs[0] >> 48) | (limbs[1] << 3));
    s[ 7] =   static_cast<byte>(limbs[1] >>  5);
    s[ 8] =   static_cast<byte>(limbs[1] >> 13);
    s[ 9] =   static_cast<byte>(limbs[1] >> 21);
    s[10] =   static_cast<byte>(limbs[1] >> 29);
    s[11] =   static_cast<byte>(limbs[1] >> 37);
    s[12] =   static_cast<byte>((limbs[1] >> 45) | (limbs[2] << 6));
    s[13] =   static_cast<byte>(limbs[2] >>  2);
    s[14] =   static_cast<byte>(limbs[2] >> 10);
    s[15] =   static_cast<byte>(limbs[2] >> 18);
    s[16] =   static_cast<byte>(limbs[2] >> 26);
    s[17] =   static_cast<byte>(limbs[2] >> 34);
    s[18] =   static_cast<byte>(limbs[2] >> 42);
    s[19] =   static_cast<byte>((limbs[2] >> 50) | (limbs[3] << 1));
    s[20] =   static_cast<byte>(limbs[3] >>  7);
    s[21] =   static_cast<byte>(limbs[3] >> 15);
    s[22] =   static_cast<byte>(limbs[3] >> 23);
    s[23] =   static_cast<byte>(limbs[3] >> 31);
    s[24] =   static_cast<byte>(limbs[3] >> 39);
    s[25] =   static_cast<byte>((limbs[3] >> 47) | (limbs[4] << 4));
    s[26] =   static_cast<byte>(limbs[4] >>  4);
    s[27] =   static_cast<byte>(limbs[4] >> 12);
    s[28] =   static_cast<byte>(limbs[4] >> 20);
    s[29] =   static_cast<byte>(limbs[4] >> 28);
    s[30] =   static_cast<byte>(limbs[4] >> 36);
    s[31] =   static_cast<byte>(limbs[4] >> 44);

    return s;
}

FieldElement51 FieldElement51::pow2k(uint64_t k) const
{
    if (k < 1)
        throw -1;

    auto m = [](word a, word b){return static_cast<dword>(a) * static_cast<dword>(b);};
    words a = element;

    while(true){
        auto a3_19 = 19 * a[3];
        auto a4_19 = 19 * a[4];

        auto c0 = m(a[0],  a[0]) + 2*( m(a[1], a4_19) + m(a[2], a3_19) );
        auto c1 = m(a[3], a3_19) + 2*( m(a[0],  a[1]) + m(a[2], a4_19) );
        auto c2 = m(a[1],  a[1]) + 2*( m(a[0],  a[2]) + m(a[4], a3_19) );
        auto c3 = m(a[4], a4_19) + 2*( m(a[0],  a[3]) + m(a[1],  a[2]) );
        auto c4 = m(a[2],  a[2]) + 2*( m(a[0],  a[4]) + m(a[1],  a[3]) );

        constexpr word LOW_51_BIT_MASK = (static_cast<word>(1) << 51) - 1;

        c1 += c0 >> 51;
        a[0] = c0 & LOW_51_BIT_MASK;

        c2 += c1 >> 51;
        a[1] = c1 & LOW_51_BIT_MASK;

        c3 += c2 >> 51;
        a[2] = c2 & LOW_51_BIT_MASK;

        c4 += c3 >> 51;
        a[3] = c3 & LOW_51_BIT_MASK;

        auto carry = static_cast<word>(c4 >> 51);
        a[4] = c4 & LOW_51_BIT_MASK;

        a[0] = a[0] + carry * 19;

        a[1] += a[0] >> 51;
        a[0] &= LOW_51_BIT_MASK;

        if (--k == 0) {
            break;
        }
    }

    return FieldElement51(a);
}

FieldElement51 FieldElement51::square() const
{
    return pow2k(1);
}

FieldElement51 FieldElement51::pow_p58() const
{
    auto t19 = std::get<0>(this->pow22501());
    auto t20 = t19.pow2k(2);

    // return t21
    return this->mul(t20);
}

std::tuple<FieldElement51,FieldElement51> FieldElement51::pow22501() const
{
    auto t0  = this->square();
    auto t1  = t0.square().square();
    auto t2  = this->mul(t1);
    auto t3  = t0.mul(t2);
    auto t4  = t3.square();
    auto t5  = t2.mul(t4);
    auto t6  = t5.pow2k(5);
    auto t7  = t6.mul(t5);
    auto t8  = t7.pow2k(10);
    auto t9  = t8.mul(t7);
    auto t10 = t9.pow2k(20);
    auto t11 = t10.mul(t9);
    auto t12 = t11.pow2k(10);
    auto t13 = t12.mul(t7);
    auto t14 = t13.pow2k(50);
    auto t15 = t14.mul(t13);
    auto t16 = t15.pow2k(100);
    auto t17 = t16.mul(t15);
    auto t18 = t17.pow2k(50);
    auto t19 = t18.mul(t13);

    return std::tuple<FieldElement51, FieldElement51>(t19, t3);
}

FieldElement51 FieldElement51::add(const FieldElement51& other) const
{
    words res(5, 0);
    res[0] = element[0] + other.element[0];
    res[1] = element[1] + other.element[1];
    res[2] = element[2] + other.element[2];
    res[3] = element[3] + other.element[3];
    res[4] = element[4] + other.element[4];

    return FieldElement51(res);
}

FieldElement51 FieldElement51::sub(const FieldElement51& other) const
{
    return FieldElement51(reduce({
        (element[0] + 0x7FFFFFFFFFFED0) - other.element[0],
        (element[1] + 0x7FFFFFFFFFFFF0) - other.element[1],
        (element[2] + 0x7FFFFFFFFFFFF0) - other.element[2],
        (element[3] + 0x7FFFFFFFFFFFF0) - other.element[3],
        (element[4] + 0x7FFFFFFFFFFFF0) - other.element[4],
    }));
}

FieldElement51 FieldElement51::negate() const
{
    return reduce({
        0x7FFFFFFFFFFED0 - element[0],
        0x7FFFFFFFFFFFF0 - element[1],
        0x7FFFFFFFFFFFF0 - element[2],
        0x7FFFFFFFFFFFF0 - element[3],
        0x7FFFFFFFFFFFF0 - element[4]
    });
}

FieldElement51 FieldElement51::reduce(words limbs)
{
    if (limbs.size() != 5) 
        throw -1;

    constexpr word LOW_51_BIT_MASK = (static_cast<word>(1) << 51) - 1;

    auto c0 = limbs[0] >> 51;
    auto c1 = limbs[1] >> 51;
    auto c2 = limbs[2] >> 51;
    auto c3 = limbs[3] >> 51;
    auto c4 = limbs[4] >> 51;

    limbs[0] &= LOW_51_BIT_MASK;
    limbs[1] &= LOW_51_BIT_MASK;
    limbs[2] &= LOW_51_BIT_MASK;
    limbs[3] &= LOW_51_BIT_MASK;
    limbs[4] &= LOW_51_BIT_MASK;

    limbs[0] += c4 * 19;
    limbs[1] += c0;
    limbs[2] += c1;
    limbs[3] += c2;
    limbs[4] += c3;

    return FieldElement51(limbs);
}

FieldElement51 FieldElement51::mul(const FieldElement51& other) const
{
    auto m = [](word a, word b){return static_cast<dword>(a) * static_cast<dword>(b);};

    auto a = element;
    auto b = other.element;

    auto b1_19 = b[1] * 19;
    auto b2_19 = b[2] * 19;
    auto b3_19 = b[3] * 19;
    auto b4_19 = b[4] * 19;

    auto c0 = m(a[0],b[0]) + m(a[4],b1_19) + m(a[3],b2_19) + m(a[2],b3_19) + m(a[1],b4_19);
    auto c1 = m(a[1],b[0]) + m(a[0],b[1])  + m(a[4],b2_19) + m(a[3],b3_19) + m(a[2],b4_19);
    auto c2 = m(a[2],b[0]) + m(a[1],b[1])  + m(a[0],b[2])  + m(a[4],b3_19) + m(a[3],b4_19);
    auto c3 = m(a[3],b[0]) + m(a[2],b[1])  + m(a[1],b[2])  + m(a[0],b[3])  + m(a[4],b4_19);
    auto c4 = m(a[4],b[0]) + m(a[3],b[1])  + m(a[2],b[2])  + m(a[1],b[3])  + m(a[0],b[4]);

    constexpr word LOW_51_BIT_MASK = (static_cast<word>(1) << 51) - 1;
    words out (5, 0);

    c1 += c0 >> 51;
    out[0] = c0 & LOW_51_BIT_MASK;

    c2 += c1 >> 51;
    out[1] = c1 & LOW_51_BIT_MASK;

    c3 += c2 >> 51;
    out[2] = c2 & LOW_51_BIT_MASK;

    c4 += c3 >> 51;
    out[3] = c3 & LOW_51_BIT_MASK;

    word carry = c4 >> 51;
    out[4] = c4 & LOW_51_BIT_MASK;

    out[0] = out[0] + carry * 19;

    out[1] += out[0] >> 51;
    out[0] &= LOW_51_BIT_MASK;

    return FieldElement51(out);
}

FieldElement51& FieldElement51::operator= (const FieldElement51 &other)
{
    if (this == &other)
        return *this;

    element = other.element;
    return *this;
}