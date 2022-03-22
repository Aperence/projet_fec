#ifndef BLOCK_H
#define BLOCK_H

typedef struct symbol{
    uint32_t size;
    uint8_t *char_list;
} symbol_t;

typedef struct block
{
    uint32_t size;
    symbol_t *symb_list;
} block_t;

#endif /* BLOCK_H */