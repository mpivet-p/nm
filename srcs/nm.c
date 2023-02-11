#include "ft_nm.h"
#include <elf.h>
#include <stdio.h>
#include <sys/mman.h>

int		do_nm(const char *filepath, void *file_content, int file_size, int argc)
{
	Elf64_Ehdr	header;
	int			ret = 0;

	protect_offset(0, file_content, file_content + file_size); //	Initializing protected memmove with max_addr
	if (fill_header(file_content, &header) != 0)
	{
		fprintf(stderr, "ft_nm: %s: file format not recognized\n", filepath);
		munmap(file_content, file_size);
		return (1);
	}
	if (argc > 2)
		printf("\n%s:\n", filepath);
	get_header(&header);
	if (get_section_headers(file_content, &header) != 0)
	{
		fprintf(stderr, "ft_nm: %s: no symbols\n", filepath);
		ret = 1;
	}
	munmap(file_content, file_size);
	return (ret);
}

