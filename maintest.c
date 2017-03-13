#include "libft.h"
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	char	buff[8];
	int		i;
	int		fd;
	int		x;

	x = 0;
	fd = open("/dev/tty", O_RDWR);
	printf("%d\n", fd);
	ft_bzero(buff, 8);
	write(fd, "\033[6n", 4);
	read(fd, buff, 8);
	i = 1;
	while (buff[i] != ';')
		i++;
	i++;
	while (buff[i] != 'R')
	{
		printf("OK\n");
		if (buff[i] <= '9' && buff[i] >= '0')
		{
			printf("TEST\n");
			x = x * 10 + buff[i] - '0';
		}
		i++;
	}
	printf("x = %d\n", x);
}