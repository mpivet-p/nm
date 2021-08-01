#include "libft.h"
#include "ft_nm.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
//TMP
#include <stdio.h>

void			handle_open_error(const char *s)
{
	ft_putstr_fd("ft_nm: ", STDERR_FILENO);
	if (errno == ENOENT)
	{
		ft_putchar_fd('\'', STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd("': No such file\n", STDERR_FILENO);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("Warning: '", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd("': is not an ordinary file\n", STDERR_FILENO);
	}
}

static int		get_file(const char *filepath)
{
	int		file_size;
	void	*file_content;
	int		fd;

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
		perror("mmap");
	}
	if (fill_header(file_content) != 0)
	{
		ft_putstr_fd("nm: ${FILE} : File format not recognized\n", STDERR_FILENO);
		return (1);
	}
	close(fd);
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
