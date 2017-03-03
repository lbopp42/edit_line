/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isinarray.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbopp <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 11:38:02 by lbopp             #+#    #+#             */
/*   Updated: 2017/02/10 11:38:07 by lbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>

int	ft_isinarray(char *var, char **array)
{
	int i;

	i = 0;
	if (!var)
		return (-1);
	while (array && array[i])
	{
		if (array && array[i] && !ft_strcmp(var, array[i]))
			return (1);
		i++;
	}
	return (0);
}
