#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "main.h"
#include "message.h"
#include "block.h"
#include "portable_semaphore.h"
#include "my_threads.h"


uint32_t getNextFile(){
    uint32_t val = 0;
    if (t_args.nextFile >= t_args.numberFiles){
        return -1;
    }
    sem_wait(t_args.semaphore_read);
    val = t_args.nextFile;
    t_args.nextFile++;
    sem_post(t_args.semaphore_read);
    return val;
}

uint32_t *processFile(void *args_file){
    uint32_t fileNumber = getNextFile();
    while (fileNumber != -1){
        char *path = malloc(PATH_MAX);
        if (path == NULL){
            fprintf(stderr, "Error with the malloc which cas created to store the path of the file with the threads\n");
            return NULL;
        }
        strcpy(path, args.input_dir_path);
        strcat(path, "/");
        strcat(path, *(t_args.filenames + fileNumber));
        if (args.verbose){
            printf(">>>>>> filename : %s\n", *(t_args.filenames + fileNumber));
            printf(">>>>>> path     : %s\n", path);
        }
        message_t *message = openFile(path);
        if (message == NULL) return NULL;
        if (processBlock(message->listBlock, message->numberBlocks, message->seed, message->size_redundance, message->size_symbol)<0) return NULL;
        sem_wait(t_args.semaphore_writing);
        if (writeToFile(args.output_stream, message, *(t_args.filenames+fileNumber))<0) return NULL;
        sem_post(t_args.semaphore_writing);
        free(*(t_args.filenames + fileNumber));
        free(path);
        fileNumber = getNextFile();
    }
    return args.output_stream;
}