/*************************************************************************
	> File Name: 7.homework_ls_sorted_2.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年05月07日 星期二 20时18分20秒
 ************************************************************************/
#include <algorithm>

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

#define MAX_FILENAME_NUM 100
#define MAX_FILE_NUM 1000

void ls_print(char namestr[][MAX_FILE_NUM], int cnt) {
    int i =0;
    for (int i = 0; i < cnt; i++) {
        printf("%s\n", &namestr[MAX_FILENAME_NUM][i]);
    }

}

void ls_handle(char* wd) {
    struct stat* buf;
    buf = (struct stat* )malloc(sizeof(struct stat));
    DIR* dir = opendir(wd);

    if(dir == NULL) {
        perror("opendir");
        exit(1);
    }

    struct dirent* dirent;
    int cnt = 0;
    char namestr[MAX_FILENAME_NUM][MAX_FILE_NUM];
	while((dirent = readdir(dir)) != NULL)
    {
        char newwd[100];
        sprintf(newwd, "%s/%s", wd, dirent->d_name);
        stat(newwd,buf);
        if(stat(newwd,buf) < 0) {
            perror("stat");
        }
        sprintf(&namestr[MAX_FILENAME_NUM][cnt++], " %s\t", dirent->d_name);
        ls_print(namestr, cnt);
    }
    printf("\n");
}
int main(int argc, char* argv[]) {
    char* wd;
    if(argc == 1) {
        wd = getcwd(NULL, 0);
        ls_handle(wd);
    } else if(argc == 2) {
        wd = argv[1];
        ls_handle(wd);
    } else {
        for(int i = 1; i < argc; i++) {
            i == 1 || printf("\n");
            wd = argv[i];
            printf("%s:\n", wd);
            ls_handle(wd);
        }
    }
    return 0;
}
