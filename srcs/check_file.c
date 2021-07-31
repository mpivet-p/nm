#include "libft.h"
#include <errno.h>
#include <sys/stat.h>
#include <elf.h>
//TMP
#include <stdio.h>

static void	handle_file_type(struct stat *sb, const char *s)
{
	ft_putstr_fd("ft_nm: ", STDERR_FILENO);
	ft_putstr_fd("Warning: '", STDERR_FILENO);
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

size_t		check_file(int fd, const char *filepath)
{
	struct stat	sb;
	if (fstat(fd, &sb) < 0)
	{
		perror("fstat");	// TMP
		return (-1); // Handle error
	}
	if (S_ISREG(sb.st_mode) != 1)
	{
		handle_file_type(&sb, filepath);
	}
	return (sb.st_size);
}
