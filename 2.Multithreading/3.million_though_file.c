/*************************************************************************
	> File Name: 3.million_though_file.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月04日 星期四 19时18分57秒
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

void get_num(char *filename, int *num) {
    FILE *fp;
    fp = fopen(filename,"r");
    size_t len = 0;
    char *buf;
    getline(&buf, &len, fp);
    int sum = atoi(buf);
    printf("%d\n", sum);
    fclose(fp);
    //fread(buf, 1, 4, fp)
    
}

void set_num(char *filename, int num) {
    FILE *fp;
    fp = fopen(filename, "w");
    char *buf;
    sprintf(buf, "%d", num);
    fwrite(buf, 1, strlen(buf), fp);
}









int main() {
    int my_id = 0;
    pid_t cpid = 1;
    
    for(int i = 0; i < 10; i++) {
        if(cpid != 0) {  //主
            my_id++;
            cpid = fork();
        }
        if(cpid == 0) {  //子
            break;
        }
    }

    if(cpid != 0) {
        long long allsum = 0;
        for(int i = 0; i < 10; i++) {

            //allsum += sum;
        }

        printf("allsum = %lld\n", allsum);
        return 0;
    }
    
    if(cpid == 0) {   //子
        printf("No. %d  ready!\n", my_id);
        long long sum = 0;
        for(int j = 1 + (100000) * (my_id-1); j <= 100000 * my_id; j++) {
            sum += j;
        }
        FILE *fp = fopen("tmp_sum_store", "a+");
        fprintf(fp, "%lld\n", sum);
        printf("[No. %d] sum = %lld\n", my_id, sum);
        return 0;
    }
    
}
