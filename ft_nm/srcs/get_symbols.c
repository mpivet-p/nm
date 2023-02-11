#include "ft_nm.h"
#include "libft.h"
#include <elf.h>
#include <stdio.h>

static int	fill_symbol(void const *file_content, Elf64_Sym *symbol, int class)
{
	Elf32_Sym tmp_symbol;

	if (class == ELFCLASS64)
		return (protected_memmove(symbol, file_content, sizeof(Elf64_Sym)));
	if (protected_memmove(&tmp_symbol, file_content, sizeof(Elf32_Sym)) != 0)
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

void	debug_symbols(void const *file_content, Elf64_Sym *symbol)
{	
	Elf64_Ehdr	*header = get_header(NULL);
	Elf64_Shdr	shdr;

	if (fill_section_header(file_content
		, header->e_shoff + (header->e_shentsize * symbol->st_shndx), &shdr, header) != 0)
	{
		return ;
	}
	printf("SHT_ = [%d %x]", shdr.sh_type, shdr.sh_type);
	printf(" SHF_ = 0(%d) 1(%d) 2(%d) 4(%d) 5(%d) 6(%d) 7(%d) 8(%d) 9(%d) 10(%d) 11(%d)"
	, (shdr.sh_flags & SHF_WRITE) ? 1 : 0
	, (shdr.sh_flags & SHF_ALLOC) ? 1 : 0
	, (shdr.sh_flags & SHF_EXECINSTR) ? 1 : 0
	, (shdr.sh_flags & SHF_MERGE) ? 1 : 0
	, (shdr.sh_flags & SHF_STRINGS) ? 1 : 0
	, (shdr.sh_flags & SHF_INFO_LINK) ? 1 : 0
	, (shdr.sh_flags & SHF_LINK_ORDER) ? 1 : 0
	, (shdr.sh_flags & SHF_OS_NONCONFORMING) ? 1 : 0
	, (shdr.sh_flags & SHF_GROUP) ? 1 : 0
	, (shdr.sh_flags & SHF_TLS) ? 1 : 0
	, (shdr.sh_flags & SHF_COMPRESSED) ? 1 : 0
	);
	printf(" STB_ = [%d] [%d] {%d}\n", ELF64_ST_BIND(symbol->st_info), ELF64_ST_TYPE(symbol->st_info), symbol->st_shndx);
}
static char	get_type_from_header(void const *file_content, uint32_t sh_shndx)
{
	Elf64_Ehdr	*header = get_header(NULL);
	Elf64_Shdr	shdr;
	char		type = 0;

	if (fill_section_header(file_content, header->e_shoff + (header->e_shentsize * sh_shndx), &shdr, header) != 0)
		return ('0');
	if (shdr.sh_type == SHT_NOBITS && shdr.sh_flags == (SHF_ALLOC | SHF_WRITE)) //.bss
		type = 'B';
	else if (shdr.sh_type == SHT_PROGBITS)
	{
		switch (shdr.sh_flags)
		{
			case (SHF_ALLOC | SHF_MERGE):
				type = 'R';
				break ;
			case SHF_ALLOC:
				type = 'R';
				break ;
			case (SHF_ALLOC | SHF_WRITE):
				type = 'D';
				break ;
			case (SHF_ALLOC | SHF_EXECINSTR):
				type = 'T';
		}
	}
	else if (shdr.sh_flags == (SHF_ALLOC | SHF_WRITE)
		&& (shdr.sh_type == SHT_INIT_ARRAY || shdr.sh_type == SHT_FINI_ARRAY)) //.init_array or .fini_array
		type = 'D';
	else if (shdr.sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_EXECINSTR)) //.fini_array
		type = 'T';
	else if (shdr.sh_type == SHT_DYNAMIC) //.dynamic
		type = 'D';
	return (type ? type : 'B');
}

static char	get_symbol_type(void const *file_content, Elf64_Sym *symbol)
{
	char		type;

	if (ELF64_ST_BIND(symbol->st_info) == STB_GNU_UNIQUE)
	{
		return('u');
	}
	else if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
	{
		type = 'W';
		if (symbol->st_shndx == SHN_UNDEF)
			type = 'w';
		if (ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
		{
			type = 'v';
		}
	}
	else if (symbol->st_shndx == SHN_UNDEF)
	{
		type = 'U';
	}
	else if (symbol->st_shndx == SHN_ABS)
	{
		type = 'A';
	}
	else if (symbol->st_shndx == SHN_COMMON)
	{
		type = 'C';
	}
	else
	{
		type = get_type_from_header(file_content, symbol->st_shndx);
	}
	if (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL) 
		type += 32;
	if (ELF64_ST_TYPE(symbol->st_info) == STT_LOOS)
		type = 'i';
	return (type);
}


void	print_symbols(void const *file_content, t_list *symbols, uint32_t sh_offset)
{
	Elf64_Sym	*ptr;
	uint8_t		class_padding = 8 * get_header(NULL)->e_ident[EI_CLASS];

	for ( ; symbols != NULL; symbols = symbols->next)
	{
		ptr = symbols->content;
		if (ptr->st_value != 0
			|| ((ELF64_ST_BIND(ptr->st_info) == STB_GLOBAL
				||  ELF64_ST_BIND(ptr->st_info) == STB_LOCAL)
				&& ptr->st_shndx != SHN_UNDEF
				&& ptr->st_shndx != SHN_COMMON
				))
			printf("%0*lx", class_padding, ptr->st_value);
		else
			printf("%*c", class_padding, ' ');
		printf(" %c", get_symbol_type(file_content, ptr));
		if (protect_offset(file_content + sh_offset + ptr->st_name, NULL, NULL) == 0)
			printf(" %s\n", (char*)(file_content + sh_offset + ptr->st_name));
		else
			break ;
//		debug_symbols(file_content, ptr);
	}
}

void	delete_list(t_list *ptr)
{
	t_list *next;
	while (ptr)
	{
		next = ptr->next;
		free(ptr->content);
		ptr->content = NULL;
		free(ptr);
		ptr = next;
	}
}

int		get_symbols(void const *file_content, Elf64_Shdr *strtab, Elf64_Shdr *symtab)
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
		if ((symbol.st_info != STT_SECTION && symbol.st_info != STT_FILE
				&& protect_offset(str + symbol.st_name, 0, 0) == 0 && ft_strlen(str + symbol.st_name) != 0)
			|| (symbol.st_info == STT_NOTYPE && symbol.st_shndx != 0))
		{
			ft_lstappend(&lst, &symbol, sizeof(symbol));
		}
		offset += symtab->sh_entsize;
	}
	lst = merge_sort(lst, str);
	print_symbols(file_content, lst, strtab->sh_offset);
	delete_list(lst);
	lst = NULL;
	return (0);
}
