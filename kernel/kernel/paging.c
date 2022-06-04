#include <kernel/paging.h>


uint32_t *temp_mem;
page_directory_t *new_page_directory;

void *dumb_kmalloc(uint32_t size, int align) {
    void * ret = temp_mem;
    // If it's not aligned, align it first
    if(align && !IS_ALIGNED(ret))
        ret = (void*)ALIGN(ret);
    temp_mem = temp_mem + size;
    return ret;
}


void *virtual2phys(void *virtualaddr, page_directory_t *pd) {
	unsigned long pdindex = PD_INDEX(virtualaddr);
	unsigned long ptindex = PT_INDEX(virtualaddr);
	unsigned long offset = PAGE_OFFSET(virtualaddr);

	page_table_t *table = pd->ref_pt[pdindex];
	if (!table) {
		qemu_printf("virtual2phys: page directory entry does not exist for this vaddr: %p\n", virtualaddr);
		return NULL;
	}
	if (!table->pages[ptindex].present) {
		qemu_printf("virtual2phys: page table entry does not exist for this vaddr: %p\n", virtualaddr);
		return NULL;
	}

	uint32_t paddr = table->pages[ptindex].frame << 12;
	paddr += offset;
	return (void *)(paddr);
}

void clear_pd(page_directory_t *page_directory) {
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
	entry.frame = (uint32_t)page_table_physical_addr >> 12;
	entry.page_size = page_size;
	entry.cache_disable = cache_disable;
	entry.write_through = write_through;
	entry.read_write = 1;
	entry.present = present;

	return entry;
}

page_table_entry_t make_page_table_entry(
		void *page_frame_physical_addr,
		bool cache_disable,
		bool write_through,
		bool present
	) {

	page_table_entry_t entry;
	entry.frame = ((uint32_t)page_frame_physical_addr >> 12);
	entry.cache_disable = cache_disable;
	entry.write_through = write_through;
	entry.read_write = 1;
	entry.present = present;

	return entry;
}

void *page_table_virtual_address(uint16_t page_table_number) {
	// first 10 bits set to 1 --> last PDE entry points to itself
	uint32_t vaddr = 0xFFC00000;

	// next 10 bits point into PD
	vaddr |= (page_table_number << PAGE_OFFSET_BITS);

	return (void*)vaddr;
}


// can do this for the temporary PD so that later one can be created outside of kernel
page_directory_t *page_directory_init(page_directory_t *pd) {
	page_directory_t *page_directory = pd;

	//printf("Page dir at %p\n", page_directory);

	void *page_dir_physical_addr = (void*)page_directory - VMA_BASE;
	//printf("Page dir physical address at %p\n", page_dir_physical_addr);

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


	// map full page table for kernel
	page_table_t *page_table = page_table_virtual_address(KERNEL_PAGE_NUMBER);
	page_directory->ref_pt[KERNEL_PAGE_NUMBER] = page_table;
	//printf("Page table virtual address at %p\n", page_table);
	for (uint16_t i = 0; i < 1024; ++i) {
		uint32_t *frame_addr = allocate_block();
		//printf("Allocating block %p\n", frame_addr);

		page_table_entry_t pte = make_page_table_entry(
								frame_addr,
								false,
								false,
								true);

		page_table->pages[i] = pte;
	}

	return page_directory;
}

void switch_page_directory(page_directory_t *pd_addr, uint32_t phys) {
	uint32_t addr;
	if (!phys) {
		addr = (uint32_t)virtual2phys(pd_addr, pd_addr);
	}
	else
		addr = (uint32_t)pd_addr;

    asm volatile("mov %%cr3, %0" :: "r"(addr));
}


void paging_init() {
	temp_mem = bitmap + bitmap_size;
	new_page_directory = dumb_kmalloc(sizeof(page_directory_t), 1);

	page_directory_t *old_page_dir = page_directory_init(&boot_page_directory);
	new_page_directory = page_directory_init(new_page_directory);

	switch_page_directory(new_page_directory, 0);

}