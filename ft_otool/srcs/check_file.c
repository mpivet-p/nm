#include "libft.h"
#include <errno.h>
#include <sys/stat.h>
#include <elf.h>
//TMP
#include <stdio.h>

static void	handle_file_type(struct stat *sb, const char *s)
{
	if (S_ISDIR(sb->st_mode) == 1)
	{
		fprintf(stderr, "ft_otool: '%s': is a directory\n", s);
	}
	else
	{
		fprintf(stderr, "ft_otool: '%s': is not an ordinary file\n", s);
	}
}

int			check_file(int fd, const char *filepath)
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
