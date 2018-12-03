//
// Created by mydayyy on 8/12/18.
//

#ifndef THISWAROFMINEUNPACKER_GAMEFILE_H
#define THISWAROFMINEUNPACKER_GAMEFILE_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <experimental/filesystem>
#include "Z.h"

namespace fs = std::experimental::filesystem;

struct FileIndex {
    FileIndex() : hash(0), size(0), inflatedSize(0), offset(0), isCompressed(0) {}
    uint32_t hash;
    uint32_t size;
    uint32_t inflatedSize;
    uint32_t offset;
    uint8_t isCompressed;
};

class GameFile {
public:
    GameFile(std::string fileName, std::string dest, std::map<uint32_t, std::string> &fileNames);

    bool readIndices();
    bool extractFiles();


private:
    void createFolders();

    uint32_t read32LE(std::ifstream *file);
    uint8_t read8(std::ifstream *file);

    std::string fileName;
    std::string indexFile;
    std::string dataFile;
    std::string dest;

    std::map<uint32_t, std::string> fileNames;
    std::map<uint32_t, FileIndex> indices;
};


#endif //THISWAROFMINEUNPACKER_GAMEFILE_H
