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
#include "main.h"



void freeMessage(message_t *m){
    for (int i = 0; i < m->numberBlocks; i++)
    {
        freeBlock(*(m->listBlock + i), m->size_redundance);
    }
    free(m->listBlock);
    free(m);
}

void printMessage(message_t *m){
    for (int i = 0; i < m->numberBlocks; i++)
    {
        printBlock(*(m->listBlock + i), m->size_redundance, m->size_symbol);
    }
}


char **readDir(DIR *directory, const char* directoryname){
    struct dirent *entry;

    t_args.numberFiles = -2;   // remove "." and ".." from directory

    if(directory == NULL){
        printf("Unable to read directory");
        return NULL;
    }

    while ((entry = readdir(directory))){
        (t_args.numberFiles)++;
    }
    closedir(directory);
    
    directory = opendir(directoryname);
    char **filenames = malloc(t_args.numberFiles*(sizeof(char *)));

    int i = 0;
    while ((entry = readdir(directory))){
        if (!strcmp(entry->d_name, "..") == 0 && !strcmp(entry->d_name, ".") == 0){
            char *filename = malloc(256);
            strcpy(filename, entry->d_name);
            *(filenames + i) = filename;
            i++;
        }
    }
    
    closedir(directory);

    return filenames;
}

block_t **makeBlockList(uint32_t numberBlocks, uint8_t *message, uint32_t block_size, uint32_t symbol_size, uint32_t redundance_size, uint32_t messageSize, uint32_t padding){
    block_t **ret = malloc(sizeof(block_t *)*numberBlocks);
    uint32_t offset = 0;
    for (uint32_t i = 0; i < numberBlocks; i++) //iterate on blocks
    {
        uint8_t **listSymbol;
        uint32_t number_symbol = block_size+redundance_size;
        if (i == numberBlocks - 1){
                uint32_t lastBlockSize = ((messageSize) % ((block_size) * (symbol_size))) / symbol_size;
                if (((messageSize) % ((block_size) * (symbol_size))) % symbol_size != 0) lastBlockSize++;
                number_symbol = lastBlockSize + redundance_size;
        }
        listSymbol = malloc(number_symbol*sizeof(uint8_t *));
        for (uint32_t j = 0; j < number_symbol; j++) //iterate on symbols
        {
            uint8_t *symbol = malloc(symbol_size);
            if (i == numberBlocks - 1 && j == number_symbol - 1 - redundance_size){ // last symbol of last block => add padding
                memset(symbol, (uint8_t) 0, symbol_size);
                memcpy(symbol, (void *) (message + offset), symbol_size);
                offset += symbol_size;
            }else{
                memcpy(symbol, (void *) (message + offset), symbol_size);
                offset += symbol_size;
            }
            *(listSymbol+j) = symbol;
        }

        block_t *temp = malloc(sizeof(block_t));
        temp->size_block = number_symbol - redundance_size;
        temp->symb_list = listSymbol;
        *(ret+i) = temp;
    }
    return ret;
}

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

    uint32_t lastBlockSize = (message_size) % ((block_size) * (symbol_size));
    uint32_t lastSymbolSize = lastBlockSize % symbol_size;
    uint32_t padding = symbol_size - lastSymbolSize;

    if (args.verbose){
        printf("Seed: %d\nBlock : %d\nRedundance: %d\nSymbol: %d\nMessage: %ld\nPadding : %d\n", seed, block_size, redundance_size, symbol_size, message_size, padding);
    }
    
    struct stat st;
    stat(filename, &st);

    uint32_t numberBlocks = (message_size) / ((block_size) * (symbol_size));

    if (((message_size) % ((block_size) * (symbol_size))) != 0){
        numberBlocks++;
    }

    if (args.verbose){
        printf("Number blocks : %d\nLast Block size : %d\nLast symbol size : %d\nTotal size : %ld\n", numberBlocks, lastBlockSize, lastSymbolSize, message_size + redundance_size*numberBlocks*symbol_size );
    }

    uint8_t *fileMessage = malloc(st.st_size - 24);
    fread(fileMessage , 1, st.st_size - 24 , f);
    
    block_t **blockList = makeBlockList(numberBlocks, fileMessage , block_size, symbol_size, redundance_size, message_size, padding);
 
    free(fileMessage);

    message_t *message = malloc(sizeof(message_t));

    message->seed = seed;
    message->messageSize = message_size;
    message->numberBlocks = numberBlocks;
    message->listBlock = blockList;
    message->size_redundance = redundance_size;
    message->size_symbol = symbol_size;
    message->padding = padding;
    
    if (args.verbose){
        printMessage(message);
    }
    if (fclose(f)!=0) {
        freeMessage(message); 
        return NULL;
    }
    
    return message;
}

void writeToFile(FILE *outFile, message_t *message, const char*filename){
    uint32_t filenameSize = htobe32(strlen(filename));
    fwrite(&filenameSize, 4, 1, outFile);
    uint64_t messageSize =  htobe64(message->messageSize);
    fwrite(&(messageSize), 8, 1, outFile);
    fwrite(filename, strlen(filename), 1, outFile);

    for (uint32_t i = 0; i < message->numberBlocks; i++)
    {
        block_t *b = *(message->listBlock+i);
        for (uint32_t j = 0; j < b->size_block ; j++)
        {
            if (i==message->numberBlocks - 1 && j == b->size_block - 1){
                fwrite(*(b->symb_list + j), 1, message->size_symbol - message->padding , outFile);
            }else{
                fwrite(*(b->symb_list + j), 1, message->size_symbol , outFile);
            }
            free(*(b->symb_list + j));
        }
        for (uint32_t j = 0; j < message->size_redundance; j++)
        {
            free(*(b->symb_list + b->size_block + j));   //free the redundance symbols
        }
        free(b->symb_list);
        free(b);
    }
    free(message->listBlock);
    free(message);
}