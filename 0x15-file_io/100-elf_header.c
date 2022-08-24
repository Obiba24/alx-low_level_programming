#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

/**
 * print_addr - prints address
 * @ptr: magic.
 * Return: no return.
 */
void print_addr(char *ptr)
{
	int i;
	int begin;
	char sys;

	printf("  Entry point address:               0x");

	sys = ptr[4] + '0';
	if (sys == '1')
	{
		begin = 26;
		printf("80");
		for (i = begin; i >= 22; i--)
		{
			if (ptr[i] > 0)
				printf("%x", ptr[i]);
			else if (ptr[i] < 0)
				printf("%x", 256 + ptr[i]);
		}
		if (ptr[7] == 6)
			printf("00");
	}

	if (sys == '2')
	{
		begin = 26;
		for (i = begin; i > 23; i--)
		{
			if (ptr[i] >= 0)
				printf("%02x", ptr[i]);

			else if (ptr[i] < 0)
				printf("%02x", 256 + ptr[i]);

		}
	}
	printf("\n");
}

/**
 * print_type - prints type
 * @ptr: magic.
 * Return: no return.
 */
void print_type(char *ptr)
{
	char type = ptr[16];

	if (ptr[5] == 1)
		type = ptr[16];
OBOBOBOB	else
OB		type = ptr[17];
OBOB
	printf("  Type:                              ");
OBOBOBOB	if (type == 0)
OBOB		printf("NONE (No file type)\n");
OBOB	else if (type == 1)
OBOB		printf("REL (Relocatable file)\n");
OBOB	else if (type == 2)
		printf("EXEC (Executable file)\n");
OBOBOB	else if (type == 3)
OBOB		printf("DYN (Shared object file)\n");
	else if (type == 4)
OB		printf("CORE (Core file)\n");
OB	else
OB		printf("<unknown: %x>\n", type);
OB}

/**
OB * print_osabi - prints osabi
 * @ptr: magic.
 * Return: no return.
 */
OBvoid print_osabi(char *ptr)
{
OB	char osabi = ptr[7];

OB	printf("  OS/ABI:                            ");
	if (osabi == 0)
OB		printf("UNIX - System V\n");
	else if (osabi == 2)
OB		printf("UNIX - NetBSD\n");
	else if (osabi == 6)
		printf("UNIX - Solaris\n");
	else
		printf("<unknown: %x>\n", osabi);

	printf("  ABI Version:                       %d\n", ptr[8]);
}


/**
 * print_version - prints version
OBOBOBOBOBOBOBOB * @ptr: magic.
OBOB * Return: no return.
 */
OBOBOBvoid print_version(char *ptr)
{
OBOBOBOBOBOBOBOB	int version = ptr[6];
OBOBOB
OBOB	printf("  Version:                           %d", version);

OBOBOBOBOB	if (version == EV_CURRENT)
		printf(" (current)");

OB	printf("\n");
OBOB}
/**
OB * print_data - prints data
 * @ptr: magic.
OBOB * Return: no return.
 */
OBvoid print_data(char *ptr)
{
OB	char data = ptr[5];

	printf("  Data:                              2's complement");
	if (data == 1)
OB		printf(", little endian\n");

	if (data == 2)
		printf(", big endian\n");
}
OBOBOBOBOB/**
 * print_magic - prints magic info.
OBOB * @ptr: magic.
OBOBOBOBOBOBOB * Return: no return.
OBOBOB */
OBOBOBvoid print_magic(char *ptr)
OBOB{
	int bytes;
OBOBOBOB
	printf("  Magic:  ");

	for (bytes = 0; bytes < 16; bytes++)
OB		printf(" %02x", ptr[bytes]);
OBOBOBOB
	printf("\n");
OB
}
OBOBOBOB
/**
 * check_sys - check the version system.
 * @ptr: magic.
 * Return: no return.
 */
void check_sys(char *ptr)
{
	char sys = ptr[4] + '0';

	if (sys == '0')
		exit(98);

OBOBOBOB	printf("ELF Header:\n");
OB	print_magic(ptr);
OBOB
	if (sys == '1')
OBOBOBOB		printf("  Class:                             ELF32\n");
OBOB
OB	if (sys == '2')
OBOB		printf("  Class:                             ELF64\n");
OBOB
	print_data(ptr);
OBOBOB	print_version(ptr);
	print_osabi(ptr);
	print_type(ptr);
	print_addr(ptr);
OBOBOB}

OBOB/**
 * check_elf - check if it is an elf file.
OBOBOB * @ptr: magic.
 * Return: 1 if it is an elf file. 0 if not.
OB */
int check_elf(char *ptr)
OB{
	int addr = (int)ptr[0];
OB	char E = ptr[1];
	char L = ptr[2];
OB	char F = ptr[3];

	if (addr == 127 && E == 'E' && L == 'L' && F == 'F')
		return (1);
OB
	return (0);
}

/**
 * main - check the code for Holberton School students.
OB * @argc: number of arguments.
 * @argv: arguments vector.
 * Return: Always 0.
OBOBOBOBOBOB */
OBint main(int argc, char *argv[])
OBOBOB{
	int fd, ret_read;
OBOB	char ptr[27];
OBOB
OBOB	if (argc != 2)
	{
OBOB		dprintf(STDERR_FILENO, "Usage: elf_header elf_filename\n");
OBOB		exit(98);
OB	}
OBOB
	fd = open(argv[1], O_RDONLY);
OB
OBOB	if (fd < 0)
	{
OB		dprintf(STDERR_FILENO, "Err: file can not be open\n");
		exit(98);
	}
OBOB
	lseek(fd, 0, SEEK_SET);
OBOBOB	ret_read = read(fd, ptr, 27);

OBOB	if (ret_read == -1)
	{
OB		dprintf(STDERR_FILENO, "Err: The file can not be read\n");
		exit(98);
OB	}

OB	if (!check_elf(ptr))
	{
OB		dprintf(STDERR_FILENO, "Err: It is not an ELF\n");
		exit(98);
OB	}

	check_sys(ptr);
	close(fd);
OB
	return (0);
}
