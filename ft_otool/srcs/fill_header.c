#include "libft.h"
#include <stdio.h>
#include <elf.h>

static void	get_32_bits_header(Elf64_Ehdr *header, void *file_content)
{
	Elf32_Ehdr tmp_header;

	ft_memmove(&tmp_header, file_content, sizeof(Elf32_Ehdr));
	ft_memmove(header, &tmp_header, EI_NIDENT);
	header->e_type = tmp_header.e_type;
	header->e_machine = tmp_header.e_machine;
	header->e_version = tmp_header.e_version;
	header->e_entry = tmp_header.e_entry;
	header->e_phoff = tmp_header.e_phoff;
	header->e_shoff = tmp_header.e_shoff;
	header->e_flags = tmp_header.e_flags;
	header->e_ehsize = tmp_header.e_ehsize;
	header->e_phentsize = tmp_header.e_phentsize;
	header->e_phnum = tmp_header.e_phnum;
	header->e_shentsize = tmp_header.e_shentsize;
	header->e_shnum = tmp_header.e_shnum;
	header->e_shstrndx = tmp_header.e_shstrndx;
}

static int	check_header(Elf64_Ehdr *header)
{
	if (header->e_ident[EI_VERSION] != EV_CURRENT)
		return (1);
	if (header->e_ident[EI_DATA] != ELFDATA2LSB && header->e_ident[EI_DATA] != ELFDATA2MSB)
		return (1);
	if (header->e_ident[EI_CLASS] != ELFCLASS32 && header->e_ident[EI_CLASS] != ELFCLASS64)
		return (1);
	return (0);
}

int			fill_header(void *file_content, Elf64_Ehdr *header)
{
	unsigned char	e_ident[EI_NIDENT];

	ft_memmove(&e_ident, file_content, EI_NIDENT);			// Copying e_ident array
	if (e_ident[EI_MAG0] == ELFMAG0							// Checking file's magic number (0x07454c46)
		&& e_ident[EI_MAG1] == ELFMAG1
		&& e_ident[EI_MAG2] == ELFMAG2
		&& e_ident[EI_MAG3] == ELFMAG3)
	{
		if (e_ident[EI_CLASS] == ELFCLASS32)
		{
			get_32_bits_header(header, file_content);
		}
		else if (e_ident[EI_CLASS] == ELFCLASS64)
		{
			ft_memmove(header, file_content, sizeof(Elf64_Ehdr));
		}
		return (check_header(header));
	}
	return (1);
}
