#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include "../headers/system.h"
#include "../headers/gf256_tables.h"
#include "../headers/tinymt32.h"


uint8_t *gf_256_full_add_vector(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size){
    uint8_t *r = malloc(sizeof(uint8_t)*symbol_size);
    for (int i = 0; i < symbol_size; i++)
    {
        *(r+i) = *(symbol_1+i) ^ *(symbol_2 + i);
    }
    return *r;
}

uint8_t *gf_256_mul_vector(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size){
    uint8_t *r = malloc(sizeof(uint8_t)*symbol_size);
    for (int i = 0; i < symbol_size; i++)
    {
        *(r+i) = gf256_mul_table[*(symbol_1+i)][coef];
    }
    return *r;
}

uint8_t *gf_256_inv_vector(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size){
    return gf_256_mul_vector(symbol_1, gf256_inv_table[coef], symbol_size);
}

uint8_t subrow(uint8_t *firstRow, uint8_t *secondRow, uint8_t lam1, uint8_t lam2, uint32_t size){
    return gf_256_full_add_vector(firstRow, 
        gf_256_mul_vector(gf_256_inv_vector(secondRow, 
        lam1, size), -lam2, size),
        size);
}

void gf_256_gaussian_elimination(uint8_t **A, uint8_t **B, uint32_t symbol_size, uint32_t system_size){
    uint8_t lam1 = 0;
    uint8_t lam2 = 0;

    uint8_t *temp = malloc(system_size);

    uint8_t *tempSymb = malloc(symbol_size);

    // forward reduction  
    /* 
       [ a  b ]  lam1 = a
       [ c  d ]  lam2 = c
    */
    for (int i = 0; i < system_size; i++)
    {
        lam1 = *(A + i*system_size + i);
        for (int j = i+1; j < system_size; j++)
        {
            lam2 = *(A + j*system_size + i);
            *(A+j) = subrow(*(A+j), *(A+i), lam1, lam2, system_size);
            *(B+j) = subrow(*(B+j), *(B+i), lam1, lam2, symbol_size);
        }
    }
    
    /*
       [ a  b ]  lam1 = a
       [ 0  d ]  lam2 = c
    */
    //backward reduction
    for (int i = system_size - 1 ; i >= 0; i--)
    {
        lam1 = *(A + i*system_size + i);
        *(A+i) = gf_256_inv_vector(*(A+i), lam1 , system_size);
        *(B+i) = gf_256_inv_vector(*(B+i), lam1, symbol_size);
        for (int j = i-1; j >= 0; j--)
        {
            lam2 = *(A + j*system_size + i);
            *(A+j) = subrow(*(A+j), *(A+i), 1, -lam2, system_size);
            *(B+j) = subrow(*(B+j), *(B+i), 1, -lam2, system_size);
        }
    }
    return;
    
}

uint8_t **gen_coefs(uint32_t seed, uint32_t nss, uint32_t nrs){
    uint8_t **res = malloc(sizeof(uint8_t *)*nss);
    uint8_t *temp = malloc(sizeof(uint8_t)*nrs);
    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, seed);
    for (int i = 0; i < nss; i++)
    {
        for (int j = 0; j < nrs; j++)
        {
            uint8_t coef = ( uint8_t ) tinymt32_generate_uint32 (&prng);
            *(temp+j) = coef;
        }
        *(res+i) = temp;
    }
    return res;
}