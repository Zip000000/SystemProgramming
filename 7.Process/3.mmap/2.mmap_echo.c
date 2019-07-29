/*************************************************************************
	> File Name: 2.mmap_echo.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月29日 星期一 19时49分48秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {perror("fork"); exit(1);}


    size_t len = 1024;
    
    if (pid != 0) {
        wait(NULL);
        printf("wait over\n");
        /*
        FILE *fp = fopen("tmp_file", "r+");
        if (fp == NULL) { perror("fopen"); exit(1); }
        char *share = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fp->_fileno, 0);
        if (share == (void *)(-1)) {perror("mmap main"); exit(1);}
        printf("share = %s\n", share);

        fclose(fp);
*/
    }

    if (pid == 0) {
        FILE *fp = fopen("tmp_file", "w+");
        if (fp == NULL) { perror("fopen"); exit(1); }
        fwrite("123", 1, 3, fp);
        char *share = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fp->_fileno, 0);
        if (share == (void *)(-1)) {perror("mmap son"); exit(1);}

        printf("请输入 ：\n");
        char buf[10];
        memset(buf, 0, sizeof(buf));
        int ret = scanf("%s", buf);
        printf("scanf_ret = %d buf = %s len = %ld\n", ret, buf, strlen(buf));
        
        printf("strcpy\n");
        strncpy(share, buf, strlen(buf));
        printf("strcpy over\n");
        
        printf("share = %s\n", share);
        fclose(fp);
        printf("son over\n");
        return 0;
        /*
        FILE *fp = fopen("tmp_file", "w");
        if (fp == NULL) { perror("fopen"); exit(1); }
        
        char buf[] = "HelloHaizei!";
        int n = 5;
        while (n--) {
            int fwrite_ret = fwrite(buf, 1, strlen(buf), fp);
            if (fwrite_ret <= 0) { perror("fwrite"); exit(1); }
        }
        fclose(fp);
*/


    }



    return 0;
}
