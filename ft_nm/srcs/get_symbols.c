#include "ft_nm.h"
#include "libft.h"
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

char	print_symbol_type(uint32_t symbol_type, uint32_t symbol_bind)
{
	static char symbol_list[16] = {'u', 'r', 'r', 't', 't', 'd', 'd', 'd', 'b', '2', '3', 't', '4', '5', 't', 't'}; 
	char		symbol;

	symbol = symbol_list[ELF64_ST_TYPE(symbol_type)];
	if (ELF64_ST_BIND(symbol_bind) == STB_GLOBAL)
		return (symbol - 32);
	else if (ELF64_ST_BIND(symbol_bind) == STB_WEAK)
		return ((ELF64_ST_TYPE(symbol_type) == 0) ? 'w' : 'W');
	return (symbol);
}

void	print_symbols(t_list *symbols, void const *str)
{
	Elf64_Sym *ptr;

	for ( ; symbols != NULL; symbols = symbols->next)
	{
		ptr = symbols->content;
		if (ptr->st_value != 0)
			printf("%016lx", ptr->st_value);
		else
			printf("%*c", 16, ' ');
		//printf(" %c", print_symbol_type(ptr->st_shndx, ptr->st_info));
		printf(" %-3d %-2d %-2d", ptr->st_shndx
									, ELF64_ST_BIND(ptr->st_info), ELF64_ST_TYPE(ptr->st_info));
		printf(" %s\n", (char*)(str + ptr->st_name));
	}
}

int		get_symbols(void *file_content, Elf64_Shdr *shstrtab, Elf64_Shdr *strtab
											, Elf64_Shdr *symtab, Elf64_Ehdr *header)
{
	Elf64_Sym	symbol;
	size_t		offset = symtab->sh_offset;
	size_t		sym_size = (header->e_ident[EI_CLASS] == ELFCLASS64) ? sizeof(Elf64_Sym) : sizeof(Elf32_Sym);
	size_t		symtab_length = symtab->sh_size / symtab->sh_entsize;
	void		*str = file_content + strtab->sh_offset;
	t_list		*lst = NULL;

	for (size_t i = 0; i < symtab_length; i++)
	{
		if (fill_symbol(file_content + offset, &symbol, header->e_ident[EI_CLASS]))
			return (1);
		if (symbol.st_info != STT_SECTION && symbol.st_info != STT_FILE && ft_strlen(str + symbol.st_name) != 0)
		{
			ft_lstappend(&lst, &symbol, sizeof(symbol));
		}
		offset += sym_size;
	}
	sort_list(&lst, str);
	print_symbols(lst, str, shstrtab);
	return (0);
}
