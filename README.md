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