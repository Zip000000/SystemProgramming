/*************************************************************************
	> File Name: shm1.c
	> Author:
	> Mail:
	> Created Time: Sun 28 Jul 2019 18:33:26 CST
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



struct sm_msg{
    char buff[1024];
    pthread_mutex_t sm_mutex;
    pthread_cond_t sm_ready;
};

int main() {
    int shmid, pid;
    char *share_memory = NULL;

    pthread_mutexattr_t m_attr;
    pthread_condattr_t c_attr;

    pthread_mutexattr_init(&m_attr);
    pthread_condattr_init(&c_attr);

    pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&c_attr, PTHREAD_PROCESS_SHARED);


    if ((shmid = shmget(IPC_PRIVATE, sizeof(struct sm_msg), 0666|IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    if ((share_memory = shmat(shmid, 0, 0)) == NULL) {
        perror("shmat");
        exit(1);
    }

    struct sm_msg *msg = (struct sm_msg *)share_memory;

    pthread_mutex_init(&(msg->sm_mutex), &m_attr);
    pthread_cond_init(&msg->sm_ready, &c_attr);


    //char *buff = share_memory;

    if((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        while (1) {
            printf("In Clild -> please input the string:");
            scanf("%s", msg->buff);
            pthread_mutex_lock(&msg->sm_mutex);
            pthread_cond_signal(&msg->sm_ready);
            pthread_mutex_unlock(&msg->sm_mutex);
        }
    }
    while (1) {
        pthread_mutex_lock(&msg->sm_mutex);
        pthread_cond_wait(&msg->sm_ready, &msg->sm_mutex);
        pthread_mutex_unlock(&msg->sm_mutex);
        printf("In father -> %s \n", msg->buff);
        memset(msg->buff, 0, sizeof(msg->buff));
    }
    return 0;
}
