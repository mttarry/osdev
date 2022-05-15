#include <kernel/paging.h>


static const pageframe_t startframe = &_kernel_end;

static uint32_t frame_map[BITMAP_SIZE];
static pageframe_t pre_frames[NUM_PAGES];


pageframe_t kalloc_frame_int() {
	uint32_t i = 0;
	uint32_t shift_idx = 0;

	uint32_t four_bytes = frame_map[i];
	while (four_bytes & (1 << shift_idx) != FREE) {
		++shift_idx;
		if (shift_idx > 31) {
			shift_idx = 0;
			four_bytes = frame_map[++i];
		}
	}

	frame_map[i] |= (1 << shift_idx); 
	return ((uint32_t*)startframe + (((i * 32) + shift_idx) * 0x400000));
}

pageframe_t kalloc_frame() {
	static uint8_t allocate = 1;
	static uint8_t pframe = 0;
	pageframe_t ret;

	if (pframe == 20) {
		allocate = 1;
	}

	if (allocate == 1) {
		for (int i = 0; i < 20; ++i) {
			pre_frames[i] = kalloc_frame_int();
		}
		pframe = 0;
		allocate = 0;
	}

	ret = pre_frames[pframe];
	pframe++;
	return(ret);
}

void kfree_frame(pageframe_t a) {
	a = a - startframe;

	if (a == 0) {
		uint32_t index = (uint32_t)a;
		frame_map[index] = FREE;
	}
	else {
		a = a;
		uint32_t index = ((uint32_t)a)/0x400000;
		frame_map[index] = FREE;
	}
}
