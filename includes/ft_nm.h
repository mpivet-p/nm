#ifndef FT_NM_H
# define FT_NM_H

int		check_file(int fd, const char *filepath);
int		check_file_format(void *file_content);
int		fill_header(void *file_content);

#endif
