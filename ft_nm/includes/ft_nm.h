#ifndef FT_NM_H
# define FT_NM_H

# include "libft.h"
# include <elf.h>
# include <stddef.h>

# define FTNM_SETUP	0x2a
# define FTNM_START	0
# define FTNM_SIZE	1
# define FTNM_CLASS	2

int			check_file(int fd, const char *filepath);
int			check_file_format(void const *file_content);
int			fill_header(void const *file_content, Elf64_Ehdr *header);
int			get_section_headers(void const *file_content, Elf64_Ehdr *header);
int			protected_memmove(void *dst, const void* src, size_t len, void *max_addr);
int			get_symbols(void const *file_content, Elf64_Shdr *shstrtab, Elf64_Shdr *strtab
											, Elf64_Shdr *symtab);
int			sort_list(t_list **head, void const *str);
uint64_t	memory(uint64_t file_content, uint64_t size, uint64_t class, uint8_t action);
Elf64_Ehdr	*get_header(Elf64_Ehdr *hdr);

#endif
