#include "ft_nm.h"

int	fill_section_header(void const *file_content, size_t offset, Elf64_Shdr *s_hdr, Elf64_Ehdr *header)
{
	Elf32_Shdr	tmp_hdr;

	if (header->e_ident[EI_CLASS] == ELFCLASS64)
	{
		return (protected_memmove(s_hdr, file_content + offset, sizeof(Elf64_Shdr)));
	}
	if (protected_memmove(&tmp_hdr, file_content + offset, sizeof(Elf32_Shdr)) != 0)
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
