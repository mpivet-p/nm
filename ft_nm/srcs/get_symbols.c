#include "ft_nm.h"
#include "libft.h"
#include <elf.h>
#include <stdio.h>

static int	fill_symbol(void const *file_content, Elf64_Sym *symbol, int class)
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

//void	print_symbol_type(Elf64

char *get_section_name(void const *file_content, size_t str_offset, uint32_t sh_index)
{
	Elf64_Ehdr *hdr = get_header(NULL);

	uint32_t name_offset = *(uint32_t*)(file_content + hdr->e_shoff + (hdr->e_shentsize * sh_index));

	return ((char*)(file_content + str_offset + name_offset));
}

void	print_symbols(void const *file_content, t_list *symbols, Elf64_Shdr *shstrtab, uint32_t sh_offset)
{
	Elf64_Sym	*ptr;
	uint8_t		class_padding = 8 * (int)memory(0, 0, 0, FTNM_CLASS);

	for ( ; symbols != NULL; symbols = symbols->next)
	{
		ptr = symbols->content;
		if (ptr->st_value != 0)
			printf("%0*lx", class_padding, ptr->st_value);
		else
			printf("%*c", class_padding, ' ');
		printf(" %-16s", get_section_name(file_content, shstrtab->sh_offset, ptr->st_shndx));
		printf(" %-2d" , ELF64_ST_BIND(ptr->st_info));
		printf(" %s\n", (char*)(file_content + sh_offset + ptr->st_name));
	}
}

int		get_symbols(void const *file_content, Elf64_Shdr *shstrtab, Elf64_Shdr *strtab, Elf64_Shdr *symtab)
{
	Elf64_Sym	symbol;
	t_list		*lst = NULL;
	size_t		symtab_length = symtab->sh_size / symtab->sh_entsize;
	size_t		offset = symtab->sh_offset;
	void		*str = (void*)file_content + strtab->sh_offset;

	for (size_t i = 0; i < symtab_length; i++)
	{
		if (fill_symbol(file_content + offset, &symbol, get_header(NULL)->e_ident[EI_CLASS]))
			return (1);
		if (symbol.st_info != STT_SECTION && symbol.st_info != STT_FILE && ft_strlen(str + symbol.st_name) != 0)
		{
			ft_lstappend(&lst, &symbol, sizeof(symbol));
		}
		offset += symtab->sh_entsize;
	}
	sort_list(&lst, str);
	print_symbols(file_content, lst, shstrtab, strtab->sh_offset);
	return (0);
}
