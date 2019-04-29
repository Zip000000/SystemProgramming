/*************************************************************************
	> File Name: 5.homework_ll.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年04月27日 星期六 18时49分02秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

char* print_mode(struct stat* buf, char* str)
{
    int mode = buf->st_mode;
    strcpy(str, "----------");
    if (S_ISDIR(mode)) { 
        str[0] = 'd';
    }
    if (S_ISCHR(mode)) {
        str[0] = 'c';
    }
     if (S_ISBLK(mode)) {
        str[0] = 'b';
    }
     if ((mode & S_IRUSR)) {
        str[1] = 'r';
    }
     if ((mode & S_IWUSR)) {
        str[2] = 'w';
    }
     if ((mode & S_IXUSR)) {
        str[3] = 'x';
    }
     if ((mode & S_IRGRP)) {
        str[4] = 'r';
    }
     if ((mode & S_IWGRP)) {
        str[5] = 'w';
    }
     if ((mode & S_IXGRP)) {
        str[6] = 'x';
    }
     if ((mode & S_IROTH)) {
        str[7] = 'r';
    }
     if ((mode & S_IWOTH)) {
        str[8] = 'w';
    }
     if ((mode & S_IXOTH)) {
        str[9] = 'x';
    }
    return str;
}

void print(struct stat* buf, struct dirent* dirent) {
    //print_mode(buf);
	char str[100];
    printf("%s", print_mode(buf, str));
    printf(" %ld" , buf->st_nlink);
    printf(" %-8s", getpwuid(buf->st_uid)->pw_name);
    printf(" %-8s", getgrgid(buf->st_gid)->gr_name);
    printf(" %8ld" , buf->st_size);
    printf(" %.12s", 4 + ctime(&buf->st_mtime));
    printf(" %s\t", dirent->d_name);
}

int main() {
    struct stat* buf;
    buf = (struct stat* )malloc(sizeof(struct stat));
    char wd[100];
    strcpy(wd, "/home/zip/haizei/3.SystemProgramming/Homework/");
    DIR* dir = opendir(wd);
    struct dirent* dirent;
    
	while((dirent = readdir(dir)) != NULL)
    {
        char newwd[100];
        sprintf(newwd, "%s/%s", wd, dirent->d_name);
        stat(newwd,buf);
        print(buf, dirent);
        printf("\n");

    }
    return 0;
}
