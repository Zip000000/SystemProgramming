/*************************************************************************
	> File Name: 6.million_pthread.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月05日 星期五 15时09分30秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <pthread.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>

#define MAX 100000

char fnumname[] = "tmpnum";
char fsumname[] = "tmpsum";

void get_num(FILE *fp, long long *num) {
    *num = 0;
    size_t len = 0;
    char *buf;
    getline(&buf, &len, fp);
    *num = atoll(buf);
}

void set_num(FILE *fp, long long num) {
    fprintf(fp, "%lld", num);
}



void *calc(void* arg) {
        while(1) {
            long long num = 0;
            FILE *fpnum = fopen(fnumname, "r+");
            flock(fpnum->_fileno, LOCK_EX);
            get_num(fpnum, &num);
            if(num > MAX) {
                fclose(fpnum);
                break;
            }

            fseek(fpnum, 0, SEEK_SET);
            set_num(fpnum, num + 1);
            fclose(fpnum);

            long long sum = 0;
            FILE *fpsum = fopen(fsumname, "r+");
            flock(fpsum->_fileno, LOCK_EX);
            get_num(fpsum, &sum);

            fseek(fpsum,0,SEEK_SET);
            //printf("num = %lld  sum = %lld \n", num, sum);
            set_num(fpsum, sum+num);
            fclose(fpsum);
        }
    return NULL;
}


int main(){
	pthread_t pthread_id[10];

    FILE *initnp = fopen(fnumname, "w");
	set_num(initnp, 0);    
	fclose(initnp);
    FILE *initsp = fopen(fsumname, "w");
	set_num(initsp, 0);    
	fclose(initsp);
    
    
    for(int i = 0; i < 10; i++) {
        pthread_create(&pthread_id[i], NULL, calc, NULL);
        printf("pthread[%d] created success!\n", i);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(pthread_id[i], NULL);
        printf("join thread[%d]  success!\n", i);
    }

    printf("i am main, it is over\n");

    FILE *fp = fopen(fsumname, "r");
    long long ans;
    get_num(fp, &ans);
    printf("*****************ans = %lld*******************\n", ans);

    
	return 0;
}

