#include "libft.h"
#include <stdio.h>
#include <elf.h>

void	fill_header(void *file_content)
{
	unsigned char	e_ident[EI_NIDENT];

	ft_memmove(&e_ident, file_content, EI_NIDENT);
	if (e_ident[EI_MAG0] == ELFMAG0
		&& e_ident[EI_MAG1] == ELFMAG1
		&& e_ident[EI_MAG2] == ELFMAG2
		&& e_ident[EI_MAG3] == ELFMAG3)
	{
		if (e_ident[EI_CLASS] == ELFCLASS32)
			printf("This is a 32bits Elf file\n");
		else if (e_ident[EI_CLASS] == ELFCLASS64)
			printf("This is a 64bits Elf file\n");
		else
			printf("This is not an elf file\n");
	}
}
