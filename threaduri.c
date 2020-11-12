#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include "a2_helper.h"

int th1cr=0;

typedef struct
{
    int i;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;

} TH_STRUCT_INT;

typedef struct
{
    int thr;
    sem_t *semafor;
} STR_4;

typedef struct
{
    int thr;
    pthread_mutex_t *lock1;
} STR_5;

void *thread_function_4(void *param)
{
    STR_4 *st=(STR_4 *)param;
    sem_wait(st->semafor);

    info(BEGIN, 2, st->thr);
    info(END, 2, st->thr);

    sem_post(st->semafor);



    return NULL;

}

void *thread_function_5(void *param)
{
    STR_5 *st=(STR_5 *)param;

    pthread_mutex_lock(st->lock1);

    info(BEGIN, 8, st->thr);
    info(END, 8, st->thr);

    pthread_mutex_unlock(st->lock1);
    return NULL;
}

void *thread_function(void *param)
{
    TH_STRUCT_INT *st=(TH_STRUCT_INT *)param;

    pthread_mutex_lock(st->lock);

    info(BEGIN, 5, st->i);
    info(END, 5, st->i);

    pthread_mutex_unlock(st->lock);
    return NULL;
}

void *thr1_fnc(void *param)
{
    TH_STRUCT_INT *st=(TH_STRUCT_INT *)param;

    pthread_mutex_lock(st->lock);

    info(BEGIN, 5,st->i);
    th1cr=1;
    pthread_cond_broadcast(st->cond);
    pthread_cond_wait(st->cond,st->lock);
    info(END, 5, st->i);

    pthread_mutex_unlock(st->lock);
    return NULL;
}

void *thr2_fnc(void *param)
{
    TH_STRUCT_INT *st=(TH_STRUCT_INT *)param;

    pthread_mutex_lock(st->lock);

    if (th1cr==0)
        pthread_cond_wait(st->cond,st->lock);
    info(BEGIN, 5,st->i);
    info(END, 5, st->i);
    pthread_cond_broadcast(st->cond);
    pthread_mutex_unlock(st->lock);
    return NULL;
}

int main()
{
    init();

    info(BEGIN, 1, 0);

    int status=0;
    pid_t pid2=-1;
    pid_t pid8=-1;
    pid2=fork();
    if (pid2==-1)
    {
        perror("Could not create child process");
    }
    if (pid2==0)
    {
        info(BEGIN, 2,0);

        pid_t pid3=-1;
        pid3=fork();
        if(pid3 == -1)
        {
            perror("Could not create child process");
        }
        if (pid3 == 0)
        {
            info(BEGIN, 3,0);

            pid_t pid4=-1;
            pid4=fork();
            if(pid4 == -1)
            {
                perror("Could not create child process");
            }
            if (pid4==0)
            {
                info(BEGIN, 4,0);

                pid_t pid5=-1;
                pid5=fork();
                if(pid5 == -1)
                {
                    perror("Could not create child process");
                }
                if (pid5 == 0)
                {
                    info(BEGIN, 5,0);
                    pid_t pid6=-1;
                    pid_t pid7=-1;
                    pid6=fork();
                    if(pid6 == -1)
                    {
                        perror("Could not create child process");
                    }
                    if (pid6 == 0)
                    {
                        info(BEGIN, 6,0);

                        info(END, 6,0);
                        exit(status);
                    }

                    pid7=fork();
                    if(pid7 == -1)
                    {
                        perror("Could not create child process");
                    }
                    if (pid7 == 0)
                    {

                        info(BEGIN, 7,0);
                        info(END, 7,0);
                        exit(status);
                    }

                    waitpid(pid6,&status,0);
                    waitpid(pid7,&status,0);

                    pthread_t t_id[4];
                    TH_STRUCT_INT param[4];
                    int i;
                    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
                    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

                    for (i=0; i<4; i++)
                    {
                        param[i].i=i+1;
                        param[i].lock=&lock;
                        param[i].cond=&cond;

                        if (i == 2 || i == 3)
                            pthread_create(&t_id[i], NULL, thread_function,&param[i]);
                        if (i == 0)
                            pthread_create(&t_id[i], NULL, thr1_fnc,&param[i]);
                        if (i == 1)
                            pthread_create(&t_id[i], NULL, thr2_fnc,&param[i]);
                    }


                    for (i=0; i<4; i++)
                        pthread_join(t_id[i],NULL);

                    pthread_cond_destroy(&cond);

                    pthread_mutex_destroy(&lock);

                    info(END, 5,0);
                    exit(status);
                }
                waitpid(pid5,&status,0);

                info(END, 4,0);
                exit(status);
            }

            waitpid(pid4,&status,0);

            info(END, 3,0);
            exit(status);
        }

        waitpid(pid3,&status,0);


        sem_t semafor;
        sem_init(&semafor, 0, 5);
        int i=0;
        STR_4 param[40];
        pthread_t t_id[40];

        for (i=0;i<40;i++)
        {
            param[i].thr=i+1;
            param[i].semafor=&semafor;
            pthread_create(&t_id[i],NULL,thread_function_4,&param[i]);
        }

        for (i=0;i<40;i++)
        {
        pthread_join(t_id[i],NULL);
        }
        sem_destroy(&semafor);
        info(END, 2,0);
        exit(status);
    }

    pid8=fork();
    if (pid8 == -1)
    {
        perror("Could not create child process");
    }

    if (pid8==0)
    {
        info(BEGIN, 8,0);

        int i=0;
        STR_5 param[5];
        pthread_t t_id[5];
        pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

        for (i=0;i<5;i++)
        {
            param[i].lock1=&lock1;
            param[i].thr=i+1;
            pthread_create(&t_id[i],NULL,thread_function_5,&param[i]);
        }

        for (i=0;i<5;i++)
        {
        pthread_join(t_id[i],NULL);
        }

        pthread_mutex_destroy(&lock1);

        info(END, 8,0);
        exit(status);
    }

    waitpid(pid2,&status,0);
    waitpid(pid8,&status,0);
    info(END, 1, 0);
    return 0;
}

