#include <stdint.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

// Definitions
#define EXIT_SUCCESS 0 
#define EXIT_ERR_TOO_FEW_ARGUMENTS 1
#define EXIT_NOT_A_DIRECTORY 2

int main(int argc, char* argv[]) {
    if (argc != 2){
        fprintf(stderr, "Usage: %s [dir path]", argv[0]);
        exit(EXIT_ERR_TOO_FEW_ARGUMENTS);
    }

    struct stat buf;
    int r = stat(argv[1], &buf);
    if (!S_ISDIR(buf.st_mode)){
        fprintf(stderr, "Usage: %s [dir path]", argv[0]);
        exit(EXIT_NOT_A_DIRECTORY);
    }
}