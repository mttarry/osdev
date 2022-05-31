#include <kernel/paging.h>


uint32_t *temp_mem;

page_directory_t* page_directory;


void *dumb_kmalloc(uint32_t size, int align) {
    void * ret = temp_mem;
    // If it's not aligned, align it first
    if(align && !IS_ALIGNED(ret))
        ret = (void*)ALIGN(ret);
    temp_mem = temp_mem + size;
    return ret;
}


void *virtual2phys(void *virtualaddr) {
	uint32_t pdindex = PD_INDEX(virtualaddr);
	uint32_t ptindex = PT_INDEX(virtualaddr);
	uint32_t offset = PAGE_OFFSET(virtualaddr);

	return 0;
}

void clear_pd() {
	for (int i = 0; i < PAGING_STRUCTURE_SIZE; ++i) {
		// flags: supervisor mode; r/w enable; not present
		page_directory_entry_t entry;
		entry.read_write = 1;
		page_directory->tables[i] = entry;
	}
}

page_directory_entry_t make_page_directory_entry(
		void *page_table_physical_addr,
		bool page_size,
		bool cache_disable,
		bool write_through,
		bool present
	) {

	page_directory_entry_t entry;
	entry.frame = (uint32_t)page_table_physical_addr >> 20;
	entry.page_size = page_size;
	entry.cache_disable = cache_disable;
	entry.write_through = write_through;
	entry.read_write = 1;
	entry.present = present;

	return entry;
}

void *page_table_virtual_address(uint16_t page_table_number) {
	// first 10 bits set to 1 --> last PDE entry points to itself
	void *va = 0xFFC00000;

	// next 10 bits point into PD
	va |= (page_table_number << PAGE_OFFSET_BITS);

	return va;
}

void paging_init() {
	// reserve paging structures after pfa bitmap
	temp_mem = bitmap + bitmap_size;

	page_directory = dumb_kmalloc(sizeof(page_directory_t), 1);

	printf("Page dir at %p\n", page_directory);

	clear_pd();

	void *page_dir_physical_addr = (void*)page_directory - VMA_BASE;
	printf("Page dir physical address at %p\n", page_dir_physical_addr);

	// recursively map the page directory to itself
	// this allows us to access page table w/ virtual addr
	page_directory->tables[1023] = make_page_directory_entry(
									page_dir_physical_addr,
									FOUR_KB,
									false,
									false,
									true );


	// allocate 4096 bytes for kernel page table
	uint32_t *page_table_physical_address = allocate_block();
	page_directory->tables[KERNEL_PAGE_NUMBER] = make_page_directory_entry(
													page_table_physical_address,
													FOUR_KB,
													false,
													false,
													true);








}