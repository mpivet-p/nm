#include "ft_otool.h"
#include <stdio.h>
#include <stddef.h>
#include <elf.h>

static void	print_bytes(void const *file_content, size_t start, size_t size, const char *color)
{
	size_t end = start + size;

	for (size_t i = start; i < end; i++)
	{
		printf("%s%02x", color, *(uint8_t*)(file_content + i));
		if (i != 0 && i % 16 == 15)
			printf("\e[0m\n");
		else
			printf(" ");
	}
	printf("\e[0m");
}

static void	dump_header_32(void const *file_content, Elf64_Ehdr *header)
{
	printf("%08x\t", 0);
	print_bytes(file_content, 0, 4, "\e[31;1m");
	print_bytes(file_content, 4, 5, COL_NONE);
	print_bytes(file_content, 9, 7, "\e[1m");
	printf("%08x\t", 1);
	print_bytes(file_content, 16, 2, "\e[34;1m");
	print_bytes(file_content, 18, 2, COL_NONE);
	print_bytes(file_content, 20, 4, "\e[31;1m");
	print_bytes(file_content, 24, 4, "\e[34;1m");
	print_bytes(file_content, 28, 4, COL_NONE);
	printf("%08x\t", 2);
	print_bytes(file_content, 32, 4, "\e[31;1m");
	print_bytes(file_content, 36, 4, "\e[34;1m");
	print_bytes(file_content, 40, 2, COL_NONE);
	print_bytes(file_content, 42, 2, "\e[31;1m");
	print_bytes(file_content, 44, 2, COL_NONE);
	print_bytes(file_content, 46, 2, "\e[34;1m");
	printf("%08x\t", 3);
	print_bytes(file_content, 48, 2, "\e[31;1m");
	print_bytes(file_content, 50, 2, COL_NONE);

	printf("\n\e[31;1m(4 bytes) Magic number:\e[0m\t\t.ELF\n");
	printf("(5 bytes) e_ident array:\e[0m\t%d %d %d %d %d\n"
	, header->e_ident[EI_CLASS], header->e_ident[EI_DATA]
	, header->e_ident[EI_VERSION], header->e_ident[EI_OSABI], header->e_ident[EI_ABIVERSION]);
	printf("\e[1m(7 bytes) Padding\e[0m\n");
	printf("\e[34;1m(2 bytes) e_type:\e[0m\t\t%d\n", header->e_type);
	printf("(2 bytes) e_machine:\e[0m\t\t%d\n", header->e_machine);
	printf("\e[31;1m(4 bytes) e_version:\e[0m\t\t%d\n", header->e_version);
	printf("\e[34;1m(4 bytes) e_entry:\e[0m\t\t%d\n", (uint32_t)header->e_entry);
	printf("(4 bytes) e_phoff:\e[0m\t\t%d\n", (uint32_t)header->e_phoff);
	printf("\e[31;1m(4 bytes) e_shoff:\e[0m\t\t%d\n", (uint32_t)header->e_shoff);
	printf("\e[34;1m(4 bytes) e_flags:\e[0m\t\t%d\n", header->e_flags);
	printf("(2 bytes) e_ehsize:\e[0m\t\t%d\n", header->e_ehsize);
	printf("\e[31;1m(2 bytes) e_phentsize:\e[0m\t\t%d\n", header->e_phentsize);
	printf("(2 bytes) e_phnum:\e[0mt\t\t%d\n", header->e_phnum);
	printf("\e[34;1m(2 bytes) e_shentsize:\e[0m\t\t%d\n", header->e_shentsize);
	printf("\e[31;1m(2 bytes) e_shnum:\e[0m\t\t%d\n", header->e_shnum);
	printf("(2 bytes) e_shstrndx:\e[0m\t\t%d\n", header->e_shstrndx);
}

static void	dump_header_64(void const *file_content, Elf64_Ehdr *header)
{
	printf("%016x\t", 0);
	print_bytes(file_content, 0, 4, "\e[31;1m");
	print_bytes(file_content, 4, 5, COL_NONE);
	print_bytes(file_content, 9, 7, "\e[1m");
	printf("%016x\t", 1);
	print_bytes(file_content, 16, 2, "\e[34;1m");
	print_bytes(file_content, 18, 2, COL_NONE);
	print_bytes(file_content, 20, 4, "\e[31;1m");
	print_bytes(file_content, 24, 8, "\e[34;1m");
	printf("%016x\t", 2);
	print_bytes(file_content, 32, 8, COL_NONE);
	print_bytes(file_content, 40, 8, "\e[31;1m");
	printf("%016x\t", 3);
	print_bytes(file_content, 48, 4, "\e[34;1m");
	print_bytes(file_content, 52, 2, COL_NONE);
	print_bytes(file_content, 54, 2, "\e[31;1m");
	print_bytes(file_content, 56, 2, COL_NONE);
	print_bytes(file_content, 58, 2, "\e[34;1m");
	print_bytes(file_content, 60, 2, "\e[31;1m");
	print_bytes(file_content, 62, 2, COL_NONE);

	printf("\n\e[31;1m(4 bytes) Magic number:\e[0m\t\t.ELF\n");
	printf("(5 bytes) e_ident array:\e[0m\t%d %d %d %d %d\n"
	, header->e_ident[EI_CLASS], header->e_ident[EI_DATA]
	, header->e_ident[EI_VERSION], header->e_ident[EI_OSABI], header->e_ident[EI_ABIVERSION]);
	printf("\e[1m(7 bytes) Padding\e[0m\n");
	printf("\e[34;1m(2 bytes) e_type:\e[0m\t\t%d\n", header->e_type);
	printf("(2 bytes) e_machine:\e[0m\t\t%d\n", header->e_machine);
	printf("\e[31;1m(4 bytes) e_version:\e[0m\t\t%d\n", header->e_version);
	printf("\e[34;1m(8 bytes) e_entry:\e[0m\t\t%ld\n", header->e_entry);
	printf("(8 bytes) e_phoff:\e[0m\t\t%ld\n", header->e_phoff);
	printf("\e[31;1m(8 bytes) e_shoff:\e[0m\t\t%ld\n", header->e_shoff);
	printf("\e[34;1m(4 bytes) e_flags:\e[0m\t\t%d\n", header->e_flags);
	printf("(2 bytes) e_ehsize:\e[0m\t\t%d\n", header->e_ehsize);
	printf("\e[31;1m(2 bytes) e_phentsize:\e[0m\t\t%d\n", header->e_phentsize);
	printf("(2 bytes) e_phnum:\e[0mt\t\t%d\n", header->e_phnum);
	printf("\e[34;1m(2 bytes) e_shentsize:\e[0m\t\t%d\n", header->e_shentsize);
	printf("\e[31;1m(2 bytes) e_shnum:\e[0m\t\t%d\n", header->e_shnum);
	printf("(2 bytes) e_shstrndx:\e[0m\t\t%d\n", header->e_shstrndx);
}

void	dump_header(void const *file_content, Elf64_Ehdr *header, const char *filepath)
{
	printf("%s:\nContent of Elf header\n", filepath);

	if (header->e_ident[EI_CLASS] == ELFCLASS64)
		dump_header_64(file_content, header);
	else
		dump_header_32(file_content, header);
}
