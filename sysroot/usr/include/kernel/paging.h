/* INCLUDES */
#include <stdint.h>
#include <stddef.h>


#define FREE 0x00
#define USED 0x01
#define PAGE_DIRECTORY_ENTRIES 0x400
#define PAGE_TABLE_ENTRIES 0x400
#define PAGE_SIZE 4096
#define NUM_PAGES PAGE_DIRECTORY_ENTRIES * PAGE_TABLE_ENTRIES
#define BITMAP_SIZE NUM_PAGES / 0x8 / 0x4 // assuming 32-bit entries in bitmap
#define ERROR NULL

extern uint32_t _kernel_end;

typedef uint32_t* pageframe_t;


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

//extern page_directory_t boot_page_directory; 


// pageframe_t kalloc_frame_int();
// pageframe_t kalloc_frame();
// void kfree_frame(pageframe_t a);