#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "utils.h"

#define MAX_ITER 20

#define CONSUMERS 5
#define PRODUCERS 10
#define BUF_SIZE 10

int get_consumer_index();

int get_producer_index();

void *producer(void *arg);

void *consumer(void *arg);

pthread_t consumers[CONSUMERS];
pthread_t producers[PRODUCERS];

int main(int argc, char const *argv[])
{
    chan ch;
    chan_int(&ch, BUF_SIZE, CHAN_INT);

    for (int i = 0; i < PRODUCERS; i++)
    {
        pthread_create(&producers[i], NULL, producer, &ch);
    }

    for (int i = 0; i < CONSUMERS; i++)
    {
        pthread_create(&consumers[i], NULL, consumer, &ch);
    }

    for (int i = 0; i < PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    chan_destroy(&ch);

    return EXIT_SUCCESS;
}

void *producer(void *arg)
{
    chan *ch = (chan *)arg;
    int i = rand() % MAX_ITER;
    int j = 0;
    int idx = -1;

    while (j < i)
    {
        if (idx == -1)
        {
            idx = get_producer_index();
        }

        int msg = rand() % 2000;
        printf("[PRODUCER %d] sending %d\n", idx, msg);
        fflush(stdout);
        chan_send(ch, &msg);
        usleep(rand() % 1000001);
    }
    printf("[%ld] finished!\n");
    fflush(stdout);
}

void *consumer(void *arg)
{
    chan *ch = (chan *)arg;
    int idx = -1;

    while (1)
    {
        if (idx == -1)
        {
            idx = get_consumer_index();
        }

        int msg;
        // printf("[CONSUMER %d] waiting message\n", idx, pthread_self());
        fflush(stdout);

        chan_recv(ch, &msg);

        printf("[CONSUMER %d] got %d\n", idx, msg);
        fflush(stdout);

        usleep(rand() % 1000001);
    }
}

int get_consumer_index()
{
    pthread_t tid = pthread_self();

    int i = 0;
    while (consumers[i] != tid)
    {
        i++;
    }
    return i;
}

int get_producer_index()
{
    pthread_t tid = pthread_self();

    int i = 0;
    while (producers[i] != tid)
    {
        i++;
    }
    return i;
}