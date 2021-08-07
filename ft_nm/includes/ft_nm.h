#ifndef FT_NM_H
# define FT_NM_H

#include "libft.h"
#include <elf.h>
#include <stddef.h>

int		check_file(int fd, const char *filepath);
int		check_file_format(void *file_content);
int		fill_header(void *file_content, Elf64_Ehdr *header);
void	print_error(const char *name, const char *opt_str, const char *filename, const char *error);
int		get_section_headers(void *file_content, Elf64_Ehdr *header);
int		protected_memmove(void *dst, const void* src, size_t len, void *max_addr);
int		get_symbols(void *file_content, Elf64_Shdr *strtab, Elf64_Shdr *symtab, Elf64_Ehdr *header);
int		sort_list(t_list **head, void const *str);

#endif
