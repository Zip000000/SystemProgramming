/*************************************************************************
	> File Name: 1.million.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年10月09日 星期三 13时51分44秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>

//1 ~1000000
typedef struct arg {
    long long l, r;
} Arg;

long long sum = 0;
pthread_mutex_t mut;
void *work(void *arg) {
    Arg *ar = (Arg *)arg;
    for (int i = ar->l; i <= ar->r; i++) {
        pthread_mutex_lock(&mut);
        sum += i;
        pthread_mutex_unlock(&mut);
    }
}

int main() {
    long long sum = 0;
    for (long long i = 0; i <= 1000000; i++) sum += i;
    printf("%lld\n", sum);
    
    pthread_t pth_id[10];

    Arg arg[10];
    pthread_mutex_init(&mut, NULL);
    for (int i = 0; i < 10; i++) {
        int tmpval = 100000;
        arg[i].l = tmpval * i + 1;
        arg[i].r = arg[i].l + tmpval;
        pthread_create(pth_id + i, NULL, work, &arg[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(pth_id[i], NULL);
    }
    pthread_mutex_destroy(&mut);
    printf("%lld\n", sum);
    
    return 0;
}
