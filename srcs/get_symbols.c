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

int		is_sorted(t_list *a, t_list *b, void const *offset)
{
	size_t	i = 0, j = 0;
	while (a->content->)
}

t_list	*sort_symbols_list(t_list *to_sort, void const *offset)
{
	t_list *head = to_sort;
	t_list *ptr;

	to_sort = to_sort->next;
	while (to_sort != NULL)
	{
		ptr = head;
		if (is_sorted(
	}
}

void	print_symbols(t_list *symbols, void const *str)
{
	for (t_list *ptr = symbols; ptr != NULL; ptr = ptr->next)
	{
		if (((Elf64_Sym*)ptr->content)->st_value != 0)
			printf("%016lx", ((Elf64_Sym*)ptr->content)->st_value);
		else
			printf("%*c", 16, ' ');
		printf(" %s\n", (char*)(str + ((Elf64_Sym*)ptr->content)->st_name));
	}
}

int		get_symbols(void *file_content, Elf64_Shdr *strtab, Elf64_Shdr *symtab, Elf64_Ehdr *header)
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
		(void)str;
		if (symbol.st_info != STT_SECTION && symbol.st_info != STT_FILE && ft_strlen(str + symbol.st_name) != 0)
		{
			ft_lstappend(&lst, &symbol, sizeof(Elf64_Sym));
			////printf("%-8x ", symbol.st_shndx);
			//if (symbol.st_value != 0)
			//	printf("%016lx", symbol.st_value);
			//else
			//	//printf("                 ");
			//	printf("%*c", 16, ' ');
			//printf(" %s\n", (char*)(str + symbol.st_name));
		}
		offset += sym_size;
	}
	lst = sort_symbols_list(lst);
	print_symbols(lst, str);
	return (0);
}
