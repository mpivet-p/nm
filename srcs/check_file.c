#include "libft.h"
#include <errno.h>
#include <sys/stat.h>
#include <elf.h>
//TMP
#include <stdio.h>

static void	handle_file_type(struct stat *sb, const char *s)
{
	ft_putstr_fd("ft_nm: Warning: '", STDERR_FILENO);
	if (S_ISDIR(sb->st_mode) == 1)
	{
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd("': is a directory\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd("': is not an ordinary file\n", STDERR_FILENO);
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
