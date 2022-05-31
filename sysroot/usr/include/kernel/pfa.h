#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <kernel/serial.h>

#define BLOCK_SIZE 4096
#define BLOCKS_PER_GROUP 32
#define ERROR NULL
#define PAGE_OFFSET_BITS 12


#define ISSET(i) ((bitmap[i / BLOCKS_PER_GROUP] >> ((i % BLOCKS_PER_GROUP)) & 1))
#define SETBIT(i) ((bitmap[i / BLOCKS_PER_GROUP] |= (1 << (i % BLOCKS_PER_GROUP))));
#define CLEARBIT(i) ((bitmap[i / BLOCKS_PER_GROUP] &= ~(1 << (i % BLOCKS_PER_GROUP))));
#define BLOCK_ALIGN(addr) ((addr + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1)) 

extern uint8_t* _kernel_end;

void pfa_init(uint32_t mem_size);
uint32_t find_free_block();
uint32_t* allocate_block();
void free_block(uint32_t block);
void pfa_test();