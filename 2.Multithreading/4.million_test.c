/*************************************************************************
	> File Name: 4.million_test.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月04日 星期四 19时47分29秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>


void get_num(FILE *fp, int *num) {
    *num = 0;
    size_t len = 0;
    char *buf;
    getline(&buf, &len, fp);
    *num = atoi(buf);
}

void set_num(FILE *fp, int num) {
    fprintf(fp, "%d", num);
}

int main() {
    int num;
    char fnumname[] = "tmpnum";
    char fsumname[] = "tmpsum";
    FILE *initfps = fopen(fsumname, "w");
    set_num(initfps, 0);
    fclose(initfps);
    FILE *initfpn = fopen(fnumname, "w");
    set_num(initfpn, 0);
    fclose(initfpn);

    pid_t pid = 1;
    int my_id = 0;
    for(int i = 0; i < 10; i++) {
        if(pid != 0) {
            my_id++;
            pid = fork();
        } else {
            break;
        }
    }
    if(pid != 0) printf("[my_id %d] pid = %d\n", my_id, pid);

    if(pid != 0) {
        //sleep(5);
        int n = 10;
        while(n--) {
            int waitid = waitpid(-1, NULL, 0);
            printf("wait [%d]  waitpid = %d \n", n, waitid);
            if(waitid == -1) perror("waitpi()");
        }
        int ans;
        FILE *fpans = fopen(fsumname, "r");
        get_num(fpans, &ans);
        fclose(fpans);
        printf("ans = %d\n", ans);
    }
    
    
    if(pid == 0) {
        printf("HI im %d cpid!\n", my_id);

        while(1) {
            int num = 0;
            FILE *fpnum = fopen(fnumname, "r+");
            printf("[No. %d] fopen1 success, ready to lock\n",my_id);
            flock(fpnum->_fileno, LOCK_EX);
            printf("[No. %d]fpnum locked!!!\n",my_id);
            get_num(fpnum, &num);
            if(num > 1000) {
                fclose(fpnum);
                break;
            }
            //fclose(fpnum);
            //fpnum = fopen(fnumname, "w");
            printf("[No. %d]second fopen success\n", my_id);
            fseek(fpnum, 0, SEEK_SET);
            set_num(fpnum, num + 1);
            fclose(fpnum);
            printf("[No. %d]fpnum closed\n", my_id);

            
            int sum = 0;
            FILE *fpsum = fopen(fsumname, "r+");
            flock(fpsum->_fileno, LOCK_EX);
            get_num(fpsum, &sum);
            //fclose(fpsum);
            //fpsum = fopen(fsumname, "w");
            fseek(fpsum,0,SEEK_SET);
            printf("num = %d  sum = %d \n", num, sum);
            set_num(fpsum, sum+num);
            fclose(fpsum);
        }
        printf("[No. %d] OK!\n", my_id);

        exit(0);
    }
    
    return 0;
}
