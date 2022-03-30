#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <block.h>

char **filenames;
uint32_t numberFiles;

typedef struct
{
    uint32_t seed;
    uint64_t messageSize;
    uint32_t numberBlocks;
    block_t **listBlock;
} message_t;

void freeMessage(message_t *m);

void printMessage(message_t *m);

char **readDir(DIR *directory, const char* directoryname, uint32_t *numberFiles);

block_t **makeBlockList(uint32_t numberBlocks, uint8_t *message, uint32_t block_size, uint32_t symbol_size, uint32_t redundance_size, uint32_t messageSize);


message_t *openFile(const char *filename);

void writeToFile(FILE *outFile, message_t *message, const char*filename);

#endif /* MESSAGE_H */