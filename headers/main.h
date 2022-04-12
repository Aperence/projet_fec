#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <stdbool.h>

typedef struct args
{
    DIR *input_dir;
    char input_dir_path[PATH_MAX];
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

args_t args;

typedef struct threads_args{
    char **filenames;    //list of filenames from which threads will consume
    uint32_t nextFile;   // index of next file to be processed
    uint32_t numberFiles;
}threads_args_t;

args_t args;

#endif