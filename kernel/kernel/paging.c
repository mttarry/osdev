#include <kernel/paging.h>


uint32_t *temp_mem;

page_directory_t* page_directory;


void * dumb_kmalloc(uint32_t size, int align) {
    void * ret = temp_mem;
    // If it's not aligned, align it first
    if(align && !IS_ALIGNED(ret))
        ret = (void*)ALIGN(ret);
    temp_mem = temp_mem + size;
    return ret;
}

void clearPageDir() {
	for (int i = 0; i < PAGING_STRUCTURE_SIZE; ++i) {
		// flags: supervisor mode; r/w enable; not present
		page_directory_entry_t entry;
		entry.read_write = 1;
		page_directory->tables[i] = entry;
	}
}

// Map all 4 MB
void addPageTable(uint32_t pageDirIdx) {
	page_table_t page_table;
	for (int i = 0; i < PAGING_STRUCTURE_SIZE; ++i) {
		// frame is page start addr, supervisor level, read/write, present
		page_table.tables[i].frame = (i * 0x1000);
		page_table.tables[i].read_write = 1; 
		page_table.tables[i].present = 1;
	}

	page_directory->tables[pageDirIdx].frame = ?
	page_directory->tables[pageDirIdx].present = 1;
	page_directory->tables[pageDirIdx].read_write = 1;
}


void paging_init() {
	// reserve paging structures after pfa bitmap
	temp_mem = bitmap + bitmap_size;

	page_directory = dumb_kmalloc(sizeof(page_directory_t), 1);

	printf("Page dir at %p\n", page_directory);

	clearPageDir();

	addPageTable(0);



}