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


void gf_256_full_add_vector(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size){
    for (int i = 0; i < symbol_size; i++)
    {
        *(symbol_1+i) = *(symbol_1+i) ^ *(symbol_2 + i);
    }
}

void gf_256_mul_vector(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size){
    for (int i = 0; i < symbol_size; i++)
    {
        *(symbol_1+i) = gf256_mul_table[*(symbol_1+i)][coef];
    }
}

void gf_256_inv_vector(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size){
    gf_256_mul_vector(symbol_1, gf256_inv_table[coef], symbol_size);
}


void printMatrix(uint8_t **matrix, uint32_t n, uint32_t m){
    for (int i = 0; i < n; i++)
    {
        printf("[ ");
        for (int j = 0; j < m; j++)
        {
            printf("%d\t", *(*(matrix+i)+j));
        }
        printf(" ]\n");
    }
    
}

void gf_256_gaussian_elimination(uint8_t **A, uint8_t **B, uint32_t symbol_size, uint32_t system_size){
    uint8_t factor = 0;
    // forward reduction  
    /* 
       [ a  b ]  
       [ c  d ]  
    */
    for (int i = 0; i < system_size; i++)
    {
        for (int j = i+1; j < system_size; j++)
        {
            factor = gf256_mul_table[*(*(A+j)+i)][gf256_inv_table[*(*(A+i)+i)]];
            gf_256_mul_vector(*(A+i), factor, system_size);
            gf_256_full_add_vector(*(A+j), *(A+i), system_size);

            gf_256_mul_vector(*(B+i), factor, symbol_size);
            gf_256_full_add_vector(*(B+j), *(B+i), symbol_size);
        }
    }

    /*
       [ a  b ] 
       [ 0  d ]
    */
    //backward reduction
    uint8_t *temp = malloc(sizeof(uint8_t)*symbol_size);
    for (int i = system_size-1; i >=0 ; i--)
    {
        for (int j = i-1; j >=0 ; j--)
        {
            if (*(*(A+j)+i) != 0){
                memcpy(temp, *(B+i), symbol_size * sizeof(u_int8_t));
                factor = gf256_mul_table[*(*(A+j)+i)][gf256_inv_table[*(*(A+i)+i)]];
                *(*(A+j)+i) = (*(*(A+j)+i)) ^ gf256_mul_table[*(*(A+i)+i)][factor];
                gf_256_mul_vector(temp, factor, symbol_size);
                gf_256_full_add_vector(*(B+j), temp, symbol_size);
            } 
        }
        factor = gf256_inv_table[*(*(A+i)+i)];
        *(*(A+i)+i) = gf256_mul_table[*(*(A+i)+i)][factor];
        gf_256_mul_vector(*(B+i), factor, symbol_size);
    }
    free(temp);
}

uint8_t **gen_coefs(uint32_t seed, uint32_t nss, uint32_t nrs){
    uint8_t **res = malloc(sizeof(uint8_t *)*nss);
    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, seed);
    for (int i = 0; i < nss; i++)
    {
        uint8_t *temp = malloc(sizeof(uint8_t)*nrs);
        for (int j = 0; j < nrs; j++)
        {
            uint8_t coef = ( uint8_t ) tinymt32_generate_uint32 (&prng);
            *(temp+j) = coef;
        }
        *(res+i) = temp;
    }
    return res;
}

void printVector(uint8_t *vector, uint8_t size){
    printf("[ ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", *(vector+i));
    }
    printf(" ]\n");
}

/**
int main(int argc, char const *argv[])
{

    const int size = 5;
    uint8_t **A = gen_coefs(14, size, size);
    uint8_t **B = gen_coefs(14, size, size);

    gf_256_gaussian_elimination(A, B, size, size);
    

    for (int i = 0; i < size; i++)
    {
        free(*(A+i));
        free(*(B+i));
    }
    free(A);
    free(B);
}*/
