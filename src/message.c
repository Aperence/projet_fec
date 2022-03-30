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
#include "block.h"
#include "system.h"
#include "portable_endian.h"


void freeMessage(message_t *m){
    for (int i = 0; i < m->numberBlocks; i++)
    {
        freeBlock(*(m->listBlock + i));
    }
    free(m->listBlock);
    free(m);
}

void printMessage(message_t *m){
    for (int i = 0; i < m->numberBlocks; i++)
    {
        printBlock(*(m->listBlock + i));
    }
}


char **readDir(DIR *directory, const char* directoryname, uint32_t *numberFiles){
    struct dirent *entry;

    *numberFiles = -2;   // remove "." and ".." from directory

    if(directory == NULL){
        printf("Unable to read directory");
        return NULL;
    }

    while ((entry = readdir(directory))){
        (*numberFiles)++;
    }
    closedir(directory);
    
    directory = opendir(directoryname);
    char **filenames = malloc((*(numberFiles))*(sizeof(char *)));

    int i = 0;
    while ((entry = readdir(directory))){
        if (!strcmp(entry->d_name, "..") == 0 && !strcmp(entry->d_name, ".") == 0){
            char *path = malloc(PATH_MAX);
            strcpy(path, directoryname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            *(filenames + i) = path;
            i++;
        }
    }
    
    closedir(directory);

    return filenames;
}

block_t **makeBlockList(uint32_t numberBlocks, uint8_t *message, uint32_t block_size, uint32_t symbol_size, uint32_t redundance_size, uint32_t messageSize){
    block_t **ret = malloc(sizeof(block_t *)*numberBlocks);
    for (uint32_t i = 0; i < numberBlocks; i++) //iterate on blocks
    {
        uint8_t **listSymbol;
        uint32_t number_symbol = block_size+redundance_size;
        if (i == numberBlocks - 1){
            number_symbol = (messageSize - (i*symbol_size*number_symbol)) / symbol_size;
            if (((messageSize - (i*symbol_size*(block_size+redundance_size))) % symbol_size) != 0){
                number_symbol++;
            }
        }
        listSymbol = malloc(number_symbol*sizeof(uint8_t *));
        for (uint32_t j = 0; j < number_symbol; j++) //iterate on symbols
        {
            uint8_t *symbol = malloc(symbol_size);
            if (i== numberBlocks - 1 && j == number_symbol - 1 - redundance_size){ // last symbol of last block => add padding
                memset(symbol, 0, symbol_size);
            }
            memcpy(symbol, (void *) (message + symbol_size*(block_size+redundance_size)*i + symbol_size*j), symbol_size);
            *(listSymbol+j) = symbol;
        }

        block_t *temp = malloc(sizeof(block_t));
        temp->size_block = number_symbol - redundance_size;
        temp->size_redundance = redundance_size;
        temp->size_symbol = symbol_size;
        temp->symb_list = listSymbol;
        *(ret+i) = temp;
    }
    return ret;
}

/*
return -1 if file can't be openned

*/

message_t *openFile(const char *filename){

    FILE *f = fopen(filename, "r");

    uint32_t seed;
    fread(&seed, sizeof(uint32_t), 1, f);
    seed = be32toh(seed);
    uint32_t block_size;
    fread(&block_size, sizeof(uint32_t), 1, f);
    block_size = be32toh(block_size);
    uint32_t symbol_size;
    fread(&symbol_size, sizeof(uint32_t), 1, f);
    symbol_size = be32toh(symbol_size);
    uint32_t redundance_size;
    fread(&redundance_size, sizeof(uint32_t), 1, f);
    redundance_size = be32toh(redundance_size);
    uint64_t message_size;
    fread(&message_size, sizeof(uint64_t), 1, f);
    message_size = be64toh(message_size);


    printf("Seed: %d\nBlock : %d\nRedundance: %d\nSymbol: %d\nMessage: %ld\n", seed, block_size, redundance_size, symbol_size, message_size);
    
    struct stat st;
    stat(filename, &st);

    uint32_t numberBlocks = (st.st_size-24) / (( (block_size) + (redundance_size)) * (symbol_size));

    if ((st.st_size-24) % ( (block_size) + (redundance_size)) * (symbol_size) != 0){
        numberBlocks++;
    }

    uint8_t *fileMessage = malloc(st.st_size - 24);
    fread(fileMessage , 1, st.st_size - 24 , f);
    
    block_t **blockList = makeBlockList(numberBlocks, fileMessage , block_size, symbol_size, redundance_size, st.st_size - 24);
    
    free(fileMessage);

    message_t *message = malloc(sizeof(message_t));

    message->seed = seed;
    message->messageSize = message_size;
    message->numberBlocks = numberBlocks;
    message->listBlock = blockList;

    if (fclose(f)!=0) {
        freeMessage(message); 
        return NULL;
    }
    
    return message;
}

void writeToFile(FILE *outFile, message_t *message, const char*filename){
    uint32_t filenameSize = strlen(filename);
    fwrite(&filenameSize, 4, 1, outFile);
    uint32_t messageSize = message->messageSize;
    fwrite(&(messageSize), 4, 1, outFile);
    fwrite(filename, filenameSize, 1, outFile);

    for (uint32_t i = 0; i < message->numberBlocks; i++)
    {
        for (uint32_t j = 0; j < (*(message->listBlock+i))->size_block ; j++)
        {
            fwrite(*((*(message->listBlock+i))->symb_list + j), 1, (*(message->listBlock+i))->size_symbol , outFile);
            free(*((*(message->listBlock+i))->symb_list + j));
        }
        for (uint32_t j = 0; j < (*(message->listBlock+i))->size_redundance; j++)
        {
            free(*((*(message->listBlock+i))->symb_list + (*(message->listBlock+i))->size_block + j));   //free the redundance symbols
        }
        free((*(message->listBlock+i))->symb_list);
        free(*(message->listBlock+i));
    }
    free(message->listBlock);
    free(message);
}

/**
int main(int argc, char const *argv[])
{
    message_t *m = openFile("./binary_exemple/mediumNoLost.bin");
    for (int i = 0; i < m->numberBlocks; i++)
    {
        printf("Block : %d\n", i);
        printBlock(*(m->listBlock + i));
        freeBlock(*(m->listBlock + i));
    }
    free(m->listBlock);
    free(m);

    readDir("./binary_exemple");
    printf("%d\n", numberFiles);
    for (int i = 0; i < numberFiles; i++)
    {
        printf("%s\n", *(filenames + i));
        free(*(filenames + i));
    }
    free(filenames);
    return 0;
}*/
