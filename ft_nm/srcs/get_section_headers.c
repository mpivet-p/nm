#include "libft.h"
#include "ft_nm.h"
#include <stdio.h>
#include <elf.h>


int	fill_section_header(void const *file_content, size_t offset, Elf64_Shdr *s_hdr, Elf64_Ehdr *header)
{
	Elf32_Shdr	tmp_hdr;

	if (header->e_ident[EI_CLASS] == ELFCLASS64)
	{
		return (protected_memmove(s_hdr, file_content + offset, sizeof(Elf64_Shdr), 0));
	}
	if (protected_memmove(&tmp_hdr, file_content + offset, sizeof(Elf32_Shdr), 0) != 0)
		return (1);
	s_hdr->sh_name = tmp_hdr.sh_name;
	s_hdr->sh_type = tmp_hdr.sh_type;
	s_hdr->sh_flags = tmp_hdr.sh_flags;
	s_hdr->sh_addr = tmp_hdr.sh_addr;
	s_hdr->sh_offset = tmp_hdr.sh_offset;
	s_hdr->sh_size = tmp_hdr.sh_size;
	s_hdr->sh_link = tmp_hdr.sh_link;
	s_hdr->sh_info = tmp_hdr.sh_info;
	s_hdr->sh_addralign = tmp_hdr.sh_addralign;
	s_hdr->sh_entsize = tmp_hdr.sh_entsize;
	return (0);
}

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
	//	printf("[%.2d] ", i);
	//	printf("%-20s", (char*)file_content + shstrtab.sh_offset + section_hdr.sh_name);
	//	printf(" %12d", section_hdr.sh_type);
	//	printf(" %12d", section_hdr.sh_info);
	//	printf(" %12ld\n", section_hdr.sh_flags);
	//	if (section_hdr.sh_flags == (SHF_MERGE | SHF_STRINGS))
	//		printf("DEBUG\n");
		if (section_hdr.sh_type == SHT_SYMTAB)
		{
			symtab = section_hdr;
		}
		if (section_hdr.sh_type == SHT_STRTAB
			&& ft_strcmp((char*)file_content + shstrtab.sh_offset + section_hdr.sh_name, ".strtab") == 0)
		{
			strtab = section_hdr;
		}
	}
	if (symtab.sh_name == 0 || strtab.sh_name == 0)
		return (1);
	get_symbols(file_content, &strtab, &symtab);
	return (0);
}
