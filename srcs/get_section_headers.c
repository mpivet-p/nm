#include "libft.h"
#include <stdio.h>
#include <elf.h>


static int	fill_section_header(void *file_content, size_t offset, Elf64_Shdr *section_hdr, Elf64_Ehdr *header)
{
	Elf32_Shdr	tmp_hdr;
	if (header->e_ident[EI_CLASS] == ELFCLASS64)
	{
		ft_memmove(section_hdr, file_content + offset, sizeof(Elf64_Shdr));
		return (0);
	}
	ft_memmove(&tmp_hdr, file_content + offset, sizeof(Elf32_Shdr));
	section_hdr->sh_name = tmp_hdr.sh_name;
	section_hdr->sh_type = tmp_hdr.sh_type;
	section_hdr->sh_flags = tmp_hdr.sh_flags;
	section_hdr->sh_addr = tmp_hdr.sh_addr;
	section_hdr->sh_offset = tmp_hdr.sh_offset;
	section_hdr->sh_size = tmp_hdr.sh_size;
	section_hdr->sh_link = tmp_hdr.sh_link;
	section_hdr->sh_info = tmp_hdr.sh_info;
	section_hdr->sh_addralign = tmp_hdr.sh_addralign;
	section_hdr->sh_entsize = tmp_hdr.sh_entsize;
	return (0);
}

void		get_section_headers(void *file_content, Elf64_Ehdr *header, int file_size)
{
	size_t	offset = header->e_shoff;
	size_t	shift = header->e_shentsize;
	Elf64_Shdr section_hdr;
	Elf64_Shdr sh_rtrtab;

(void)file_size;
	fill_section_header(file_content, offset + (shift * header->e_shstrndx), &sh_rtrtab, header);
	printf("There is %d program section headers\n", header->e_shnum);
	for (int i = 0; i < header->e_shnum; i++)
	{
		fill_section_header(file_content, offset + (shift * i), &section_hdr, header);
		printf("[%d] %s", i, (char*)file_content + sh_rtrtab.sh_offset + section_hdr.sh_name);
		if (section_hdr.sh_type == SHT_SYMTAB)
			printf(" SYMTAB");
		printf("\n");
	}
}
