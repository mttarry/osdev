# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Some constants for loading higher half kernel
.set VM_BASE,     0xC0000000
.set PDE_INDEX,   (VM_BASE >> 22)
.set PSE_BIT,     0x00000010
.set PG_BIT,      0x80000000
.set WP_BIT,      0x00010000

# Declare a multiboot header that marks the program as a kernel.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096


# Allocate the initial stack.
.section .bootstrap_stack, "aw", @nobits
.align 4
stack_bottom:
.skip 16384 # 16 KiB
stack_top:


# The kernel entry point.
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	# Map the first 4mb physical memory to first 4mb virtual memory.
	movl $(0x00000083), boot_page_directory - VM_BASE
	movl $(0x00000083), boot_page_directory - VM_BASE + PDE_INDEX * 4

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_page_directory - VM_BASE), %ecx
	movl %ecx, %cr3

	# Enable 4 MiB pages
	mov %cr4, %ecx
	orl PSE_BIT, %ecx
	movl %ecx, %cr4

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl PG_BIT | WP_BIT, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump. 
	lea higher_half, %ecx
	jmp *%ecx

.section .text

higher_half:
	# Unmap the identity mapping as it is now unnecessary. 
	movl $0x00000000, boot_page_directory + 0

	# Reload crc3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set up the stack.
	mov $stack_top, %esp

	# Enter the kernel.
	call kernel_main

	# Infinite loop if the system has nothing more to do.
_loop:
	jmp _loop
