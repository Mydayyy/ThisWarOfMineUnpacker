//
// Created by mydayyy on 8/12/18.
//
#include "GameFile.h"

GameFile::GameFile(std::string fileName, std::string dest, std::map<uint32_t, std::string> &fileNames) {
    this->fileName = fileName;
    this->indexFile = fileName + ".idx";
    this->dataFile = fileName + ".dat";
    this->dest = dest;

    this->fileNames = fileNames;
}

bool GameFile::readIndices() {
    std::ifstream file(this->indexFile, std::ios::binary);

    if(file.fail()) {
        printf("File '%s' not found\n", this->indexFile.c_str());
        return false;
    }

    file.seekg(11);

    while(file.peek() != EOF) {
        FileIndex fi;
        fi.hash = this->read32LE(&file);
        fi.size = this->read32LE(&file);
        fi.inflatedSize = this->read32LE(&file);
        fi.offset = this->read32LE(&file);
        fi.isCompressed = this->read8(&file);

        this->indices[fi.hash] = fi;
    }

    return true;
}

bool GameFile::extractFiles() {
    this->createFolders();

    fs::path dest = fs::path(this->dest) / fs::path(this->fileName);

    std::ifstream dataFile(this->dataFile, std::ios::binary);
    if(dataFile.fail()) {
        printf("File '%s' not found\n", this->dataFile.c_str());
        return false;
    }

    printf("Extracting '%s'\n", this->dataFile.c_str());

    uint32_t chunkSize = 1024*16;
    unsigned char buff[chunkSize];


    for (auto &indice : this->indices) {
        FileIndex fi = indice.second;
        uint32_t remaining = fi.size;

        fs::path resultPath = dest;


        if(this->fileNames.count(fi.hash)) {
            resultPath /= fs::path(this->fileNames[fi.hash]);
            printf("Extracted file: %s\n", this->fileNames[fi.hash].c_str());
        }  else {
            resultPath /= fs::path(std::to_string(fi.hash));
            printf("Extracted file: %s\n", std::to_string(fi.hash).c_str());
        }

        fs::create_directories(fs::path(resultPath).remove_filename());

        std::ofstream output(resultPath, std::ios::binary);

        dataFile.seekg(fi.offset);

        Z z(&output, chunkSize);
        int ret;
        while(remaining) {
            dataFile.read((char*) buff, std::min(chunkSize, remaining));

            if(!fi.isCompressed) {
                output.write((char*) buff, std::min(chunkSize, remaining));
                remaining -= std::min(chunkSize, remaining);
                continue;
            }

            ret = z.inf(buff, std::min(chunkSize, remaining));

            if(ret < 0 && ret != Z_BUF_ERROR) {
                printf("Zlib returned error: %i\n", ret);
                printf("Files may be incomplete\n");
                return false;
            }
            remaining -= std::min(chunkSize, remaining);
        }
    }

    printf("\r");

    return true;
}

void GameFile::createFolders() {
    fs::path dest = fs::path(this->dest);

    if (!fs::is_directory(dest) || !fs::exists(dest)) {
        fs::create_directory(dest);
    }

    dest /= fs::path(this->fileName);

    if (!fs::is_directory(dest) || !fs::exists(dest)) {
        fs::create_directory(dest);
    }
}

uint32_t GameFile::read32LE(std::ifstream *file) {
    char b0, b1, b2, b3;

    file->read(&b3, 1);
    file->read(&b2, 1);
    file->read(&b1, 1);
    file->read(&b0, 1);

    uint32_t num = (((uint32_t) (unsigned char) b0) << 24) | (((uint32_t) (unsigned char) b1) << 16) | (((uint32_t) (unsigned char) b2) << 8) | (unsigned char) b3;

    return num;
}

uint8_t GameFile::read8(std::ifstream *file) {
    char b0;
    file->read(&b0, 1);
    auto num = (uint8_t) b0;
    return num;
}
