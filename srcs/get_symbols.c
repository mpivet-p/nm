#include "ft_nm.h"
#include <elf.h>
#include <stdio.h>

static int	fill_symbol(void *file_content, Elf64_Sym *symbol, int class)
{
	Elf32_Sym tmp_symbol;

	if (class == ELFCLASS64)
		return (protected_memmove(symbol, file_content, sizeof(Elf64_Sym), 0));
	if (protected_memmove(&tmp_symbol, file_content, sizeof(Elf32_Sym), 0) != 0)
	{
		return (1);
	}
	symbol->st_name = tmp_symbol.st_name;
	symbol->st_value = tmp_symbol.st_value;
	symbol->st_size = tmp_symbol.st_size;
	symbol->st_info = tmp_symbol.st_info;
	symbol->st_other = tmp_symbol.st_other;
	symbol->st_shndx = tmp_symbol.st_shndx;
	return (0);
}

int		get_symbols(void *file_content, Elf64_Shdr *strtab, Elf64_Shdr *symtab, Elf64_Ehdr *header)
{
	Elf64_Sym	symbol;
	size_t		offset = symtab->sh_offset;
	size_t		sym_size = (header->e_ident[EI_CLASS] == ELFCLASS64) ? sizeof(Elf64_Sym) : sizeof(Elf32_Sym);
	size_t		symtab_length = symtab->sh_size / symtab->sh_entsize;
	void		*str = file_content + strtab->sh_offset;

	for (size_t i = 0; i < symtab_length; i++)
	{
		if (fill_symbol(file_content + offset, &symbol, header->e_ident[EI_CLASS]))
			return (1);
		(void)str;
		printf("%s\n", (char*)(str + symbol.st_name));
		offset += sym_size;
	}
	return (0);
}
