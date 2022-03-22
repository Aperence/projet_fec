#include <stdint.h>
#include <stdlib.h>
#include "stack.h"
#include "block.h"
#include "system.h"


/**
* @param : symb : the symbol to verify if lost
* @return : 1 if the symbol is lost
*           0 if the symbol isn't lost
*/
int verifySymbolLost(symbol_t symb){
    for (int i = 0; i < symb.size; i++)
    {
        if(symb.char_list[i] != '0'){
            return 0;
        }
    }
    return 1;
}

/**
* @param : block: the block to verify has a symbol lost
* @return : 1 if a symbol is lost in the block
*           0 if no symbol is lost in the block
*/
stack_t *verifyBlock(block_t block){
    stack_t *s = NULL;
    for (int i = 0; i < block.size; i++)
    {
        if (verifySymbolLost(*(block.symb_list + i)) == 1){
            push(s, i);
        }
    }
    return s;
}

void makeLinearSystem(block_t block, stack_t *lost_indexes, uint32_t seed){

    gen_coefs(seed, lost_indexes->size, block.size);

    // generate system

    //gf_256_gaussian_elimination(A, b, symbol_size, lost_indexes->size);
}

void processFile(block_t *list_block, uint8_t size, uint32_t seed){
    for (uint8_t i = 0; i < size; i++)
    {
        stack_t *s = verifyBlock(*(list_block + i));
        if (s != NULL){
            gen_redondance();
            makeLinearSystem(*(list_block + i), s, seed);
        }
    }

}