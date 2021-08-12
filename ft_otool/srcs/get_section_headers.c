#include "libft.h"
#include "ft_otool.h"
#include <stdio.h>
#include <elf.h>


static int	fill_section_header(void *file_content, size_t offset, Elf64_Shdr *section_hdr, Elf64_Ehdr *header)
{
	Elf32_Shdr	tmp_hdr;

	if (header->e_ident[EI_CLASS] == ELFCLASS64)
	{
		return (protected_memmove(section_hdr, file_content + offset, sizeof(Elf64_Shdr)));
	}
	if (protected_memmove(&tmp_hdr, file_content + offset, sizeof(Elf32_Shdr)) != 0)
		return (1);
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

int			get_section_headers(void *file_content, Elf64_Ehdr *header
				, const char *filename, const char *section_name, int option)
{
	Elf64_Shdr	section_hdr;
	Elf64_Shdr	shstrtab;
	size_t		offset = header->e_shoff;
	size_t		shift = header->e_shentsize;
	char		*st_name;

	if (fill_section_header(file_content, offset + (shift * header->e_shstrndx), &shstrtab, header) != 0)
	{
		return (1);
	}
	printf("%s:\n", filename);
	for (int i = 0; i < header->e_shnum; i++)
	{
		if (fill_section_header(file_content, offset + (shift * i), &section_hdr, header) != 0)
			return (1);
		st_name = (char*)file_content + shstrtab.sh_offset + section_hdr.sh_name;
		if ((option == OTOOL_OPT_TEXT
				&& section_hdr.sh_type == SHT_PROGBITS
				&& protected_strcmp(".text", st_name) == 0)
			|| (section_name != NULL && protected_strcmp(section_name, st_name) == 0))
		{
			printf("Contents of (%s) section\n", st_name);
			dump_section(file_content, &section_hdr, header->e_ident[EI_CLASS]);
			break ;
		}
		else if (option == OTOOL_OPT_LIST)
		{
			printf("%0*lx %-21s   (%ld bytes)\n", 8 * header->e_ident[EI_CLASS], section_hdr.sh_offset, st_name, section_hdr.sh_size);
		}
	}
	return (0);
}
