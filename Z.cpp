//
// Created by mydayyy on 8/12/18.
//

#include "Z.h"

Z::Z(std::ofstream *output, uInt chunkSize) {
    this->output = output;
    this->out = new unsigned char[chunkSize];
    this->chunkSize = chunkSize;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    inflateInit2(&strm, 31);
}

Z::~Z() {
    delete[] this->out;
}

int Z::inf(unsigned char *in, uInt inSize) {
    int ret;
    strm.avail_in = inSize;
    strm.next_in = in;

    do {
        strm.avail_out = this->chunkSize;
        strm.next_out = this->out;
        ret = inflate(&strm, Z_SYNC_FLUSH);
        output->write((char*) out, this->chunkSize - strm.avail_out);
    } while(strm.avail_out == 0);

    return ret;
}