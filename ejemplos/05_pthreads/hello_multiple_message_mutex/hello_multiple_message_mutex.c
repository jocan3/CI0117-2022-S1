// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr> 
// This example shows an incorrect use of pthread mutex. The main thread is
// locking a mutex gaining its ownership but never unlocks it, passing the
// responsibility to a secondary thread. Unlocking a mutex without its ownership
// produces undefined/unknown behavior

#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int create_threads(size_t thread_count);
void* run(void*);

typedef struct shared_data {
    size_t thread_count;
    size_t next;
    pthread_mutex_t can_print_hello;
    pthread_mutex_t can_print_world;
    char message[6];
} shared_data_t;

typedef struct private_data {
    size_t thread_num;
    shared_data_t* shared_data;
    char message[6];
} private_data_t;

int main(int argc, char** arg) {
    size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);

    if (argc == 2) {
        if (sscanf(arg[1], "%zu", &thread_count) != 1 || errno) {
            fprintf(stderr, "Invalid number of threads.\n");
            return EXIT_FAILURE;
        }
    }

    int error = create_threads(thread_count);

    return error;
}

int create_threads(size_t thread_count) {
    pthread_t* threads = calloc(thread_count, sizeof(pthread_t));

    if (threads) {
        private_data_t* private_data = (private_data_t*)malloc(
                                    thread_count * sizeof(private_data_t));

        shared_data_t* shared_data = (shared_data_t*)calloc(1,
                                        sizeof(shared_data_t));

        shared_data->next = 0;

        if (private_data && shared_data) {
            pthread_mutex_init(&shared_data->can_print_hello, NULL);
            pthread_mutex_init(&shared_data->can_print_world, NULL);

            pthread_mutex_lock(&shared_data->can_print_world);

            for (size_t i = 0; i < thread_count; ++i) {
                private_data[i].thread_num = i;
                private_data[i].shared_data = shared_data;
                if (pthread_create(&threads[i], NULL, run,
                    (void*)&private_data[i]) != EXIT_SUCCESS) {
                    fprintf(stderr, "Could not create thread %zu.\n", i);
                    return EXIT_FAILURE;
                }
            }

            for (size_t i = 0; i < thread_count; ++i) {
                pthread_join(threads[i], NULL);
            }

            free(private_data);

            pthread_mutex_destroy(&shared_data->can_print_hello);
            pthread_mutex_destroy(&shared_data->can_print_world);
            free(shared_data);

        } else {
            fprintf(stderr, "Unable to allocate memory for %zu threads\n",
                thread_count);
            return EXIT_FAILURE;
        }

        free(threads);

    } else {
        fprintf(stderr, "Unable to allocate memory for %zu threads\n",
            thread_count);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// 1. multiple threads
// 2. shared memory
// 3. WR - WW

void* run(void* params) {
    private_data_t* data = (private_data_t*)params;
    shared_data_t* shared_data = data->shared_data;

    // mutex lock
    if (data->thread_num % 2 == 0) {
        pthread_mutex_lock(&shared_data->can_print_hello);
        sscanf("hello", "%s", data->message);
        printf("%zu: %s!\n", data->thread_num, data->message);
        pthread_mutex_unlock(&shared_data->can_print_world);
    } else {
        pthread_mutex_lock(&shared_data->can_print_world);
        sscanf("world", "%s", data->message);
        printf("%zu: %s!\n", data->thread_num, data->message);
        pthread_mutex_unlock(&shared_data->can_print_hello);
    }

    return NULL;
}