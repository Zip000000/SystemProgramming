/*************************************************************************
	> File Name: 8.homework_ls_sorted.cpp
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年05月08日 星期三 12时27分09秒
 ************************************************************************/

#include<iostream>
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
#include<sys/ioctl.h>
#include<termios.h>
using namespace std;

#define MAX_FILENUM 1000    //文件数
#define MAX_FILEWORDS_NUM 100     //文件名长度

#define LSFLAG 2
#define LLFLAG 4

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

void ll_print(struct stat* buf, struct dirent* dirent) {
    //print_mode(buf);
	char str[100];
    printf("%s", print_mode(buf, str));
    printf(" %4ld" , buf->st_nlink);
    printf(" %-8s", getpwuid(buf->st_uid)->pw_name);
    printf(" %-8s", getgrgid(buf->st_gid)->gr_name);
    printf(" %8ld" , buf->st_size);
    printf(" %.12s", 4 + ctime(&buf->st_mtime));
    printf(" %s\t", dirent->d_name);
}

int get_col() {
   struct winsize size;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);   //取得宽度  需要再研究一下
	return size.ws_col;
}
int get_maxwords(char filename[][MAX_FILEWORDS_NUM], int a, int b)  {
    //在从 第一个参数开始 的 n 个文件中 找出最长名字的长度
    int max = 0;
	for (int i = a; i < b; i++) {
        int temp = strlen(&filename[i][0]); 
        max = max > temp ? max : temp; 
    }

    return max+2;
}
int get_minwords(char filename[][MAX_FILEWORDS_NUM], int a, int b)  {
    //在从 第一个参数开始 的 n 个文件中 找出最长名字的长度
    int min = MAX_FILEWORDS_NUM;
	for (int i = a; i < b; i++) {
        int temp = strlen(&filename[i][0]); 
        min = min < temp ? min : temp; 
    }
    return min+2;
}
void ls_print(char filename[][MAX_FILEWORDS_NUM], int cnt) {
    int col = get_col();
	int max = get_maxwords(filename, 0, cnt);
    int min = get_minwords(filename, 0, cnt);
    int listnum;
    for (listnum = col/max + 1; listnum <= col/min + 1; listnum++) {  //循环找到最合适的列数
        int listlong = cnt/listnum; //先这样 一会需要改这里  
        int listmax[listnum] = {0};   //每列的最长文件名
        for(int i = 0; i < listnum; i++) {
            //for(int j = i*listlong; j < ( (i*listnum+listnum)>cnt?cnt:(i*listnum+listnum)); j++ ){
            //    int temp = get_maxwords(filename, i*listlong, (i*listlong+listlong) > cnt ? cnt : (i*listlong+listlong));
            //    listmax[i] = listmax[i] > temp ? listmax[i] : temp;
            //}
            listmax[i] = get_maxwords(filename, i*listlong, (i*listlong+listlong)>cnt?cnt:(i*listlong+listlong));
        }

        int maxsum = 0;
        for(int i = 0; i < listnum; i++) {
            maxsum+=listmax[i];
        }
        if(maxsum <= col) continue; else break;
    }
    int ans = listnum-1;   //ans 就是最终的行数！！！！！！

}
void file_store(char filename[][MAX_FILEWORDS_NUM], struct dirent* dirent, int* cnt) {
	sprintf(&filename[(*cnt)++][0], "%s", dirent->d_name);
}
int cmp(const void *a, const void *b) {
    char *s1 = (char *)a;
    char *s2 = (char *)b;
    return strcmp(s1, s2);
}
void file_sort(char filename[][MAX_FILEWORDS_NUM], int cnt) {
    qsort(&filename[0][0], cnt, MAX_FILEWORDS_NUM, cmp);   //排序成功

}

void ls_handle(char* wd, int flag) {
    //printf("进入lshandle成功！\n");
    struct stat* buf;
    buf = (struct stat* )malloc(sizeof(struct stat));
    DIR* dir = opendir(wd);

    if(dir == NULL) {
        perror("opendir");
        exit(1);
    }
    char filename[MAX_FILENUM][MAX_FILEWORDS_NUM]; 
    int cnt = 0;
    struct dirent* dirent;
    //printf("flag = %d\n", flag);
    //printf("flag&LL = %d\n", flag&LLFLAG);
    //printf("flag&LS = %d\n", flag&LSFLAG);
	while((dirent = readdir(dir)) != NULL) {
        //printf("进入循环成功\n");
        char newwd[100];
        sprintf(newwd, "%s/%s", wd, dirent->d_name);
        stat(newwd,buf);
        if(stat(newwd,buf) < 0) {
            perror("stat");
        }
        if(flag & LLFLAG) {
            ll_print(buf, dirent);
            printf("\n");
        } else if(flag & LSFLAG) {
            file_store(filename, dirent, &cnt);
        } 
    }
    
    file_sort(filename, cnt);
    ls_print(filename, cnt);
}

int main(int argc, char* argv[]) {
    char* wd;
    int flag = 0;
    if((argc == 1 ) || strcmp(argv[1], "-al") != 0) {
        printf("处理ls：\n");
        flag |= LSFLAG;
        if(argc == 1) {    //无地址参数  缺省为当前目录
            wd = getcwd(NULL, 0);
            ls_handle(wd, flag);
        } else if(argc == 2) {  //只有一个地址参数
            wd = argv[1];
            ls_handle(wd, flag);
        } else {  //有多个地址参数
            for(int i = 1; i < argc; i++) {
                i == 1 || printf("\n");
                wd = argv[i];
                printf("%s:\n", wd);
                ls_handle(wd, flag);
            }
        }
    } else {
        flag |= LLFLAG;
        if(argc == 2) {    //无地址参数  缺省为当前目录
            wd = getcwd(NULL, 0);
            ls_handle(wd, flag);
        } else if(argc == 3) {  //只有一个地址参数
            wd = argv[2];
            ls_handle(wd, flag);
        } else {  //有多个地址参数
            for(int i = 2; i < argc; i++) {
                i == 2 || printf("\n");
                wd = argv[i];
                printf("%s:\n", wd);
                ls_handle(wd, flag);
            }
        }
    }
    return 0;
}
