## This War of Mine (Un)packer

This application can unpack `This War of Mine` gamefiles.
It needs to be in the same directory as the games container files
(common, animation, sounds, ...)

It will default to decompress / compress all container files.

Decompression will take the container from the working directory and
place the output inside a folder named `decompressed`

Compression will take the decompressed files from the folder
`decompressed` and place the recreated container files
into `compressed`.

You can modify the defaults inside main.cpp and recompile.

You can modify the unpacked files and even add new files (e.g lua scripts).

Due to the the architecture of the compressed files, we are not able
to recover filenames. The compressed files only store a hash of the filename.
Therefore, all decompressed files will have the name of the corresponding hash.

The workaround for this is to log all file accesses of the game and save
the hash and corresponding filename into a map, which the unpacker
can use to translate the hash to a filename.

Obviously, this will only translate filenames which were accessed by the game
during the recording. A basic filemap is included, but it is far away
from having all files.

The filemap needs to be in the same directory as the executable.

### Compilation

`cmake . && make`

The application uses the experimental c++ filesystem API.

### Usage

```
void usage() {
    printf("TWoM (Un)packer\n");
    printf("  -d : decompress files\n");
    printf("  -c : compress file\n");
    printf("\n");
    exit(EXIT_SUCCESS);
}
```


### License

http://unlicense.org/