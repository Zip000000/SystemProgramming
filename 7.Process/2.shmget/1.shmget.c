/*************************************************************************
	> File Name: 1.shmget.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月29日 星期一 11时26分14秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/file.h>

int main() {
    key_t key = IPC_PRIVATE;
    //int flag = (IPC_CREAT | IPC_EXCL);
    int flag = (IPC_CREAT | 0666);
    size_t size = sizeof(char) * 1024;
    int shmid = shmget(key, size, flag);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    printf("shmid = %d\n", shmid);

    pid_t pid = 0;
    pid = fork();

    if (pid != 0) {
        sleep(1);
        FILE *fp = fopen("tmp", "w");
        if (fp == NULL) { perror("fopen1");exit(1); }
        char* buf = shmat(shmid, NULL, 0);
        if ((void *)buf == (void *)(-1)) {perror("shmat in master");exit(1);}
        printf("buf in master = %p\n", buf);
        while(1) {
                flock(fp->_fileno, LOCK_EX);
                printf("[master] bufff = %s\n", buf);
                flock(fp->_fileno, LOCK_UN);
                sleep(1);
        }
    } 
    if (pid == 0) {
        printf("in child\n");
        char *buf = shmat(shmid, NULL, 0);
        if ((void *)buf == (void *)(-1)) {perror("shmat in master");exit(1);}
        FILE *fp = fopen("tmp", "w");
        if (fp == NULL) { perror("fopen2");exit(1); }
        while(1) {
            flock(fp->_fileno, LOCK_EX);
            scanf("%s", buf);
            flock(fp->_fileno, LOCK_UN);
            sleep(1);
        }
    }

    
    return 0;
}
