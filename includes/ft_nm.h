#ifndef FT_NM_H
# define FT_NM_H

size_t	check_file(int fd, const char *filepath);
int		check_file_format(void *file_content);
void	fill_header(void *file_content);

#endif
