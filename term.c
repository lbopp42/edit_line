/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/25 12:38:13 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/25 12:38:15 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <termios.h>
#include <term.h>

struct termios	g_init_term;
void	default_term(void);

void	init_term(void)
{
	char			*term;
	struct termios	attr;

	tcgetattr(STDIN_FILENO, &g_init_term);
	tcgetattr(STDIN_FILENO, &attr);
	attr.c_lflag &= ~(ECHO | ICANON);
	attr.c_cc[VMIN] = 1;
	attr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSADRAIN, &attr);
	if (!(term = getenv("TERM")))
	{
		ft_putstr_fd("minishell: environment not found\n", 2);
		default_term();
		exit(0);
	}
	else
		tgetent(NULL, term);
}

void	default_term(void)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_init_term);
	tgetent(NULL, getenv("TERM"));
}
