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
	void	(*f)(void);
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

void	enter_funct(void)
{
	printf("ENTER\n");
}

void	arrow_right_funct(void)
{
	tputs(tgetstr("nd", NULL), 1, &put_my_char);
}

void	arrow_left_funct(void)
{
	tputs(tgetstr("le", NULL), 1, &put_my_char);
}

void	arrow_top_funct(void)
{
	tputs(tgetstr("up", NULL), 1, &put_my_char);
}

void	arrow_down_funct(void)
{
	tputs(tgetstr("sf", NULL), 1, &put_my_char);
}

void	add_char_to_line(int *x, char c)
{
	int 			i;
	static int		line_div = 1;
	struct winsize	w;

	i = 0;
	ioctl(0, TIOCGSIZE, &w);
	if (c == 'a')
	{
		printf("x = %d et size = %d\n", *x, w.ws_col);
		return ;
	}
	if (*x - 1 == ft_strlen(g_line) && *x < 256)
	{
		ft_putchar(c);
		g_line[ft_strlen(g_line)] = c;
		*x += 1;
	}
	else if (*x > 0 && *x < ft_strlen(g_line))
	{
		ft_putchar(c);
		tputs(tgetstr("sc", NULL), 1, &put_my_char);
		ft_memmove(&g_line[*x + 1], &g_line[*x], ft_strlen(&g_line[*x]));
		g_line[*x] = c;
		ft_putstr(&g_line[*x + 1]);
		tputs(tgetstr("rc", NULL), 1, &put_my_char);
		*x += 1;
	}
}

int	main(void)
{
	char			buff[8];
	int				key;
	int				i;
	static int		x = 1;
	struct winsize	w;
	static const t_manage	funct[] = {
		{KEY_CODE_ENTER, &enter_funct},
		{KEY_CODE_RIGHT, &arrow_right_funct},
		{KEY_CODE_TOP, &arrow_top_funct},
		{KEY_CODE_DOWN, &arrow_down_funct},
		{KEY_CODE_LEFT, &arrow_left_funct},
		{0, 0}
	};
	static int	div_coef = 1;

	i = 0;
	ioctl(0, TIOCGSIZE, &w);
	init_term();
	if (!(g_line = (char*)ft_memalloc(sizeof(char) * 256)))
		clean_exit();
	ft_bzero(buff, 8);
	while (1)
	{
		ft_bzero(buff, 8);
		read(0, buff, 8);
		key = get_key(buff);
		i = 0;
		if (ft_isprint(buff[0]) && !buff[1])
			add_char_to_line(&x, buff[0]);
		while (funct[i].key_code)
		{
			if (key == funct[i].key_code)
			{
				if (key == KEY_CODE_RIGHT && x / div_coef == w.ws_col)
				{
					tputs(tgetstr("do", NULL), 1, &put_my_char);
					div_coef++;
					x++;
					break ;
				}
				else if (key == KEY_CODE_LEFT && x > 0)
				{
					if (x > 0)
					{
						if ((div_coef > 2 && (x / (div_coef - 1)) == w.ws_col)
							|| (div_coef == 2 && (x / (div_coef - 1) - 1) == w.ws_col))
							div_coef--;
						x--;
					}
					else
						break ;
				}
				else if (key == KEY_CODE_RIGHT)
				{
					if (x < ft_strlen(g_line))
						x++;
					else
						break ;
				}
				else if (key == KEY_CODE_ENTER)
				{
					printf("\nline %s\n", g_line);
					free(g_line);
					default_term();
					exit(0);
				}
				funct[i].f();
			}
			i++;
		}
	}
	default_term();
	return (0);
}