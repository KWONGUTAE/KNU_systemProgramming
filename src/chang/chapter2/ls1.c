#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void do_ls(char[]);
int main(int ac, char *av[]) {
    if (ac == 1) {
        do_ls(".");
    } else {
        while (--ac) 
        {
            printf("%s: \n", *++av);
            do_ls(*av);
        }
    }
}

void do_ls(char dirname[]) {
    DIR *dir_ptr = NULL;
    struct dirent *dirent_ptr = NULL;

    // opendir(dirname) return -> DIR* first entry
    // readdir(dir_ptr) return -> pointer to a struct dirent
    if ((dir_ptr = opendir(dirname)) != NULL) {
        while ((dirent_ptr = readdir(dir_ptr)) != NULL) 
        {
            printf("%s\n", dirent_ptr->d_name);
        }
        closedir(dir_ptr);
    }
}