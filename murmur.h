//
// Created by mydayyy on 8/13/18.
//

#ifndef THISWAROFMINEUNPACKER_MURMUR_H
#define THISWAROFMINEUNPACKER_MURMUR_H

#include <cstdint>

uint32_t murmurHash ( const void * key, int len, uint32_t seed )
{
    // 'm' and 'r' are mixing constants generated offline.
    // They're not really 'magic', they just happen to work well.

    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    // Initialize the hash to a 'random' value

    uint32_t h = seed ^ len;

    // Mix 4 bytes at a time into the hash

    auto * data = (const unsigned char *)key;

    while(len >= 4)
    {
        uint32_t k = *(uint32_t*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    // Handle the last few bytes of the input array

    switch(len)
    {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
            h *= m;
        default:break;
    };

    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

#endif //THISWAROFMINEUNPACKER_MURMUR_H
