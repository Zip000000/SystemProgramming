/*************************************************************************
	> File Name: 10.cp_simple.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年05月10日 星期五 21时08分53秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define SINGLE_COPY 1
#define MANY_COPY 2
#define FTOF 4
#define FTOD 8
#define DTOD_EXIST 16
#define DTOD_NEW 32


#define DIR_YES 64
#define DIR_NO 128

int cp_atob_file(char* a, char* b) {
    FILE *fp_from = NULL;
    FILE *fp_to = NULL;
    int len;
    char* buf = NULL;
    if((fp_from = fopen(a, "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    if((fp_to = fopen(b, "w+")) == NULL) {
        perror("fopen");
        exit(1);
    }
    fseek(fp_from,0L,SEEK_END);
    len = ftell(fp_from);
    buf = (char*)malloc(sizeof(char) * len);   //??????
    memset(buf, 0, len);
    rewind(fp_from);   //复位到开头
    fread(buf, 1, len, fp_from);  //???????
    fwrite(buf, 1, len, fp_to);    //???????
    printf("copy %s to %s success!\n", a, b);
    fclose(fp_from);
    fclose(fp_to);
}
char* get_filename(char* path, char* filename) {//通过路径得到文件名
    filename = strrchr(path, '/');
    if(filename == NULL) {
        filename = path;
    } else {
        filename++;
    }
    return filename;
}
char* get_plusname(char* dir, char* filename) {   
    if(strncmp((dir + strlen(dir) - 1), "/", 1) == 0) {
        strcat(dir, filename);
    } else {
        strcat(strcat(dir, "/"), filename);
    }
    return dir;
}

int cp_atob_dir(char* a, char* b_dir) {
    //printf("进入atob_dir函数,成功\n");
    char* a_name = get_filename(a, a_name);
    //printf("get_filename success! a_name = %s\n", a_name);
    char* b_name = get_plusname(b_dir, a_name);
    //printf("get_copyneame success!\n");
    printf("a = %s  b = %s \n", a_name, b_name);
    cp_atob_file(a_name, b_name);
}

int cp_dtod_exist(char* a, char* b) {
    printf("进入 dtodexist \n");
    DIR* dir;
    if((dir = opendir(a)) == NULL) {
        perror("opendir");
        exit(1);
    }
    struct dirent* dirent;     //接下来要处理隐藏文件！！！！
    while ((dirent = readdir(dir)) != NULL ) {
        printf("进入while\n");
        char* a_name ;
        a_name = get_plusname(a, dirent->d_name);
        printf("********aname = %s  b = %s\n", a_name, b);
        

        cp_atob_dir(a_name, b);
    }
    
    
    
    
}
int cp_dtod_new(char* a, char* b) {

}

int is_dir(char* path) {
    DIR* dir;
    if ((dir = opendir(path)) == NULL) {
            return DIR_NO;
    } else {
        closedir(dir);
        printf("%s is dir\n", path);
        return DIR_YES;
    }
}
int is_enough(int argc, char* argv[]) {
    switch (argc) {
        case 1: printf("缺少了文件操作数\n");
                exit(1);
                break;
        case 2: printf("在%s后缺少了要操作的目标文件\n", argv[1]);
                exit(1);
                break;
        case 3: return SINGLE_COPY;
        default: return MANY_COPY;
    } 

}

void get_flag(char* a, char* b, int* flag) {
    int is_a = is_dir(a);
    int is_b = is_dir(b);
    if(is_a == DIR_YES) printf("a是dir\n");
    if(is_b == DIR_YES) printf("b是dir\n");
    if(is_a == DIR_NO && is_b == DIR_NO) {
        (*flag) |= FTOF;
    } else if(is_a == DIR_NO && is_b == DIR_YES) {
        (*flag) |= FTOD;
    } else if(is_a == DIR_YES && is_b == DIR_YES) {
        (*flag) |= DTOD_EXIST;
    } else {
        (*flag) |= DTOD_NEW;
    }
}

int main (int argc, char *argv[]) {
    int flag = 0;
    flag |= is_enough(argc, argv);
    if(flag & SINGLE_COPY) {
        get_flag(argv[1], argv[2], &flag);
        if(flag & FTOF) {
            printf("atob_file flag = %d\n", flag);
            cp_atob_file(argv[1], argv[2]);
        } else if(flag & FTOD) {
            printf("atob_dir flag = %d\n", flag);
            cp_atob_dir(argv[1], argv[2]);
        } else if(flag & DTOD_EXIST) {
            printf("dtod_ex flag = %d\n", flag);
            cp_dtod_exist(argv[1], argv[2]);
        } else if(flag & DTOD_NEW) {
            printf("dtod_new flag = %d\n", flag);
            cp_dtod_new(argv[1], argv[2]);
        }
    } else {
        int needcp_num = argc - 2;
        char* final=argv[argc - 1];
        is_dir(final);
        for (int i = 0; i < needcp_num; i++) {
            cp_atob_dir(argv[i], final);
        }

    }


    return 0;
}
