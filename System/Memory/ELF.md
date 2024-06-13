linux 下可执行二进制文件的格式称为 ELF, 执行时, 首先将该文件载入到内存 `mm_struct` 中.

[目标文件](../../Compiler/linking/目标文件.md)

```c
static int load_elf_binary(struct linux_binprm *bprm)
{
	...
  // set mmap_base
  setup_new_exec(bprm);

	...
  // setup vm_area_struct of stack
  // mm->start_stack --> bottom of stack
  // mm->arg_start --> bottom of stack, start of main arguments
  retval = setup_arg_pages(bprm, randomize_stack_top(STACK_TOP),
         executable_stack);

	...
  // map .text, .data, .bss in elf file to mm
  error = elf_map(bprm->file, load_bias + vaddr, elf_ppnt,
        elf_prot, elf_flags, total_size);

	...
 // setup vm_area_struct of heap, 
 // set current->mm->start_brk = current->mm->brk, 
  retval = set_brk(elf_bss, elf_brk, bss_prot);

	...
  // map dependent .so to mmap.
  elf_entry = load_elf_interp(&loc->interp_elf_ex,
              interpreter,
              &interp_map_addr,
              load_bias, interp_elf_phdata);

	...
  // setup mm_struct 
  current->mm->end_code = end_code;
  current->mm->start_code = start_code;
  current->mm->start_data = start_data;
  current->mm->end_data = end_data;
  current->mm->start_stack = bprm->p;
	
	...
}
```