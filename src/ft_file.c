/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/21 21:21:21 by sconso            #+#    #+#             */
/*   Updated: 2014/05/06 20:01:59 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <ft_fc_print.h>
#include <ft_fc_str.h>
#include <fcntl.h>
#include <unistd.h>

int			ft_open(char *file, int mode)
{
	int		fd;

	ft_assert(file != NULL, "File error : Couldn't retrive the filename.\n");
	if ((fd = open(file, mode)) == -1)
	{
		ft_putstr_fd("File error : Couldn't open \033[33m", 2);
		ft_putstr_fd(file, 2);
		ft_exit("\033[0m.\n");
	}
	return (fd);
}

void		ft_close(int fd)
{
	ft_assert(close(fd) != -1, "File error : Couldn't close the file.\n");
}

void		ft_ext(char *file, char *ext)
{
	int		file_len;
	int		ext_len;

	file_len = ft_strlen(file);
	ext_len = ft_strlen(ext);
	while (--ext_len >= 0)
	{
		if (file[--file_len] != ext[ext_len])
		{
			ft_putstr_fd(file, 2);
			ft_putstr_fd(" : File must have ", 2);
			ft_putstr_fd(ext, 2);
			ft_exit(" extension.\n");
		}
	}
}
