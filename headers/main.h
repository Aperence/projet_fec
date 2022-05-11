#ifdef __APPLE__
#include <sys/syslimits.h>  // include PATH_MAX
#endif
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <stdbool.h>
#include "portable_semaphore.h"

typedef struct args
{
    DIR *input_dir;
    char input_dir_path[PATH_MAX];
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

extern args_t args;

typedef struct threads_args{
    char **filenames;               //list of filenames from which threads will consume
    uint32_t nextFile;              // index of next file to be processed
    uint32_t numberFiles;           // total number of files
    sem_t *semaphore_read;          // semaphore used to read next filename
    sem_t *semaphore_writing;       // semaphore used to write to the out file
}threads_args_t;

extern threads_args_t t_args;

#endif