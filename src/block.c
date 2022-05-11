#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block.h"
#include "system.h"
#include "tinymt32.h"
#include "main.h"


uint8_t verifySymbolLost(uint8_t *symb, uint32_t size){
    for (int i = 0; i < size; i++)
    {
        if(*(symb + i) != 0){
            return 0;
        }
    }
    return 1;
}

uint8_t *verifyBlock(block_t *block, uint32_t *nb_unk, uint32_t size_redundance, uint32_t size_symbol){
    uint8_t *s = malloc(sizeof(uint8_t)*((block->size_block)+size_redundance));
    if (s == NULL){
        fprintf(stderr, "Error with the malloc to verify the block\n");
        return NULL;
    }
    *nb_unk = 0;
    for (uint32_t i = 0; i < block->size_block + size_redundance; i++)
    {
        uint8_t res = (uint8_t) verifySymbolLost(*((block->symb_list) + i), size_symbol);
        *(s+i) = res;
        *nb_unk += res;
    }
    return s;
}

uint32_t makeLinearSystem(block_t *block, uint8_t *lost_indexes, uint32_t nb_unk, uint8_t **coeffs, uint8_t **A, uint8_t **b, uint32_t size_symbol){
    for (size_t i = 0; i < nb_unk; i++)
    {
        *(A+i) = malloc(sizeof(uint8_t)*nb_unk);
        if (*(A+i) == NULL){
            fprintf(stderr, "Error with the malloc to make linear system (A matrix)\n");
            return -1;
        }
        *(b+i) = gf_256_mul_vector_ret(*(block->symb_list + block->size_block + i), 1, size_symbol);  // make a copy of the redundancy
        if (*(b+i) == NULL){
            fprintf(stderr, "Error with the malloc to make linear system (b vector)\n");
            return -1;
        }
    }

    for (uint32_t i = 0; i < nb_unk; i++)
    {
        uint32_t temp = 0;
        for (uint32_t j = 0; j < block->size_block ; j++)
        {
            if (*(lost_indexes + j) == 1){
                *(*(A+i) + temp) = *(*(coeffs + i) + j);
                temp++;
            }else{
                uint8_t *temp_ptr = gf_256_mul_vector_ret(*(block->symb_list + j), *(*(coeffs + i) + j), size_symbol);
                if (temp_ptr == NULL){
                    fprintf(stderr, "Error with the malloc to make linear system (temporary pointer)\n");
                    return -1;
                }
                uint8_t *temp_ptr2 = *(b+i);
                *(b+i) = gf_256_full_add_vector_ret(temp_ptr2, temp_ptr, size_symbol);
                if (*(b+i) == NULL){
                    fprintf(stderr, "Error with the malloc to make linear system (temporary pointer 2)\n");
                    return -1;
                }
                free(temp_ptr);
                free(temp_ptr2);
            }
        }
    }
    return 0;
}

uint32_t fillLostSymbol(block_t *block, uint8_t **symbols, uint8_t *lost_indexes, uint32_t size_redundance){
    uint32_t temp = 0;
    for (uint32_t i = 0; i < block->size_block + size_redundance; i++)
    {
        if (*(lost_indexes + i) == 1){
            uint8_t *temp_ptr = *(block->symb_list +i);
            *(block->symb_list +i) = *(symbols + temp);
            free(temp_ptr);
            temp++;
        }
    }
    return 0;
}

uint32_t processBlocks(block_t **list_block, uint8_t size, uint32_t seed, uint32_t size_redundance, uint32_t size_symbol){
    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, seed);
    uint8_t **coeffs = gen_coefs(prng, size_redundance, (*(list_block))->size_block);

    if (coeffs == NULL) return -1;

    for (uint8_t i = 0; i < size; i++)
    {
        uint32_t nb_unk;
        uint8_t *s = verifyBlock(*(list_block + i), &nb_unk, size_redundance, size_symbol);
        if (s == NULL) return -1;

        if (nb_unk != 0){   // some symbols were lost
            if (args.verbose){
                printf(">>>>>>Coeffs\n");
                printMatrix(coeffs, size_redundance, (*(list_block))->size_block);
                printf("\n");
            }

            uint8_t **A = malloc(sizeof(uint8_t *) * nb_unk);
            if (A == NULL){
                fprintf(stderr, "Error with the malloc to create the matrix A for the linear system\n");
                freeMatrix(coeffs, size_redundance);
                free(s);
                return -1;
            }
            uint8_t **b = malloc(sizeof(uint8_t *) * nb_unk);
            if (b == NULL){
                fprintf(stderr, "Error with the malloc to create the matrix b for the linear system\n");
                freeMatrix(coeffs, size_redundance);
                free(s);
                freeMatrix(A, nb_unk);
                return -1;
            }
            if (makeLinearSystem(*(list_block + i), s, nb_unk, coeffs, A, b, size_symbol)<0) {
                freeMatrix(coeffs, size_redundance);
                free(s);
                freeMatrix(A, nb_unk);
                free(b);
                return -1; // if error
            }

            if (args.verbose){
                printf(">>>>>>Linear System\n");
                printMatrix(A, nb_unk, nb_unk);
                printf("\n");
                printMatrix(b, nb_unk, size_symbol);
                printf("\n");
            }

            if (gf_256_gaussian_elimination(A, b, size_symbol , nb_unk)<0){
                freeMatrix(coeffs, size_redundance);
                free(s);
                freeMatrix(A, nb_unk);
                free(b);
                return -1;
            }
            
            if (args.verbose){
                printf(">>>>>>Linear system solution\n");
                printMatrix(b, nb_unk, size_symbol);
                printf("\n");
            }

            fillLostSymbol(*(list_block + i), b, s, size_redundance);
            freeMatrix(A, nb_unk);
            free(b);
            
        }
        free(s);
    }
    freeMatrix(coeffs, size_redundance);
    return 0;
}


uint32_t freeBlock(block_t *block, uint32_t size_redundance){
    freeMatrix(block->symb_list, block->size_block + size_redundance);
    free(block);
    return 0;
}


uint32_t printBlock(block_t *block, uint32_t size_redundance, uint32_t size_symbol){
    for (int i = 0; i < block->size_block + size_redundance; i++)
    {
        printf("[ ");
        for (uint32_t j = 0; j < size_symbol; j++)
        {
            printf("%d ",*(*(block->symb_list + i) + j));
        }
        printf("]\n");
    }
    printf("\n\n");
    return 0;
}