#ifndef FT_NM_H
# define FT_NM_H

#include <elf.h>

int		check_file(int fd, const char *filepath);
int		check_file_format(void *file_content);
int		fill_header(void *file_content, Elf64_Ehdr *header);
void	print_error(const char *name, const char *opt_str, const char *filename, const char *error);
void	get_section_headers(void *file_content, Elf64_Ehdr *header, int file_size);

#endif
