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

void *processFile(void *args_file){
    uint32_t fileNumber = getNextFile();
    while (fileNumber != -1){
        char *path = malloc(PATH_MAX);
        strcpy(path, args.input_dir_path);
        strcat(path, "/");
        strcat(path, *(t_args.filenames + fileNumber));
        if (args.verbose){
            printf(">>>>>> filename : %s\n", *(t_args.filenames + fileNumber));
            printf(">>>>>> path     : %s\n", path);
        }
        message_t *message = openFile(path);
        processBlock(message->listBlock, message->numberBlocks, message->seed, message->size_redundance, message->size_symbol);
        sem_wait(t_args.semaphore_writing);
        writeToFile(args.output_stream, message, *(t_args.filenames+fileNumber));
        sem_post(t_args.semaphore_writing);
        free(*(t_args.filenames + fileNumber));
        free(path);
        fileNumber = getNextFile();
    }
    return NULL;
}