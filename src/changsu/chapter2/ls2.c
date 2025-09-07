#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>

void do_ls(char []);
void do_stat(char *);
void mode_to_letters(mode_t mode, char mode_str[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);
void show_file_info(char *, struct stat *);

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
    return 0;
}

void do_ls(char dirname[]) {
    DIR* dir_ptr = NULL;
    struct dirent *dirent_ptr = NULL;

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
    } else {
        while ((dirent_ptr = readdir(dir_ptr)) != NULL)
        {
            do_stat(dirent_ptr->d_name);
        }
        closedir(dir_ptr);
    }
}

void do_stat(char *filename) {
    struct stat buf;
    if (stat(filename, &buf) == -1) {
        perror(filename);
    } else {
        show_file_info(filename, &buf);
    }
}

void show_file_info(char *filename, struct stat *info_p) {
    char mode[10];

    mode_to_letters(info_p->st_mode, mode);

    printf("%s", mode);
    printf("%4d ", (int)info_p->st_nlink);
    printf("%-8s ", uid_to_name(info_p->st_uid));
    printf("%-8s ", gid_to_name(info_p->st_gid));
    printf("%8ld ", (long)info_p->st_size);
    printf("%.12s ", 4 + ctime(&info_p->st_mtime));
    printf("%s \n", filename);
}

void mode_to_letters(mode_t mode, char mode_str[]) {
    strcpy(mode_str, "----------");

    if (S_ISDIR(mode)) mode_str[0] = 'd';
    if (S_ISCHR(mode)) mode_str[0] = 'c';
    if (S_ISBLK(mode)) mode_str[0] = 'b';

    // permission
    if (mode & S_IRUSR) mode_str[1] = 'r';
    if (mode & S_IWUSR) mode_str[1] = 'w';
    if (mode & S_IXUSR) mode_str[1] = 'x';

    if (mode & S_IRGRP) mode_str[2] = 'r';
    if (mode & S_IWGRP) mode_str[2] = 'w';
    if (mode & S_IXGRP) mode_str[2] = 'x';

    if (mode & S_IROTH) mode_str[3] = 'r';
    if (mode & S_IWOTH) mode_str[3] = 'w';
    if (mode & S_IXOTH) mode_str[3] = 'x';
}

char *uid_to_name(uid_t uid) {
    struct passwd *pw_ptr = NULL;
    static char uid_str[10];
    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(uid_str, "%d", uid);
        return uid_str;
    } else {
        return pw_ptr->pw_name;
    }
}

char *gid_to_name(gid_t gid) {
    struct group *gp_ptr = NULL;
    static char gid_str[10];
    if ((gp_ptr = getgrgid(gid)) == NULL) {
        sprintf(gid_str, "%d", gid);
        return gid_str;
    } else {
        return gp_ptr->gr_name;
    }
}
