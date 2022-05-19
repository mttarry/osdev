#include <kernel/pfa.h>



uint32_t *bitmap = (uint32_t*) &(_kernel_end);
uint32_t bitmap_size;
uint32_t total_blocks;
uint32_t *mem_start;

// clear bitmap
void pfa_init(uint32_t mem_size) {

	total_blocks = mem_size / BLOCK_SIZE;
	bitmap_size = total_blocks / BLOCKS_PER_GROUP;

	memset(bitmap, 0, bitmap_size);	

	mem_start = (uint32_t*)BLOCK_ALIGN(((uint32_t)bitmap + bitmap_size));
}


uint32_t find_free_block() {
	for (uint32_t i = 0; i < total_blocks; ++i) {
		if (!ISSET(i)) {
			return i;
		}
	}

	return -1;
}


uint32_t allocate_block() {
	uint32_t free_block = find_free_block();
	SETBIT(free_block);
	return free_block;
}

void free_block(uint32_t block) {
	CLEARBIT(block);
}

void pfa_test() {
	pfa_init(0x10000000);

	qemu_printf("\n");
	uint32_t first_free = find_free_block();
	qemu_printf("First free block is %d\n", first_free);

	void *ret = (void*)allocate_block(); 
	qemu_printf("First allocated block at address %p\n", ret);

	uint32_t second_free = find_free_block();
	qemu_printf("First free block is %d\n", second_free);

	ret = (void*)allocate_block();
	qemu_printf("First allocated block at address %p\n", ret);

	free_block(first_free);
	first_free = find_free_block();
	qemu_printf("First free block is %d\n", first_free);


}