#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "system.h"
#include "gf256_tables.h"
#include "tinymt32.h"
#include "main.h"


void gf_256_full_add_vector(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size){
    for (int i = 0; i < symbol_size; i++)
    {
        *(symbol_1+i) = *(symbol_1+i) ^ *(symbol_2 + i);
    }
}

uint8_t *gf_256_full_add_vector_ret(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size){
    uint8_t *ret = malloc(sizeof(uint8_t)*symbol_size);
    if (ret == NULL){
        fprintf(stderr, "Error with the malloc which was created for the addition value gf 256\n");
        return NULL;
    }
    for (int i = 0; i < symbol_size; i++)
    {
        *(ret+i) = *(symbol_1+i) ^ *(symbol_2 + i);
    }
    return ret;
}

void gf_256_mul_vector(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size){
    for (int i = 0; i < symbol_size; i++)
    {
        *(symbol_1+i) = gf256_mul_table[*(symbol_1+i)][coef];
    }
}

uint8_t *gf_256_mul_vector_ret(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size){
    uint8_t *ret = malloc(sizeof(uint8_t)*symbol_size);
    if (ret == NULL){
        fprintf(stderr, "Error with the malloc which was created for the multiplication value gf 256\n");
        return NULL;
    }
    for (int i = 0; i < symbol_size; i++)
    {
        *(ret+i) = gf256_mul_table[*(symbol_1+i)][coef];
    }
    return ret;
}

void gf_256_inv_vector(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size){
    gf_256_mul_vector(symbol_1, gf256_inv_table[coef], symbol_size);
}

uint8_t *gf_256_inv_vector_ret(uint8_t *symbol_1, uint8_t coef, uint32_t symbol_size){
    return gf_256_mul_vector_ret(symbol_1, gf256_inv_table[coef], symbol_size);
}

uint32_t gf_256_gaussian_elimination(uint8_t **A, uint8_t **B, uint32_t symbol_size, uint32_t system_size){
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
    if (temp == NULL){
        fprintf(stderr, "Error with the malloc of temporary in gaussian elimination\n");
        return -1;
    }
    for (int i = system_size-1; i >=0 ; i--)
    {
        for (int j = i-1; j >=0 ; j--)
        {
            if (*(*(A+j)+i) != 0){
                memcpy(temp, *(B+i), symbol_size * sizeof(uint8_t));
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
    return 0;
}

uint8_t **gen_coefs(tinymt32_t prng, uint32_t nss, uint32_t nrs){
    uint8_t **res = malloc(sizeof(uint8_t *)*nss);
    if (res == NULL){
        fprintf(stderr, "Error with the malloc which was created to store the coefs of gf 256\n");
        return NULL;
    }
    for (int i = 0; i < nss; i++)
    {
        uint8_t *temp = malloc(sizeof(uint8_t)*nrs);
        if (temp == NULL){
            fprintf(stderr, "Error with the malloc which was created to store the coefs of gf 256\n");
            return NULL;
        }
        for (int j = 0; j < nrs; j++)
        {
            uint8_t coef = ( uint8_t ) tinymt32_generate_uint32 (&prng);
            *(temp+j) = coef;
        }
        *(res+i) = temp;
    }
    return res;
}

uint32_t printVector(uint8_t *vector, uint8_t size){
    printf("[ ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", *(vector+i));
    }
    printf(" ]\n");
    return 0;
}

uint32_t freeMatrix(uint8_t **matrix, uint32_t rows){
    for (uint32_t i = 0; i < rows; i++)
    {
        free(*(matrix+i));
    }
    free(matrix);
    return 0;
}

uint32_t printMatrix(uint8_t **matrix, uint32_t n, uint32_t m){
    for (int i = 0; i < n; i++)
    {
        printf("[ ");
        for (int j = 0; j < m; j++)
        {
            printf("%d\t", *(*(matrix+i)+j));
        }
        printf(" ]\n");
    }
    return 0;  
}