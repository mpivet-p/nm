#include <stdio.h>
#include <stdint.h>

int		check_file_format(void *file_content)
{
	uint32_t	header;

	header = *(uint32_t*)file_content;
	printf("0x%x\n", header);
	return (1);
}
