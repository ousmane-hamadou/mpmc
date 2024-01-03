#include <stdlib.h>
#include <stdio.h>

#include "chan.h"
#include "utils.h"

void chan_int(chan *ch, size_t buf_len, chan_datatype dt)
{
    ch->buf_len = buf_len;
    ch->item_count = 0;
    ch->dt = dt;
    void *buf = init_buf(buf_len, dt);

    if (buf == NULL)
    {
        error(1000, "CHAN_INT");
    }
    ch->buf = buf;

    int err;

    if ((err = pthread_mutex_init(&ch->mutex, NULL)) != 0)
    {
        error(err, "CHAN_INIT");
    }

    if ((err = sem_init(&ch->empty, 0, buf_len)) != 0)
    {
        error(err, "CHAN_INT");
    }

    if ((err = sem_init(&ch->full, 0, 0)) != 0)
    {
        error(err, "CHAN_INT");
    }
}

void chan_destroy(chan *ch)
{
    free(ch->buf);
    sem_destroy(&ch->empty);
    sem_destroy(&ch->full);
    pthread_mutex_destroy(&ch->mutex);
}

void chan_send(chan *ch, void *msg)
{
    // waitting for free place
    sem_wait(&ch->empty);

    pthread_mutex_lock(&ch->mutex);

    push(ch, msg);

    pthread_mutex_unlock(&ch->mutex);

    sem_post(&ch->full);
}

void chan_recv(chan *ch, void *arg)
{
    sem_wait(&ch->full);
    pthread_mutex_lock(&ch->mutex);
    shift(ch, arg);
    pthread_mutex_unlock(&ch->mutex);
    sem_post(&ch->empty);
}