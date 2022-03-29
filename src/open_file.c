#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <dirent.h>
#include "message.h"

char **filenames;


int readDir(const char *directoryname){
    DIR *folder;
    struct dirent *entry;
    struct dirent *entry2;
    int files = 0;
    DIR* FD;
    folder = opendir(directoryname);

    if(folder == NULL){
        puts("Unable to read directory");
        return 1;
    }

    while (entry = readdir(folder)){
        files ++;
        printf("File %s\n", entry->d_name);
    }
    closedir(folder);
    
    folder = opendir(directoryname);
    filenames = malloc(files*(sizeof(char *)));
    printf("%d\n", files);

    int i = 0;
    while (entry2 = readdir(folder)){
        *(filenames + i) = entry2->d_name;
        i++;
    }


    closedir(folder);
    return 0;
}


/*
return -1 if file can't be openned

*/
int openFile(const char *filename){
    int fd = open(filename, O_RDONLY);
    if(fd < 0) return -1;
    
    char *fileInfo = malloc(25);   // don't forget the last '\0'
    read(fd, fileInfo, 24);
    uint32_t numberBlocks = ((uint64_t) *(fileInfo+16)) / ( (((uint32_t) *(fileInfo+4)) + ((uint32_t) *(fileInfo+12))) * ((uint32_t) *(fileInfo+8)));
    if (((uint64_t) *(fileInfo+16)) % ( (((uint32_t) *(fileInfo+4)) + ((uint32_t) *(fileInfo+12))) * ((uint32_t) *(fileInfo+8))) != 0){
        numberBlocks++;
    }
    uint8_t *message = malloc(((uint64_t) *(fileInfo+16)) + 25);
    read(fd, message, ((uint64_t) *(fileInfo+16)) + 25);
    block_t blockList[numberBlocks];
    makeBlockList(blockList, numberBlocks, message+24, (uint64_t) *(fileInfo+4), (uint64_t) *(fileInfo+8), (uint64_t) *(fileInfo+12));
    message_t message = {(uint32_t) *fileInfo, (uint64_t) *(fileInfo+16), blockList};
    uint32_t blockSize;

    if (close(fd)<0) return -1;
}


void makeBlockList(block_t *blockList, uint32_t numberBlocks, char *message, uint32_t block_size, uint32_t symbol_size, uint32_t redundance_size, uint32_t messageSize){
    for (uint32_t i = 0; i < numberBlocks; i++) //iterate on blocks
    {
        for (uint32_t j = 0; j < block_size; j++) //iterate on symboles
        {
            if (symbol_size*block_size*i + symbol_size*j >= messageSize){
                break;
            }
            uint8_t *symbol = malloc(symbol_size);
            memcpy(symbol, (void *) (message + symbol_size*block_size*i + symbol_size*j), symbol_size);
            printf("%s\n", symbol);
            
        }
        
    }
    
}

int main(int argc, char const *argv[])
{
    
    makeBlockList(NULL, 2, "Mastering-C-is-not-super-trivial-my-friend", 5, 5, 0, strlen("Mastering-C-is-not-super-trivial-my-friend---"));
    return 0;
}
