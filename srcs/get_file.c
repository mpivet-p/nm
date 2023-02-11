#include "libft.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>


static void	handle_open_error(const char *s)
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

static void	handle_file_type(struct stat *sb, const char *s)
{
	if (S_ISDIR(sb->st_mode) == 1)
	{
		fprintf(stderr, "ft_nm: Warning: '%s': is a directory\n", s);
	}
	else
	{
		fprintf(stderr, "ft_nm: Warning: '%s': is not an ordinary file\n", s);
	}
}

static  int		check_file(int fd, const char *filepath)
{
	struct stat	sb;
	if (fstat(fd, &sb) < 0)
	{
		return (-1);
	}
	if (S_ISREG(sb.st_mode) != 1)
	{
		handle_file_type(&sb, filepath);
		return (-1);
	}
	return (sb.st_size);
}

int			get_file(const char *filepath, void **file_content, int *file_size)
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
	return (*file_content == NULL || *file_content == MAP_FAILED);
}
