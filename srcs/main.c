#include "ft_nm.h"
#include <stdio.h>

static int	check_args(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			fprintf(stderr, "ft_nm: invalid option -- '%s'\nUsage: ft_nm [file(s)]\n "
				"List symbols in [file(s)] (a.out by default).\n", argv[i]);
			return (1);
		}
	}
	return (0);
}

int				main(int argc, char **argv)
{
	void	*file_content = NULL;
	int		file_size = 0;
	int		ret = 0;

	if (check_args(argc, argv))
		return (1);
	for (size_t i = 1; i < (size_t)argc; i++)	// Iterating over the args
	{
		file_content = NULL;
		if (get_file(argv[i], &file_content, &file_size) != 0
			|| do_nm(argv[i], file_content, file_size, argc) != 0)
			ret = 1;
	}
	if (argc == 1)		// If we dont have args, we need to excute nm with "a.out"
	{
		if (get_file("a.out", &file_content, &file_size) != 0
			|| do_nm("a.out", file_content, file_size, 1) != 0)
			ret = 1;
	}
	return (ret);
}
