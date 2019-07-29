/*************************************************************************
	> File Name: 2.mutex_condi.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月29日 星期一 16时07分15秒
 ************************************************************************/

#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>

struct sm_msg {
    char buff[1024];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

int main() {
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_condattr_init(&cond_attr);

    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
    int shmid = shmget(IPC_PRIVATE, sizeof(struct sm_msg), 0666|IPC_CREAT);
    if (shmid == -1) { perror("shmget"); exit(1); }
    struct sm_msg *msg = shmat(shmid, 0, 0);
    if (msg == (void *)(-1)) { perror("shmat"); exit(1); }
    
    pthread_mutex_t *p_mutex = &(msg->mutex);
    pthread_cond_t *p_cond = &(msg->cond);
    pthread_mutex_init(p_mutex, &mutex_attr);
    pthread_cond_init(p_cond, &cond_attr);

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) {
        while (1) {
            scanf("%s", msg->buff);
            pthread_mutex_lock(p_mutex);
            pthread_cond_signal(p_cond);
            pthread_mutex_unlock(p_mutex);
        }
    }
    
    if (pid != 0) {
        while (1) {
            pthread_mutex_lock(p_mutex);
            pthread_cond_wait(p_cond, p_mutex);
            pthread_mutex_unlock(p_mutex);
            printf("[Father] -> %s \n", msg->buff);
            memset(msg->buff, 0, sizeof(msg->buff));
        }
    }
    
    return 0;
}
