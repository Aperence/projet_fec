#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <block.h>

typedef struct
{
    uint32_t seed;
    uint64_t messageSize;
    block_t *listBlock;
} message_t;

#endif /* MESSAGE_H */