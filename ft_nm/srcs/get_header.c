#include <elf.h>
#include <stddef.h>

Elf64_Ehdr	*get_header(Elf64_Ehdr *hdr)
{
	static Elf64_Ehdr *header;

	if (hdr != NULL)
		header = hdr;
	return (header);
}
