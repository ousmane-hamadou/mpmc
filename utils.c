#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "utils.h"

int shift_int(int *buf, size_t len, size_t offset);
void push_int(int *buf, size_t len, size_t item_count, int item);

void error(int err, char *msg)
{
    if (err == L_ERROR_CODE)
    {
        fprintf(stderr, "%s\n", msg);
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "[%s] code: %d, error message: %s\n", msg, err, strerror(errno));
    exit(EXIT_FAILURE);
}

void *init_buf(size_t buf_len, chan_datatype dt)
{
    void *buf;
    switch (dt)
    {
    case CHAN_INT:
        buf = calloc(buf_len, sizeof(int));
        break;
    default:
        error(L_ERROR_CODE, "[INIT_BUF] unknow datatype");
    }

    return buf;
}

void push(chan *ch, void *item)
{
    size_t len = ch->buf_len;
    void *buf = ch->buf;

    switch (ch->dt)
    {
    case CHAN_INT:
        int *b = (int *)buf;
        push_int(b, len, ch->item_count, *((int *)item));
        ch->item_count++;
        break;
    default:
        error(L_ERROR_CODE, "[PUSH] unknown datatype");
        break;
    }
}

void shift(chan *ch, void *arg)
{
    size_t len = ch->buf_len;
    void *buf = ch->buf;

    switch (ch->dt)
    {
    case CHAN_INT:
        int *r = (int *)arg;
        *r = shift_int((int *)buf, len, ch->item_count);
        ch->item_count--;
        break;

    default:
        error(L_ERROR_CODE, "[SHIFT] unknown datatype");
        break;
    }
}

void push_int(int *buf, size_t len, size_t item_count, int item)
{
    if (item_count == 0)
    {
        buf[len - 1] = item;
        return;
    }

    size_t i = len - item_count - 1;

    len = len - 1;
    while (i < len)
    {
        buf[i] = buf[i + 1];
        i++;
    }
    buf[i] = item;
}

int shift_int(int *buf, size_t len, size_t count)
{
    size_t i = len - count;
    int val = buf[i];
    buf[i] = 0;

    return val;
}