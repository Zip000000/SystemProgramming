/*************************************************************************
	> File Name: test.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年04月27日 星期六 19时13分40秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main() {
    char *path = ".";
    DIR *dir = opendir("./testdir/");
    struct dirent *dirent = (struct dirent *)malloc(sizeof(struct dirent));
    while ((dirent = readdir(dir)) != NULL) {
        printf("%s   ", dirent->d_name);
        printf("%u\n", dirent->d_type);
    }

    return 0;
}
