#include "libft.h"
#include <stddef.h>

void	print_error(const char *name, const char *opt_str, const char *filename, const char *error)
{
	ft_putstr_fd(name, STDERR_FILENO);
	if (opt_str != NULL)
		ft_putstr_fd(opt_str, STDERR_FILENO);
	if (filename != NULL)
		ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(error, STDERR_FILENO);
}
