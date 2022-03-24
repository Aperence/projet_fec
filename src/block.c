#include <stdint.h>
#include <stdlib.h>
#include "stack.h"
#include "block.h"
#include "system.h"


uint8_t verifySymbolLost(uint8_t *symb, uint32_t size){
    for (int i = 0; i < size; i++)
    {
        if(*(symb + i) != 0){
            return 0;
        }
    }
    return 1;
}

uint8_t *verifyBlock(block_t block, uint32_t *nb_unk){ 
    uint8_t *s = malloc(sizeof(uint8_t)*block.size_block);
    *nb_unk = 0;
    for (uint32_t i = 0; i < block.size_block; i++)
    {
        uint8_t res = (uint8_t) verifySymbolLost(*((block.symb_list) + i), block.size_symbol);
        *(s+i) = res;
        *nb_unk += res;
    }
    return s;
}

void makeLinearSystem(block_t block, uint8_t *lost_indexes, uint32_t nb_unk, uint32_t seed, uint8_t **A, uint8_t **b){

    uint8_t **coeffs = gen_coefs(seed, nb_unk, block.size_block);

    for (size_t i = 0; i < nb_unk; i++)
    {
        *(A+i) = malloc(sizeof(uint8_t)*nb_unk);
        *(b+i) = gf_256_mul_vector_ret(*(block.symb_list + block.size_block + i), 1, block.size_symbol);  // make a copy of the redundancy
    }

    for (uint32_t i = 0; i < nb_unk; i++)
    {
        uint32_t temp = 0;
        for (uint32_t j = 0; j < block.size_block; j++)
        {
            if (*(lost_indexes + j) == 1){
                *(*(A+i) + temp) = *(*(coeffs + i) + j);
                temp++;
            }else{
                uint8_t *temp_ptr = gf_256_mul_vector_ret(*(block.symb_list + j), *(*(coeffs + i) + j), block.size_symbol);
                uint8_t *temp_ptr2 = *(b+i);
                *(b+i) = gf_256_full_add_vector_ret(temp_ptr, temp_ptr2, block.size_symbol);
                free(temp_ptr);
                free(temp_ptr2);
            }
        }
    }
}

void fillLostSymbol(block_t block, uint8_t **symbols, uint8_t *lost_indexes){
    uint32_t temp = 0;
    for (uint32_t i = 0; i < block.size_block; i++)
    {
        if (*(lost_indexes + i) == 1){
            uint8_t *temp_ptr = *(block.symb_list +i);
            *(block.symb_list +i) = *(symbols + temp);
            free(temp_ptr);
            temp++;
        }
    }
}

void freeMatrix(uint8_t **matrix, uint32_t rows){
    for (uint32_t i = 0; i < rows; i++)
    {
        free(*(matrix+i));
    }
    free(matrix);
}

void processBlock(block_t *list_block, uint8_t size, uint32_t seed){
    for (uint8_t i = 0; i < size; i++)
    {
        uint32_t nb_unk;
        uint8_t *s = verifyBlock(*(list_block + i), &nb_unk);
        if (nb_unk != 0){
            uint8_t **A = malloc(sizeof(uint8_t *) * nb_unk);
            uint8_t **b = malloc(sizeof(uint8_t *) * nb_unk);
            makeLinearSystem(*(list_block + i), s, nb_unk, seed, A, b);
            gf_256_gaussian_elimination(A, b, (*(list_block + i)).size_symbol , nb_unk);
            fillLostSymbol(*(list_block + i), b, s);
            freeMatrix(A, nb_unk);
            free(b);
        }
        free(s);
    }

}
