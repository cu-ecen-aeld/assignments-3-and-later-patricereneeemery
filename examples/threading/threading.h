#ifndef THREADING_H
#define THREADING_H

#include <pthread.h>
#include <stdbool.h>

struct thread_data {
    pthread_t thread;
    pthread_mutex_t *mutex;
    int wait_to_obtain_ms;
    int wait_to_release_ms;
    bool thread_complete_success;
};

bool start_thread_obtaining_mutex(struct thread_data *td);

#endif
