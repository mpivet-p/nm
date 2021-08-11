#ifndef FT_OTOOL_H
# define FT_OTOOL_H

#include "libft.h"
#include <elf.h>
#include <stddef.h>

# define OTOOL_OPT_TEXT 0
# define OTOOL_OPT_HEDR 1
# define OTOOL_OPT_SECT 2
# define OTOOL_OPT_LIST 3

# define COL_NONE		"\e[0m"
# define COL_RED		"\e[31;1m"
# define COL_BLUE		"\e[34;1m"

int		check_file(int fd, const char *filepath);
int		check_file_format(void *file_content);
int		fill_header(void *file_content, Elf64_Ehdr *header);
int		get_section_headers(void *file_content, Elf64_Ehdr *header
			, const char *filename, const char *section_name, int option);
int		protected_memmove(void *dst, const void* src, size_t len, void *max_addr);
void	dump_section(void const *file_content, Elf64_Shdr *shdr, uint8_t ei_class);

/*
**	============================={ BONUS }===================================
*/

void	dump_header(void const *file_content, Elf64_Ehdr *header, const char *filepath);

#endif
