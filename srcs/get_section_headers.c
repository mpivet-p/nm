#include "ft_nm.h"
#include <stdio.h>
#include <elf.h>

int			get_section_headers(void const *file_content, Elf64_Ehdr *header)
{
	size_t	offset = header->e_shoff;
	size_t	shift = header->e_shentsize;
	Elf64_Shdr section_hdr;
	Elf64_Shdr shstrtab;
	Elf64_Shdr symtab;
	Elf64_Shdr strtab;

	ft_bzero(&strtab, sizeof(Elf64_Shdr));
	ft_bzero(&symtab, sizeof(Elf64_Shdr));
	if (fill_section_header(file_content, offset + (shift * header->e_shstrndx), &shstrtab, header) != 0)
	{
		return (1);
	}
	for (int i = 0; i < header->e_shnum; i++)
	{
		if (fill_section_header(file_content, offset + (shift * i), &section_hdr, header) != 0)
		{
			return (1);
		}
		if (section_hdr.sh_type == SHT_SYMTAB)
		{
			symtab = section_hdr;
		}
		if (section_hdr.sh_type == SHT_STRTAB
			&& protected_strcmp(".strtab", (char*)file_content + shstrtab.sh_offset + section_hdr.sh_name) == 0)
		{
			strtab = section_hdr;
		}
	}
	if (symtab.sh_name == 0 || strtab.sh_name == 0)
		return (1);
	get_symbols(file_content, &strtab, &symtab);
	return (0);
}
