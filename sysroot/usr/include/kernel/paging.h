/* INCLUDES */
#include <stdint.h>
#include <stddef.h>
#include <kernel/pfa.h>
#include <stdbool.h>


extern uint32_t* bitmap;
extern uint32_t bitmap_size;
extern uint32_t *mem_start;


#define PAGE_SIZE 					4096
#define PAGING_STRUCTURE_SIZE 		1024
#define VMA_BASE 					0xC0000000
#define KERNEL_PAGE_NUMBER 			768

#define FOUR_KB 					0
#define FOUR_MB 					1

#define ALIGN(addr)			   		((((uint32_t)(addr) & 0xFFFFF000)) + 0x1000)
#define IS_ALIGNED(addr)       		(((uint32_t)(addr)  & 0xFFF) == 0)

#define PD_INDEX(addr)         		((uint32_t)addr >> 22)
#define PT_INDEX(addr)         		(((uint32_t)addr >> 12) & 0x3FF)
#define PAGE_OFFSET(addr)      		((uint32_t)addr & 0xFFF)

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
	page_table_entry_t pages[1024];
} page_table_t;

typedef struct page_directory {
	page_directory_entry_t tables[1024];
	page_table_t *ref_pt[1024]; // so we can access page tables directly
} page_directory_t;

extern page_directory_t boot_page_directory;

page_directory_t *page_directory_init();
void paging_init();