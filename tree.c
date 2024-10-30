#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

// Definitions
#define EXIT_SUCCESS 0
#define EXIT_ERR_TOO_FEW_ARGUMENTS 1
#define EXIT_NOT_A_DIRECTORY 2

#define DEFAULT "\x1b[0m"
#define RED "\x1b[31;1m"
#define GREEN "\x1b[32;1m"
#define YELLOW "\x1b[33;1m"
#define BLUE "\x1b[34;1m"
#define CYAN "\x1b[36;1m"

void tree(char *path, int dashes, int spaces, int level)
{
    DIR *dp = opendir(path);
    struct dirent* direntptr;

    while ((direntptr = readdir(dp)) != NULL)
    {
        if ((strcmp(direntptr -> d_name, ".") == 0) || (strcmp(direntptr -> d_name, "..") == 0))
        {
            continue;
        }

        char *newpath = (char *) malloc( sizeof(path) + sizeof(direntptr -> d_name) + 2); // room for null terminate plus the / thats going to go string
        sprintf(newpath, "%s/%s", path, direntptr->d_name);
        printf("\n");

        if (spaces > 0)
        {
            for (int lvl = 0; lvl < level; lvl++)
            {
                printf("│");
                for (int sp = 0; sp < spaces; sp++)
                {
                    printf(" ");
                }
            }
        }
        printf("├");
        for (int place = 0; place < dashes; place++)
        {
            printf("─");
        }
        printf(" ");
        
        if ((direntptr->d_type == DT_DIR) && (access(newpath, R_OK) == 0))
        {
            printf("%s%s%s", BLUE, direntptr->d_name, DEFAULT);
            tree(newpath, dashes, spaces, level + 1);
            free(newpath);
            continue;
        }

        struct stat buf;
        int r = lstat(newpath, &buf);

        if (S_ISLNK(buf.st_mode))
        {
            printf("%s%s%s", CYAN, direntptr->d_name, DEFAULT);
            free(newpath);
            continue;
        }

        if (S_ISFIFO(buf.st_mode))
        {
            printf("%s%s%s", YELLOW, direntptr->d_name, DEFAULT);
            free(newpath);
            continue;
        }

        if (access(newpath, X_OK) == 0)
        {
            printf("%s%s%s", GREEN, direntptr->d_name, DEFAULT);
            free(newpath);
            continue;
        }

        if (access(newpath, R_OK) == -1)
        {
            printf("%s%s%s", RED, direntptr->d_name, DEFAULT);
            free(newpath);
            continue;
        }

        if (S_ISREG(buf.st_mode))
        {
            printf("%s", direntptr->d_name);
            free(newpath);
            continue;
        }
    }

    

    closedir(dp);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [dir path]\n", argv[0]);
        exit(EXIT_ERR_TOO_FEW_ARGUMENTS);
    }

    struct stat buf;
    int r = stat(argv[1], &buf);
    if (!S_ISDIR(buf.st_mode))
    {
        fprintf(stderr, "Usage: %s [dir path]\n", argv[0]);
        exit(EXIT_NOT_A_DIRECTORY);
    }

    // Realpath will malloc memory that we need to free
    char *path = realpath(argv[1], NULL);
    printf("%s%s%s", BLUE, path, DEFAULT);
    tree(path, 4, 5, 0);
    printf("\n");
    free(path);

    return EXIT_SUCCESS;
}