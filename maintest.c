#include "libft.h"
#include <stdio.h>

int main(void)
{
	char	buff[8];
	int		i;

	ft_bzero(buff, 8);
	write(1, "\033[6n", 4);
	read(0, buff, 8);
	i = 1;
	//ft_putchar(buff[1]);
	while (buff[i] != 'R')
	{
		//printf("ok\n");
		ft_putchar(buff[i]);
		//ft_putchar('\n');
		i++;
	}
}