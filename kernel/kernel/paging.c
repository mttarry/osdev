#include <kernel/paging.h>

uint32_t *temp_mem;

page_directory_t* page_directory;

extern page_directory_t* boot_page_directory;

void * dumb_kmalloc(uint32_t size, int align) {
    void * ret = temp_mem;
    // If it's not aligned, align it first
    if(align && !IS_ALIGNED(ret))
        ret = (void*)ALIGN(ret);
    temp_mem = temp_mem + size;
    return ret;
}


void paging_init() {
	// reserve paging structures after pfa bitmap
	temp_mem = bitmap + bitmap_size;

	page_directory = dumb_kmalloc(sizeof(page_directory_t), 1);

	printf("Page dir at %p\n", boot_page_directory);

	//memset(page_directory, 0, 10);



}