#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void* threadfunc(void* thread_param)
{
    struct thread_data *td = (struct thread_data*) thread_param;

    usleep(td->wait_to_obtain_ms * 1000);

    if (pthread_mutex_lock(td->mutex) != 0) {
        td->thread_complete_success = false;
        return thread_param;
    }

    usleep(td->wait_to_release_ms * 1000);

    pthread_mutex_unlock(td->mutex);
    td->thread_complete_success = true;

    return thread_param;
}

bool start_thread_obtaining_mutex(struct thread_data *td)
{
    td->thread_complete_success = false;

    if (pthread_create(&td->thread, NULL, threadfunc, td) != 0) {
        return false;
    }

    return true;
}
