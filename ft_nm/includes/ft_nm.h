#ifndef FT_NM_H
# define FT_NM_H

# include "libft.h"
# include <elf.h>
# include <stddef.h>

# define FTNM_SETUP	0x2a
# define FTNM_START	0
# define FTNM_SIZE	1
# define FTNM_CLASS	2

typedef struct	symbol_sort
{
	Elf64_Sym	*symbol;
	char		*symbol_short_name;
}				t_sym_sort;

int			check_file(int fd, const char *filepath);
int			check_file_format(void const *file_content);
int			fill_header(void const *file_content, Elf64_Ehdr *header);
int			get_section_headers(void const *file_content, Elf64_Ehdr *header);
int			get_symbols(void const *file_content, Elf64_Shdr *strtab
											, Elf64_Shdr *symtab);
t_list		*merge_sort(t_list *head, void const *str);
Elf64_Ehdr	*get_header(Elf64_Ehdr *hdr);
int			fill_section_header(void const *file_content, size_t offset, Elf64_Shdr *s_hdr, Elf64_Ehdr *header);
int			protect_offset(void const *ptr, void const *min, void const *max);
int			protected_strcmp(const char *s1, const char *s2);
int			protected_memmove(void *dst, const void* src, size_t len);

#endif
