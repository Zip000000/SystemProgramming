/*************************************************************************
    > File Name: 1.first_thread.c
    > Author: Zip 
    > Mail: 307110017@qq.com 
    > Created Time: 2019年06月20日 星期四 09时40分33秒
 ************************************************************************/

#include<stdio.h>
#include <pthread.h>
#include <string.h>
#include<stdlib.h>
#include <unistd.h>
char name[50];
int tall;

void* print(void *arg) {
    sleep(1);
    printf("111111111111\n");
    printf("%s\n", name);
    printf("%d\n", tall);
    return NULL;
}
void* printt(void *arg) {
    sleep(1);
    printf("222222222222\n");
    printf("%s\n", name);
    printf("%d\n", tall);
    return NULL;
}


int main() {
    void*(*a)(void*);
    a=print;
    strcpy(name, "haizei");
    tall = 160;
    pthread_t thread_id;
    pthread_t thread_id2;
    pthread_create(&thread_id, NULL, a, NULL);
    pthread_join(thread_id, NULL);
    pthread_create(&thread_id2, NULL, printt, NULL);
    pthread_join(thread_id2, NULL);
    return 0;
}
