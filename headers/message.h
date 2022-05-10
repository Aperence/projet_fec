#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include "block.h"

typedef struct
{
    uint32_t seed;              // seed used to generate redundancy symbols
    uint64_t messageSize;       // total size of message (without redundancy symbols)
    uint32_t numberBlocks;      // number of blocks in listBlock
    uint32_t padding;           //padding of last symbol
    uint32_t size_symbol;       // size of a symbol in listBlocks
    uint32_t size_redundance;   // number of redundancy symbols per block
    block_t **listBlock;
} message_t;

/**
 * @brief Free the memory allocated by message m
 * 
 * @param m : The message to free
 */
uint32_t freeMessage(message_t *m);

/**
 * @brief Print the message m to stdout
 * 
 * @param m : The message to print
 */
uint32_t printMessage(message_t *m);

/**
 * @brief Return a list of the names of files contained in directory. Futhermore, put the number of files in this directory in numberFiles
 * 
 * @param directory : An open directory 
 * @param directoryname : The name of the directory
 * @return An array of the list of names of files contained in directory
 *
 * @error : return NULL in case of error
 */
char **readDir(DIR *directory);

/**
 * @brief Generate the list of block from a message
 * 
 * @param numberBlocks : The number of blocks needed to represent message
 * @param message : The message which we want to extract the blocks
 * @param block_size : The number of symbols per block
 * @param symbol_size : The size of a symbol => the number of char per symbol
 * @param redundance_size : The number of redundance symbol per block
 * @param messageSize : The size of initial message (without redundancy)
 * @param padding : The number of padding for last symbol
 * @return A list of blocks representing "message" containing "block_size" symbols of size "symbol_size" each and "redundancy_size" number of redundancy symbols.
 *         The last symbols is added padding of size "padding".
 *
 * @error : return NULL in case of error
 */
block_t **makeBlockList(uint32_t numberBlocks, uint8_t *message, uint32_t block_size, uint32_t symbol_size, uint32_t redundance_size, uint32_t messageSize, uint32_t padding);

/**
 * @brief Return a message containing all the information needed to recover the content of file
 * 
 * @param filename : The name of the file to process
 * @return A struct containing the list of blocks representing the content of file and informations contained by this fil
 *
 * @error : return NULL in case of error
 */
message_t *openFile(const char *filename);

/**
 * @brief Write the recovered message to outFile
 * 
 * @param outFile : The file stream to write to
 * @param message : The recovered message to write to file
 * @param filename : The name of the processed file from which message was extracted
 *
 * @error : return -1 in case of error
 */
uint32_t writeToFile(FILE *outFile, message_t *message, const char*filename);

#endif /* MESSAGE_H */