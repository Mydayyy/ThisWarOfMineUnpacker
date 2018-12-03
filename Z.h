//
// Created by mydayyy on 8/12/18.
//

#ifndef THISWAROFMINEUNPACKER_Z_H
#define THISWAROFMINEUNPACKER_Z_H

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <zlib.h>

class Z {
public:
    Z(std::ofstream *output, uInt chunkSize);
    ~Z();

    int inf(unsigned char *in, uInt inSize);
private:
    z_stream strm;

    uInt chunkSize;
    unsigned char *out;
    std::ofstream *output;
};


#endif //THISWAROFMINEUNPACKER_Z_H
