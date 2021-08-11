#include "libft.h"
#include "ft_nm.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <elf.h>
//TMP
#include <stdio.h>

void			handle_open_error(const char *s)
{
	if (errno == ENOENT)
	{
		fprintf(stderr, "ft_nm: '%s': No such file\n", s);
	}
	else if (errno == EACCES)
	{
		fprintf(stderr, "ft_nm: %s: Permission denied\n", s);
	}
	else
	{
		fprintf(stderr, "ft_nm: Warning: '%s': is not an ordinary file\n", s);
	}
}

/*
**	=================================== MMAP CALL ===================================
**	1) NULL			=> let the kernel choose the address to which create the mapping
**	2) file_size	=> map size
**	3) PROT_READ	=> memory protection of the mapping (Page may be read)
**	4) MAP_PRIVATE	=> updates to the mapping ar invisible to other processes mapping the same region
**	5) fd			=> file
**	6) 0			=> offset
**	=================================== --------- ===================================
*/

static int		get_file(const char *filepath, void **file_content, int *file_size)
{
	int			fd;

	if ((fd = open(filepath, O_RDONLY)) < 0)
	{
		handle_open_error(filepath);
		return (1);
	}
	if ((*file_size = check_file(fd, filepath)) > 0)
	{
		*file_content = mmap(NULL, *file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	}
	close(fd);
	return (*file_content == MAP_FAILED);
}

static int		do_nm(const char *filepath, void *file_content, int file_size, int argc)
{
	Elf64_Ehdr	header;
	int			ret = 0;

	protect_offset(0, file_content, file_content + file_size); //	Initializing protected memmove with max_addr
	if (fill_header(file_content, &header) != 0)
	{
		fprintf(stderr, "ft_nm: %s: File format not recognized\n", filepath);
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

int				main(int argc, char **argv)
{
	void	*file_content = NULL;
	int		file_size = 0;
	int		ret = 0;

	for (size_t i = 1; i < (size_t)argc; i++)	// Iterating over the args
	{
		if (get_file(argv[i], &file_content, &file_size) != 0
			|| do_nm(argv[i], file_content, file_size, argc) != 0)
			ret = 1;
	}
	if (argc == 1)		// If we dont have args, we need to excute nm with "a.out"
	{
		if (get_file("a.out", &file_content, &file_size) != 0
			|| do_nm("a.out", file_content, file_size, 1) != 0)
			ret = 1;
	}
	return (ret);
}
