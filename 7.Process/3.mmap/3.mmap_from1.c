/*************************************************************************
	> File Name: 1.mmap.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月29日 星期一 19时30分39秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
void print(int n) {
        FILE *fp = fopen("tmpfile", "r+");
        if (fp == NULL) { perror("fopen"); exit(1); }
        size_t len = 1024;
        char *share = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fp->_fileno, 0);
        if (share == (void *)(-1)) { perror("mmap"); exit(1); }
        printf("[Father] buf = %s\n", share);
        fclose(fp);
}
void tmpfile_init() {
        FILE *fp = fopen("tmpfile", "w");
        if (fp == NULL) { perror("fopen"); exit(1); }
        
        char buf[] = "asd";
        int fwrite_ret = fwrite(buf, 1, strlen(buf), fp);
        if (fwrite_ret < 0) { perror("fwrite"); exit(1); }
        fclose(fp);


}
int main() {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }
    if (pid == 0) {
        tmpfile_init();   //为什么空文件不行？
        FILE *fp = fopen("tmpfile", "r+");
        if (fp == NULL) { perror("fopen"); exit(1); }
        
        size_t len = 1024;
        char *share = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fp->_fileno, 0);
        if (share == (void *)(-1)) { perror("mmap"); exit(1); }

        char buf2[100];
        while(1) {
            scanf("%s", buf2);
            strcpy(share, buf2);
            //printf("buf2 = %s\n", buf2);
            kill(getppid(), 10);
        }
        fclose(fp);

    }
    if (pid != 0) {
        signal(10, print);
        while(1) pause();
    }
    return 0;
}
