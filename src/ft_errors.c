/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/21 20:41:47 by sconso            #+#    #+#             */
/*   Updated: 2014/04/21 20:54:03 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_fc_print.h>
#include <stdlib.h>
#include <string.h>

void	ft_exit(char *str)
{
	ft_putstr_fd(str, 2);
	exit(0);
}

void	ft_assert(int check, char *str)
{
	if (!check)
		ft_exit(str);
}

void	ft_strerror(int errnum)
{
	strerror(errnum);
	exit(0);
}

void	ft_error(char *line, int lnb, char *str)
{
	ft_putstr_fd("\033[31m", 2);
	ft_putstr_fd(line, 2);
	ft_putstr_fd("\033[0m\nError on line ", 2);
	ft_putnbr_fd(lnb, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("\033[0m", 2);
	exit(0);
}
