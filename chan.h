#include <semaphore.h>
#include <pthread.h>

#ifndef CHAN_H
#define CHAN_H

typedef enum
{
    CHAN_INT
} chan_datatype;

typedef struct chan
{
    pthread_mutex_t mutex;
    sem_t empty;
    sem_t full;

    void *buf;
    size_t buf_len;
    size_t item_count;
    chan_datatype dt;
} chan;

void chan_int(chan *ch, size_t buf_len, chan_datatype dt);

void chan_destroy(chan *ch);

void chan_send(chan *ch, void *msg);

void chan_recv(chan *ch, void *msg);

#endif