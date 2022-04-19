#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>


typedef struct block
{
    uint32_t size_block;
    uint8_t **symb_list;
} block_t;

/**
* @param symb : the symbol to verify if lost
* @param size : the size of the symbol to check
* @return 1 if the symbol is lost
*         0 if the symbol isn't lost
*/
uint8_t verifySymbolLost(uint8_t *symb, uint32_t size);

/**
 * @brief Determine the number and indexes of lost symbols in a block
 * @param block : The block to determine the lost symbols
 * @param nb_unk : A pointer used to stock the number of lost symbols
 * @param size_redundance : the number of redundance symbols in block
 * @param size_symbol : the size of a symbol of the block
 * @return A pointer containing for each symbol of block 1 if the symbol is lost, 
 *                                                       0 if the symbol isn't lost
 * @error : return NULL in case of error
 */
uint8_t *verifyBlock(block_t *block, uint32_t *nb_unk, uint32_t size_redundance, uint32_t size_symbol);

/**
 * @brief Create the linear system to determine the value of lost symbols in block and stock this system in A and b
 * 
 * @param block : The block containing symbols (block_t structure)
 * @param lost_indexes : A pointer containing at position i : 1 if the ith symbol of block is lost, 0 otherwise
 * @param nb_unk : Number of lost symbols
 * @param seed : The seed to generate coefficients
 * @param A : An empty matrix of size nb_unk, used to contain the coefficient of system
 * @param b : An empty matrix of size nb_unk, used to contain the independant terms
 * @param size_symbol : the size of a symbol of block
 *
 * @error : return -1 in case of error
 */
uint32_t makeLinearSystem(block_t *block, uint8_t *lost_indexes, uint32_t nb_unk, uint8_t **coeffs, uint8_t **A, uint8_t **b, uint32_t size_symbol);

/**
 * @brief Replace the lost symbols in block by the symbols recovered using redundancy
 * 
 * @param block : The block which contains lost symbol(s)
 * @param symbols : The symbols recovered by solving the linear system
 * @param lost_indexes : A pointer containing at position i : 1 if the ith symbol of block is lost, 0 otherwise
 * @param size_redundancy : the number of redundancy symbols in the block
 */
uint32_t fillLostSymbol(block_t *block, uint8_t **symbols, uint8_t *lost_indexes, uint32_t size_redundance);

/**
 * @brief Free the pointers in the matrix and the pointer of the matrix
 * 
 * @param matrix : The matrix to free
 * @param rows : Number of rows in the matrix
 */
uint32_t freeMatrix(uint8_t **matrix, uint32_t rows);

/**
 * @brief For each block of list_block, recover the lost symbols in the block and fill them in it
 * 
 * @param list_block : A list of blocks
 * @param size : Size of the list
 * @param seed : Seed used to generate redundancy
 * @param size_redundancy : the number of redundancy symbols in blocks
 * @param size_symbol : the size of a symbol in block list
 *
 * @error : return -1 in case of error
 */
uint32_t processBlock(block_t **list_block, uint8_t size, uint32_t seed, uint32_t size_redundance, uint32_t size_symbol);

/**
 * @brief Free the memory used by block
 * 
 * @param block : the block to free
 * @param size_redundance : the number of redundancy symbols in block
 */
uint32_t freeBlock(block_t *block, uint32_t size_redundance);

/**
 * @brief Print the block to stdout
 * 
 * @param block : the block to print
 * @param size_redundance : the number of redundancy symbols in block
 * @param size_symbol : the size of a symbol in block;
 */
uint32_t printBlock(block_t *block, uint32_t size_redundance, uint32_t size_symbol);

#endif /* BLOCK_H */