#include "libft.h"
#include "ft_otool.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <elf.h>
#include <stdio.h>

void			handle_open_error(const char *s)
{
	if (errno == ENOENT)
	{
		fprintf(stderr, "ft_otool: '%s': No such file\n", s);
	}
	else if (errno == EACCES)
	{
		fprintf(stderr, "ft_otool: '%s': Permission denied\n", s);
	}
	else
	{
		fprintf(stderr, "ft_otool: '%s': is not an ordinary file\n", s);
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

static int		get_file(const char *filepath, const char *st_name, int option)
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
		return (-1);
	}
	close(fd);
	protected_memmove(0, 0, 0, file_content + file_size); //	Initializing protected memmove with max_addr
	if (fill_header(file_content, &header) != 0)
	{
		fprintf(stderr, "ft_otool: %s: is not an object file\n", filepath);
		munmap(file_content, file_size);
		return (1);
	}
	switch (option)
	{
		case OTOOL_OPT_HEDR:
			dump_header(file_content, &header, filepath); break ;
		default:
			get_section_headers(file_content, &header, filepath, st_name, option);
	}
	munmap(file_content, file_size);
	return (0);
}

int				parse_args(int argc, char **argv, char **st_name)
{
	static char	*options[4] = {"-t", "-h", "-s", "-l"};

	if (argc == 1)
		return (-1);
	for (size_t i = 0; i < 4; i++)
	{
		if (ft_strcmp(argv[1], options[i]) == 0)
		{
			if (i == OTOOL_OPT_SECT && argc > 3)
				*st_name = argv[2];
			else if (i == OTOOL_OPT_SECT || argc == 2)
				break ;
			return (i);
		}
	}
	fprintf(stderr, "Usage: otool [-thcsl] <object file> ...\n");
	fprintf(stderr, "\t-t print the text section\n");
	fprintf(stderr, "\t-h print the header\n");
	fprintf(stderr, "\t-s <section name> print a section by it's name\n");
	fprintf(stderr, "\t-l print the list of sections\n");
	return (-1);
}

int				main(int argc, char **argv)
{
	size_t	i;
	char	*st_name = NULL;
	int		options;
	int		ret = 0;

	if ((options = parse_args(argc, argv, &st_name)) != -1)
	{
		i = 2 + (options == OTOOL_OPT_SECT);
		for (; i < (size_t)argc; i++)	// Iterating over the args
		{
			if (get_file(argv[i], st_name, options) != 0)
				ret = 1;
		}
	}
	else
	{
		ret = 1;
	}
	return (ret);
}
