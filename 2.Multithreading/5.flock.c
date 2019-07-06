/*************************************************************************
	> File Name: 5.flock.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月04日 星期四 20时40分50秒
 ************************************************************************/

#include<stdio.h>
#include <sys/file.h>
#include <unistd.h>

#define PATH "tmp_lock"

int main() {
    FILE *fp = fopen(PATH, "w");
    flock(fp->_fileno, LOCK_EX);

    printf("locked successful!\n");
    
    pid_t pid;
    pid = fork();
    if(pid == 0) {
        //fclose(fp);
        FILE *fpp;
        printf("i am child\n");
        fp = fopen(PATH, "r");
        printf("child fopen success\n");
        flock(fpp->_fileno, LOCK_EX);
        printf("after chiled  locked\n");
        fclose(fp);
        fflush(stdout);
    }
    if(pid != 0) {
        sleep(2);
        //flock(fp->_fileno, LOCK_UN);
        fclose(fp);
        printf("fp closed\n");
        sleep(2);
    }
    
    
    return 0;
}

