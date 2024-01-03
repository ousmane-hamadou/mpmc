#ifndef UTILS_H
#define UTILS_H

#include "chan.h"

#define L_ERROR_CODE 100000

void error(int err, char *msg);

void *init_buf(size_t buf_len, chan_datatype dt);

void push(chan *ch, void *item);

void shift(chan *ch, void *arg);

#endif