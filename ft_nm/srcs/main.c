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

static int		get_file(const char *filepath)
{
	int			file_size;
	void		*file_content;
	Elf64_Ehdr	header;
	int			fd;

	if ((fd = open(filepath, O_RDONLY)) < 0)
	{
		handle_open_error(filepath);
		return (1);
	}
	if ((file_size = check_file(fd, filepath)) <= 0)
	{
		close(fd);
		return (1);
	}
	if ((file_content = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		close(fd);
		return (1);
	}
	protected_memmove(0, 0, 0, file_content + file_size); //	Initializing protected memmove with max_addr
	close(fd);
	if (fill_header(file_content, &header) != 0)
	{
		fprintf(stderr, "ft_nm: %s: File format not recognized\n", filepath);
		munmap(file_content, file_size);
		return (1);
	}
	memory((uint64_t)file_content, file_size, (uint64_t)header.e_ident[EI_CLASS], FTNM_SETUP);
	get_header(&header);
	get_section_headers(file_content, &header);
	munmap(file_content, file_size);
	return (0);
}

int				main(int argc, char **argv)
{
	int	ret = 0;

	for (size_t i = 1; i < (size_t)argc; i++)	// Iterating over the args
	{
		if (get_file(argv[i]) != 0)
			ret = 1;
	}
	if (argc == 1)		// If we dont have args, we need to excute nm with "a.out"
	{
		if (get_file("a.out") != 0)
			ret = 1;
	}
	return (ret);
}
