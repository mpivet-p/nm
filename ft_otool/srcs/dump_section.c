#include "ft_otool.h"
#include <elf.h>
#include <stdio.h>

void	dump_section(void const *file_content, Elf64_Shdr *shdr, uint8_t ei_class)
{
	if (protect_offset(file_content + shdr->sh_offset + shdr->sh_size, NULL, NULL) != 0)
		return ;
	for (size_t i = 0; i < shdr->sh_size; i++)
	{
		if (i % 16 == 0)
		{
			if (ei_class == ELFCLASS64)
				printf("%016lx\t", shdr->sh_offset + i);
			else
				printf("%08lx\t", shdr->sh_offset + i);
		}
		printf("%02x ", *(uint8_t*)(file_content + shdr->sh_offset + i));
		if (i % 16 == 15 || i + 1 == shdr->sh_size)
			printf("\n");
	}
}
