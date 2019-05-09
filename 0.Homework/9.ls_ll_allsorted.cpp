/*************************************************************************
	> File Name: 9.ls_ll_allsorted.cpp
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年05月08日 星期三 22时40分00秒
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

#define MODE_SIZE 20
#define LINK_SIZE 20
#define ID_SIZE 20
#define ID_SIZE 20
#define ST_SIZE 20
#define TIME_SIZE 20
#define NAMESIZE 100

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

int ls_cmp(const void *a, const void *b) {
    char *s1 = (char *)a;
    char *s2 = (char *)b;
    return strcmp(s1, s2);
}
void file_sort(char filename[][MAX_FILEWORDS_NUM], int cnt) {         //排序文件名
    qsort(&filename[0][0], cnt, MAX_FILEWORDS_NUM, ls_cmp);   //排序成功

}

typedef struct fileinfo{
    char mode[MODE_SIZE];
    char link[LINK_SIZE];
    char uid[ID_SIZE];
    char gid[ID_SIZE];
    char st[ST_SIZE];
    char time[TIME_SIZE];
    char name[NAMESIZE];
}File;


void ll_print(struct stat* buf, struct dirent* dirent, int cnt, File* file) {              //按ll输出
    for(int i = 0; i < cnt; i++) {
		printf("%s", file[i].mode);
		printf("%s", file[i].link);
		printf("%s", file[i].uid);
		printf("%s", file[i].gid);
		printf("%s", file[i].st);
		printf("%s", file[i].time);
		printf("%s", file[i].name);
	    printf("\n");
	} 
    //print_mode(buf);
	//char str[100];
    //printf("%s", print_mode(buf, str));
    //printf(" %4ld" , buf->st_nlink);
    //printf(" %-8s", getpwuid(buf->st_uid)->pw_name);
    //printf(" %-8s", getgrgid(buf->st_gid)->gr_name);
    //printf(" %8ld" , buf->st_size);
    //printf(" %.12s", 4 + ctime(&buf->st_mtime));
    //printf(" %s\t", dirent->d_name);
}

File* fileinfo_save(struct stat* buf, struct dirent* dirent, int* cnt, File* file) {
	char str[100];
    sprintf(file[*cnt].mode, "%s", print_mode(buf, str));
    sprintf(file[*cnt].link, " %4ld" , buf->st_nlink);
    sprintf(file[*cnt].uid, " %-8s", getpwuid(buf->st_uid)->pw_name);
    sprintf(file[*cnt].gid, " %-8s", getgrgid(buf->st_gid)->gr_name);
    sprintf(file[*cnt].st, " %8ld" , buf->st_size);
    sprintf(file[*cnt].time, " %.12s", 4 + ctime(&buf->st_mtime));
    sprintf(file[*cnt].name, " %s", dirent->d_name);
    (*cnt)++;
    return file;
}

int ll_cmp(const void *a, const void *b) {
    File* s1 = (File *)a;
    File* s2 = (File *)b;
    //printf("comparing.... :%s and %s\n", s1->name, s2->name);
    return strcmp(s1->name, s2->name);
}
File* ll_file_sort(File* file, int cnt) {
    qsort(file, cnt, sizeof(File), ll_cmp);   //排序成功
    return file;
}


int get_col() {                                                       //得到bash窗口宽度
   struct winsize size;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);//主要
	return size.ws_col;
}
int get_maxwords(char filename[][MAX_FILEWORDS_NUM], int a, int b)  { //得到 a～b 范围内最长文件名
    int max = 0;
	for (int i = a; i < b; i++) {
        int temp = strlen(&filename[i][0]); 
        max = max > temp ? max : temp; 
    }
    return max+2;  //返回时加2 留出每列之间的2个字符间隙
}
int get_minwords(char filename[][MAX_FILEWORDS_NUM], int a, int b)  { //得到 a～b 范围内最短文件名
    int min = MAX_FILEWORDS_NUM;
	for (int i = a; i < b; i++) {
        int temp = strlen(&filename[i][0]); 
        min = min < temp ? min : temp; 
    }
    return min+2;  //返回时加2 留出每列之间的2个字符间隙
}
int get_listlong(int cnt, int listnum) {                              //得到以listnum为总列数时 每列应该输出的文件数量
    if(cnt % listnum == 0) {
        return cnt/listnum;
    } else {
        return cnt/listnum+1;
    }
}
int get_bestlistnum(char filename[][MAX_FILEWORDS_NUM], int cnt) {    //得到 最佳 列数 listunm
    int col = get_col();
	int max = get_maxwords(filename, 0, cnt);
    int min = get_minwords(filename, 0, cnt);
    int listnum;
    
    if(max > col) {
        listnum = 1;
    } else {
        for (listnum = col/max; listnum <= col/min; listnum++) {  //循环找到最合适的列数
            int listlong = get_listlong(cnt, listnum); //先这样 一会需要改这里  
            int listmax[listnum] = {0};   //每列的最长文件名
            for(int i = 0; i < listnum; i++) {
                listmax[i] = get_maxwords(filename, i*listlong, (i*listlong+listlong));
            }

            int maxsum = 0;
            for(int i = 0; i < listnum; i++) {
                maxsum+=listmax[i];
            }
            if(maxsum <= col) continue; else break;
        }
        listnum--;
    }
    return listnum;
}

void ls_print(char filename[][MAX_FILEWORDS_NUM], int cnt) {          // ls 打印
    int listnum = get_bestlistnum(filename, cnt);
    int listlong = get_listlong(cnt, listnum);
    int listmax[listnum] = {0};
    for(int i = 0; i < listnum; i++) {
        listmax[i] = get_maxwords(filename, i*listlong, (i*listlong+listlong));
    }
    for(int i = 0; i < listlong; i++) {
        for(int j = 0; j < listnum; j++) {
            printf("%-*s", listmax[j], &filename[i+j*listlong][0]);
        }
    printf("\n");
    }
    //printf("共有%d个文件  最优分成%d列 每列最多输出%d个文件\n", cnt, listnum, listlong);
}

void filename_save(char filename[][MAX_FILEWORDS_NUM], struct dirent* dirent, int* cnt) {              //储存文件名
    char name_temp[MAX_FILEWORDS_NUM];
    strcpy(name_temp, dirent->d_name);
    //printf("name_temp%d = %s\n", *cnt, name_temp);
	if(strncmp(name_temp, ".", 1) == 0) {    //处理隐藏文件
        return ;
	} else {
		sprintf(&filename[(*cnt)++][0], "%s", name_temp);
	}
}


void ls_handle(char* wd, int flag) {                                  //处理ls 或 ll 命令
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
    File* file = (File* )malloc(sizeof(File)*MAX_FILENUM);
	while((dirent = readdir(dir)) != NULL) {
        char newwd[100];
        sprintf(newwd, "%s/%s", wd, dirent->d_name);
        stat(newwd,buf);
        if(stat(newwd,buf) < 0) {
            perror("stat");
            exit(1);
        }
        if(flag & LLFLAG) {
            file = fileinfo_save(buf, dirent, &cnt, file);
        } else if(flag & LSFLAG) {
            filename_save(filename, dirent, &cnt);
        } 
    }
    //***************************************
    if(flag & LLFLAG) {
        file = ll_file_sort(file, cnt);
        ll_print(buf, dirent, cnt, file);
    } else if(flag & LSFLAG) {
        file_sort(filename, cnt);
        ls_print(filename, cnt);
    } 

}

int main(int argc, char* argv[]) {
    char* wd;
    int flag = 0;
    if((argc == 1 ) || strcmp(argv[1], "-al") != 0) {        //ls
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
    } else {                                                 //ll
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
