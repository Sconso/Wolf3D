/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/04/21 21:12:33 by sconso            #+#    #+#             */
/*   Updated: 2014/05/07 21:06:32 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <ft_fc_tests.h>
#include <ft_fc_print.h>
#include <get_next_line.h>
#include <stdlib.h>

static int		get_elem_nb(char *str)
{
	int			nb;

	nb = 0;
	while (*str)
	{
		if (ft_isdigit(*str))
		{
			++nb;
			while (*str && ft_isdigit(*str))
				++str;
		}
		else
			++str;
	}
	return (nb);
}

static int		*fill_line(int *line, char *values)
{
	int			*tmp;
	int			val;

	tmp = line;
	while (*values && *tmp != -999)
	{
		if (ft_isdigit(*values))
		{
			val = 0;
			while (ft_isdigit(*values))
				val = (val * 10) + (*values++ - '0');
			*tmp++ = val;
		}
		else
			++values;
	}
	return (line);
}

int				**create_matrix(int fd)
{
	int			lines;
	char		*actline;
	int			**matrix;

	lines = 0;
	while ((actline = get_next_line(fd)))
		++lines;
	matrix = (int **)malloc((lines + 1) * sizeof(int *));
	ft_assert(matrix != NULL, "Malloc error on matrix, exiting...\n");
	matrix[lines] = NULL;
	return (matrix);
}

int				**fill_matrix(int fd, int **matrix)
{
	int			elems;
	int			i;
	char		*line;

	i = 0;
	while ((line = get_next_line(fd)))
	{
		elems = get_elem_nb(line);
		matrix[i] = (int *)malloc((elems + 1) * sizeof(int));
		ft_assert(matrix[i] != NULL, "Malloc error on matrix, exiting...\n");
		matrix[i][elems] = -999;
		matrix[i] = fill_line(matrix[i], line);
		++i;
	}
	return (matrix);
}

void			print_matrix(int **matrix)
{
	int			x;
	int			y;

	ft_putstr("===================\n");
	x = -1;
	while (matrix[++x])
	{
		y = -1;
		while (matrix[x][++y] != -999)
		{
			ft_putstr("| ");
			ft_putnbr(matrix[x][y]);
			ft_putstr(" ");
		}
		ft_putstr("|\n");
	}
	ft_putstr("===================\n");
}
