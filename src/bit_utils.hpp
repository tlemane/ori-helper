#include <cstdint>
#include <tuple>
#include <emmintrin.h>
#include <immintrin.h>

using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;


// from HowDeSBT
static const u8 poplook8[256] =
    {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

#define least_significant(type, numBits) ((((type)1) << (numBits)) - 1)

u64 bitwise_and_count(const void *bits1,
                      const void *bits2,
                      const u64 numBits)
{
    u8 *scan1 = (u8 *)bits1;
    u8 *scan2 = (u8 *)bits2;
    u64 n;
    u64 numOnes = 0;

    for (n = numBits; n >= 8; n -= 8)
        numOnes += poplook8[*(scan1++) & *(scan2++)];

    if (n == 0)
        return numOnes;

    u8 mask = least_significant(u8, n);
    numOnes += poplook8[(*scan1 & *scan2) & mask];

    return numOnes;
}

u64 bitwise_count(const void *bits,
                  const u64 numBits)
{
    u8 *scan = (u8 *)bits;
    u64 n;
    u64 numOnes = 0;

    for (n = numBits; n >= 8; n -= 8)
        numOnes += poplook8[*(scan++)];

    if (n == 0)
        return numOnes;

    u8 mask = least_significant(u8, n);
    numOnes += poplook8[(*scan) & mask];

    return numOnes;
}

inline std::tuple<double, double> common_bits(u8 *x, u8 *y, size_t nb_bits)
{
    u64 popx = bitwise_count(x, nb_bits);
    u64 popy = bitwise_count(y, nb_bits);
    u64 popxy = bitwise_and_count(x, y, nb_bits);
    std::cerr << popx << " " << popy << " " << popxy << std::endl;
    return std::make_tuple(popxy / static_cast<double>(popx), popxy / static_cast<double>(popy));
}