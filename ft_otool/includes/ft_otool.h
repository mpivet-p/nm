#ifndef FT_OTOOL_H
# define FT_OTOOL_H

#include "libft.h"
#include <elf.h>
#include <stddef.h>

int		check_file(int fd, const char *filepath);
int		check_file_format(void *file_content);
int		fill_header(void *file_content, Elf64_Ehdr *header);
int		get_section_headers(void *file_content, Elf64_Ehdr *header, const char *filename);
int		protected_memmove(void *dst, const void* src, size_t len, void *max_addr);
void	dump_section(void const *file_content, Elf64_Shdr *shdr, uint8_t ei_class);

#endif
