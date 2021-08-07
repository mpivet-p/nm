#include "libft.h"
#include <stdio.h>

int		protected_memmove(void *dst, const void* src, size_t len, void *max_addr)
{
	static void	*addr;

	if (max_addr != NULL)
	{
		addr = max_addr;
		return (-1);
	}
	if (src + len > addr)
	{
		return (1);
	}
	ft_memmove(dst, src, len);
	return (0);
}
