#include "libft.h"

int		protect_offset(void const *ptr, void const *min, void const *max)
{
	static void	*addr_min;
	static void	*addr_max;

	if (min != NULL && max != NULL)
	{
		addr_min = (void*)min;
		addr_max = (void*)max;
		return (1);
	}
	if (ptr > addr_max || ptr < addr_min)
	{
		return (1);
	}
	return (0);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	if (protect_offset(s2 + ft_strlen(s1), NULL, NULL) != 0)
		return (-420);
	return (ft_strcmp(s1, s2));
}

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
