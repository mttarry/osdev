/* INCLUDES */
#include <stdint.h>
#include <stddef.h>
#include <kernel/pfa.h>

extern uint32_t* bitmap;
extern uint32_t bitmap_size;
extern uint32_t *mem_start;

#define PAGE_SIZE 4096

#define ALIGN(addr)				((((uint32_t)(addr) & 0xFFFFF000)) + 0x1000)
#define IS_ALIGNED(addr)    	(((uint32_t)(addr)  & 0xFFF) == 0)


typedef struct page_directory_entry {
	unsigned int present 		: 1;
	unsigned int read_write 	: 1;
	unsigned int user_access 	: 1;
	unsigned int write_through 	: 1;
	unsigned int cache_disable 	: 1;
	unsigned int accessed 		: 1;
	unsigned int reserved_bit 	: 1;
	unsigned int page_size 		: 1;
	unsigned int reserved_hb 	: 4;
	unsigned int frame 			: 20;
} page_directory_entry_t;

typedef struct page_table_entry {
	unsigned int present 		: 1;
	unsigned int read_write 	: 1;
	unsigned int user_access 	: 1;
	unsigned int write_through 	: 1;
	unsigned int cache_disable 	: 1;
	unsigned int accessed 		: 1;
	unsigned int dirty 			: 1;
	unsigned int pat 			: 1;
	unsigned int global 		: 1;
	unsigned int reserved 		: 3;
	unsigned int frame 			: 20;
} page_table_entry_t;

typedef struct page_table {
	page_table_entry_t tables[1024];
} page_table_t;

typedef struct page_directory {
	page_directory_entry_t tables[1024];
} page_directory_t;


void paging_init();