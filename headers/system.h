#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include "tinymt32.h"

/**
 * Print a matrix of size n x m to the standard output
 * @param matrix : the matrix to print
 * @param n : the number of rows of the matrix
 * @param m : the number of columns of the matrix
 */
uint32_t printMatrix(uint8_t **matrix, uint32_t n, uint32_t m);

/**
 * Print a vector of size "size" to the standard output
 * @param vector : the vector to print
 * @param size : the size of the vector
 */
uint32_t printVector(uint8_t *vector, uint8_t size);

/**
 *
 * Add two vectors in a Galois Field 256 in the vector symbol_1
 * @param symbol_1: the first symbol to add
 * @param symbol_2: the second symbol to add
 * @param symbol_size: size of the two symbols (of the same size!)
 */
void gf_256_full_add_vector(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size);

/**
 * @brief Return a new vector containint the addition of the vector symbol_1 and symbol_2
 * @param symbol_1 : The first vector to add
 * @param symbol_2 : The second vector to add
 * @param symbol_size : The size of symbol_1 and symbol_2
 * @return A new vector of size symbol_size containing the addition of symbol_1 and symbol_2
 *
 * @error : return NULL in case of error
 */
uint8_t *gf_256_full_add_vector_ret(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size);

/**
 *
 * Multiply a vector by a coefficient in a Galois Field 256 in place
 * @param symbol: the symbol to multiply
 * @param coef: the coefficient of the scaling
 * @param symbol_size: size of the symbol
 */
void gf_256_mul_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size);

/**
 * @brief Return a new vector containint the multiplication of the vector symbol_1 by coef
 * @param symbol_1 : The vector to mutiply
 * @param coef : The multiplication coefficient
 * @param symbol_size : The size of symbol_1 
 * @return A new vector containing the multiplication of symbol_1 by coef and of size symbol_size
 *
 * @error : return NULL in case of error 
 */
uint8_t *gf_256_mul_vector_ret(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size);

/**
 *
 * Divide a vector in a Galois Field 256 by a coefficient in place
 * @param symbol: the symbol to add
 * @param coef: the dividing coefficient
 * @param symbol_size: size of the two symbols (of the same size!)
 */
void gf_256_inv_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size);

/**
 * @brief Return a new vector containint the division of the vector symbol_1 by coef
 * @param symbol_1 : The vector to divide
 * @param coef : The divisor coefficient
 * @param symbol_size : The size of symbol_1 
 * @return A new vector containing the multiplication of symbol_1 by coef and of size symbol_size
 */
uint8_t *gf_256_inv_vector_ret(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size);

/**
 *
 * Resolve the linear system Ax=b in a Galois Field 256. The result is stored in the independent terms after the resolution
 * @param A: matrix of coefficients
 * @param b: independent terms
 * @param symbol_size: size of the independent terms
 * @param system_size: the size of the system (i.e., number of rows/columns)
 *
 * @error : return -1 in case of error
 */
uint32_t gf_256_gaussian_elimination(uint8_t **A, uint8_t **b, uint32_t symbol_size, uint32_t system_size);

/**
 *
 * Generate all coefficients for a block
 * @param seed: the seed to generate the coefficients
 * @param nss: number of source symbols in a block
 * @param nrs: number of repair symbols in a block
 * @return: a nss * nrs array of coefficients
 *
 * @error : return NULL in case of error
 */
uint8_t **gen_coefs(tinymt32_t prng, uint32_t nss, uint32_t nrs);

#endif /* SYSTEM_H */
