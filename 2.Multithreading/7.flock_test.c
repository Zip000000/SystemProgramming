/*************************************************************************
	> File Name: 7.flock_test.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月05日 星期五 16时11分20秒
 ************************************************************************/
#include<stdio.h>
#include <sys/file.h>
#include <unistd.h>

#define PATH "tmp_lock"

int main() {
    FILE *fp = fopen(PATH, "r+");
    flock(fp->_fileno, LOCK_EX);
    if(0 != flock(fp->_fileno, LOCK_EX)) {
        perror("flock");
    }

    printf("locked successful!\n");
    int my_id = 1;
    pid_t pid;
    pid = fork();
    
    if(pid != 0) my_id++, pid = fork();

    if(pid == 0) {
        if(0 != flock(fp->_fileno, LOCK_EX)) {
            perror("flock");
        }
        flock(fp->_fileno, LOCK_EX);
        printf("[No. %d] after child  locked\n", my_id);
        
        sleep(my_id);
        fseek(fp,0,SEEK_SET);
        fprintf(fp, "Hi my name is " "%d", my_id);
        fseek(fp,0,SEEK_SET);
        
        size_t len = 0;
        char *buf;
        getline(&buf, &len, fp);
        printf("i am %d  :  getline : %s\n", my_id, buf);


        fclose(fp);
        //fflush(stdout);
    }
    if(pid != 0) {
        sleep(5);
        //flock(fp->_fileno, LOCK_UN);
        fclose(fp);
        printf("fp closed\n");
        sleep(2);
    }
    
    
    return 0;
}

