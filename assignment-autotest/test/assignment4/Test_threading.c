#include "threading.h"
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "unity.h"

void test_thread_complete_success(void)
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct thread_data td = {
        .mutex = &mutex,
        .wait_to_obtain_ms = 10,
        .wait_to_release_ms = 10
    };

    TEST_ASSERT_TRUE(start_thread_obtaining_mutex(&td));
    pthread_join(td.thread, NULL);
    TEST_ASSERT_TRUE(td.thread_complete_success);
}

void test_mutex_locking(void)
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);

    struct thread_data td = {
        .mutex = &mutex,
        .wait_to_obtain_ms = 10,
        .wait_to_release_ms = 10
    };

    TEST_ASSERT_TRUE(start_thread_obtaining_mutex(&td));
    usleep(20000);
    TEST_ASSERT_FALSE(td.thread_complete_success);

    pthread_mutex_unlock(&mutex);
    pthread_join(td.thread, NULL);
}

void test_timing_behavior(void)
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct thread_data td = {
        .mutex = &mutex,
        .wait_to_obtain_ms = 50,
        .wait_to_release_ms = 50
    };

    TEST_ASSERT_TRUE(start_thread_obtaining_mutex(&td));

    pthread_join(td.thread, NULL);
    TEST_ASSERT_TRUE(td.thread_complete_success);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_thread_complete_success);
    RUN_TEST(test_mutex_locking);
    RUN_TEST(test_timing_behavior);
    return UNITY_END();
}

// Unity requires these even if unused
void setUp(void) {}
void tearDown(void) {}

