#include <iostream>
#include <string.h>
#include <experimental/filesystem>
#include <regex>
#include <list>
#include "GameFile.h"
#include "murmur.h"

namespace fs = std::experimental::filesystem;

static const std::string files[] = {"animations", "common", "gog", "localizations", "scenes", "sounds", "templates", "textures-s3", "videos", "voices"};

static const std::string decompressedOutputLocation = "./decompressed/";
static const std::string compressedOutputLocation = "./compressed/";

void decompress() {
    std::map<uint32_t, std::string> fileNames = {};

    fs::path fileMap("./filemap");

    if(fs::exists(fileMap) && fs::is_regular_file(fileMap)) {
        std::ifstream file(fileMap, std::ios::binary);

        char buf;
        uint32_t hash;
        std::string name;

        while(file.peek() != EOF) {
            name = "";
            file.read((char *) &hash, 4);

            while (file.read(&buf, 1)) {
                if (buf == '\n') {
                    break;
                }
                name += buf;
            }
            fileNames[hash] = name;
        }
    }

    for(const std::string &file : files) {
        GameFile gf(file, decompressedOutputLocation, fileNames);
        gf.readIndices();
        gf.extractFiles();
    }
}

void compress() {
    // 0xDEADBEEF will be overwritten later with the amount of files
    Byte indexVersion[11]={0x00, 0x06, 0x01, 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x00, 0x00};
    Byte isCompressed = 0x00;

    // Location were the containers are located
    fs::path decompressedFilesPath(decompressedOutputLocation.c_str());

    if(!fs::exists(decompressedFilesPath) || !fs::is_directory(decompressedFilesPath)) {
        printf("'%s' does not exist or is not a directory", decompressedOutputLocation.c_str());
        exit(1);
    }

    // Location for the resulting .idx and .dat files
    fs::path outputLocation(compressedOutputLocation.c_str());
    fs::create_directories(outputLocation);

    // Checks whether the filename consists of only numbers
    std::regex isUnnamedFile("^[0-9]+$", std::regex_constants::ECMAScript | std::regex_constants::icase);

    // Loop all containers
    for(auto& root: fs::directory_iterator(decompressedFilesPath)) {
        std::string containerName = root.path().string().substr(decompressedFilesPath.string().length(), std::string::npos);

        printf("Creating container: %s\n", containerName.c_str());

        // We'll collect all FileIndex structs and sort them before building our .idx file
        std::list<FileIndex> indices;

        std::ofstream outputIndexFile(outputLocation / (containerName + ".idx"), std::ios::binary);
        outputIndexFile.write((char*) indexVersion, 11);

        std::ofstream outputDataFile(outputLocation / (containerName + ".dat"), std::ios::binary);

        uint32_t offset = 0;

        // Loop all files inside the container
        for(auto& innerPath: fs::recursive_directory_iterator(root)) {
            if(fs::is_directory(innerPath)) {
                continue;
            }
            std::string pathToHash = innerPath.path().string().substr(root.path().string().length()+1, std::string::npos);

            printf("Added file: %s\n", pathToHash.c_str());

            std::ifstream inputFile(innerPath.path(), std::ios::binary);
            uint32_t inputFileSize = 0;
            char c;
            while(inputFile.peek() != EOF) {
                inputFile.read(&c, 1);
                outputDataFile.write(&c, 1);
                inputFileSize++;
            }

            uint32_t hash;

            // In theory this could fail with files which
            // actually have a numeric name,
            // no extension and are located
            // in the root directory
            if(std::regex_search(pathToHash, isUnnamedFile)) {
                hash = (uint32_t) std::stoul(pathToHash, nullptr, 10);
            } else {
                hash = murmurHash(pathToHash.c_str(), (int) pathToHash.length(), 0);
            }

            FileIndex fi;
            fi.hash = hash;
            fi.inflatedSize = inputFileSize;
            fi.size = inputFileSize;
            fi.offset = offset;
            fi.isCompressed = isCompressed;

            indices.push_back(fi);

            offset += inputFileSize;
        }

        indices.sort([](const FileIndex& first, const FileIndex& second) -> bool {return first.hash < second.hash;});

        for(FileIndex &fi : indices) {
            outputIndexFile.write((char*) &fi.hash, 4);
            outputIndexFile.write((char*) &fi.size, 4);
            outputIndexFile.write((char*) &fi.size, 4);
            outputIndexFile.write((char*) &fi.offset, 4);
            outputIndexFile.write((char*) &fi.isCompressed, 1);
        }

        outputIndexFile.seekp(3);
        auto size = (uint32_t) indices.size();
        outputIndexFile.write((char*) &size, 4);
    }
}

void usage() {
    printf("TWoM (Un)packer\n");
    printf("  -d : decompress files\n");
    printf("  -c : compress file\n");
    printf("\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "-d") == 0) {
        decompress();
    } else if(argc == 2 && strcmp(argv[1], "-c") == 0) {
        compress();
    } else {
        usage();
    }
    return 0;
}