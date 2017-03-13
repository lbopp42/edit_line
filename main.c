/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 09:12:13 by lbopp             #+#    #+#             */
/*   Updated: 2017/03/03 09:12:16 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define OTHER_KEY 0
#define KEY_CODE_ENTER 1
#define KEY_CODE_RIGHT 2
#define KEY_CODE_TOP 3
#define KEY_CODE_DOWN 4
#define KEY_CODE_LEFT 6
#include <stdio.h>
#include <term.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void	init_term(void);
void	default_term(void);

char	*g_line;
typedef struct	s_funct
{
	int	key_code;
	int	(*f)(const char *buff);
}				t_funct;
typedef struct	s_manage
{
	int		key_code;
	void	(*f)(int*);
}				t_manage;

void	clean_exit(void)
{
	default_term();
	exit(0);
}

int	put_my_char(int c)
{
	write(1, &c, 1);
	return (1);
}

int	key_is_arrow_right(const char *buff)
{
	static char	enter_key[] = {27, 91, 67, 0, 0, 0, 0, 0};

	if (!ft_strcmp(buff, enter_key))
		return (1);
	return (0);
}

int	key_is_arrow_top(const char *buff)
{
	static char	enter_key[] = {27, 91, 65, 0, 0, 0, 0, 0};

	if (!ft_strcmp(buff, enter_key))
		return (1);
	return (0);
}

int	key_is_arrow_down(const char *buff)
{
	static char	enter_key[] = {27, 91, 66, 0, 0, 0, 0, 0};

	if (!ft_strcmp(buff, enter_key))
		return (1);
	return (0);
}

int	key_is_arrow_left(const char *buff)
{
	static char	enter_key[] = {27, 91, 68, 0, 0, 0, 0, 0};

	if (!ft_strcmp(buff, enter_key))
		return (1);
	return (0);
}

int	key_is_enter(const char *buff)
{
	static char	enter_key[] = {10, 0, 0, 0, 0, 0, 0, 0};

	if (!ft_strcmp(buff, enter_key))
		return (1);
	return (0);
}

int	get_key(char *buff)
{
	static const t_funct	funct[] = {
		{KEY_CODE_ENTER, &key_is_enter},
		{KEY_CODE_RIGHT, &key_is_arrow_right},
		{KEY_CODE_TOP, &key_is_arrow_top},
		{KEY_CODE_DOWN, &key_is_arrow_down},
		{KEY_CODE_LEFT, &key_is_arrow_left},
		{0, 0}
	};
	int						i;

	i = 0;
	while (funct[i].key_code)
	{
		if (funct[i].f(buff))
			return (funct[i].key_code);
		i++;
	}
	return (0);
}

void	enter_funct(int *curs)
{
	*curs = *curs;
	printf("\nline = [%s]\n", g_line);
}

void	arrow_right_funct(int *curs)
{
	if (*curs < ft_strlen(g_line))
	{
		*curs += 1;
		tputs(tgetstr("nd", NULL), 1, &put_my_char);
	}
}

void	arrow_left_funct(int *curs)
{
	if (*curs)
	{
		*curs -= 1;
		tputs(tgetstr("le", NULL), 1, &put_my_char);
	}
}

void	arrow_top_funct(void)
{
	tputs(tgetstr("up", NULL), 1, &put_my_char);
}

void	arrow_down_funct(void)
{
	tputs(tgetstr("sf", NULL), 1, &put_my_char);
}

int		get_y_position(void)
{
	char	buff[8];
	int		i;
	int		y;

	y = 0;
	ft_bzero(buff, 8);
	write(1, "\033[6n", 4);
	read(0, buff, 8);
	i = 1;
	while (buff[i] != ';')
	{
		if (buff[i] <= '9' && buff[i] >= '0')
			y = y * 10 + buff[i] - '0';
		i++;
	}
	return (y);
}

int		get_x_position(void)
{
	char	buff[8];
	int		i;
	int		fd;
	int		x;

	x = 0;
	ft_bzero(buff, 8);
	write(1, "\033[6n", 4);
	read(0, buff, 8);
	i = 1;
	while (buff[i] != ';')
		i++;
	i++;
	while (buff[i] != 'R')
	{
		if (buff[i] <= '9' && buff[i] >= '0')
			x = x * 10 + buff[i] - '0';
		i++;
	}
	return (x);
}

void	add_char_to_line(char c, int *curs)
{
	struct winsize w;
	int				x;
	int				y;

	ioctl(0, TIOCGSIZE, &w);
	ft_putchar(c);
	tputs(tgetstr("sc", NULL), 1, &put_my_char);
	ft_memmove(&g_line[*curs + 1], &g_line[*curs], ft_strlen(&g_line[*curs]));
	g_line[*curs] = c;
	ft_putstr(&g_line[*curs + 1]);
	tputs(tgetstr("rc", NULL), 1, &put_my_char);
	*curs += 1;
	if (c == 'a')
	{
		printf("x = %d et size = %d et y = %d\n", get_x_position(), w.ws_col, get_y_position());
	}
	if (get_x_position() == w.ws_col)
	{
		tputs(tgetstr("do", NULL), 1, &put_my_char);
	}
}

void	try_each_funct(int key, int *curs)
{
	static const t_manage	funct[] = {
		{KEY_CODE_ENTER, &enter_funct},
		{KEY_CODE_RIGHT, &arrow_right_funct},
		{KEY_CODE_LEFT, &arrow_left_funct},
		{0, 0}
	};
	int						i;

	i = 0;
	while (funct[i].key_code)
	{
		if (key == funct[i].key_code)
			funct[i].f(curs);
		i++;
	}
}

void	edit_line(void)
{
	char			buff[8];
	int				key;
	static int		curs = 0;

	while (1)
	{
		key = 0;
		ft_bzero(buff, 8);
		read(0, buff, 8);
		if (ft_isprint(buff[0]) && !buff[1])
			add_char_to_line(buff[0], &curs);
		else
			key = get_key(buff);
		try_each_funct(key, &curs);
	}
}

int	main(void)
{
	init_term();
	if (!(g_line = (char*)ft_memalloc(sizeof(char) * 256)))
		clean_exit();
	edit_line();
	default_term();
	return (0);
}