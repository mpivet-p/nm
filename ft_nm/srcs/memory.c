#include <stdint.h>
#include "ft_nm.h"

uint64_t	memory(uint64_t file_content, uint64_t size, uint64_t class, uint8_t action)
{
	static uint64_t mem[3];

	if (action == FTNM_SETUP)
	{
		mem[FTNM_START] = file_content;
		mem[FTNM_SIZE] = size;
		mem[FTNM_CLASS] = class;
		return (0);
	}
	return (mem[action]);
}
