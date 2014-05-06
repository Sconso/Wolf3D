/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/02 19:12:10 by sconso            #+#    #+#             */
/*   Updated: 2014/04/27 19:51:37 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>
#include <ft_fc_str.h>
#include <stdlib.h>
#include <unistd.h>

static char			*ft_exit(char **str)
{
	free(*str);
	*str = 0;
	return (0);
}

char				*get_next_line(int const fd)
{
	static char		*tmp = 0;
	char			*buf;
	char			*size;
	int				ret;

	if (fd < 0 || BUFF_SIZE < 1 || !(buf = ft_strnew(BUFF_SIZE + 1)))
		return (ft_exit(&tmp));
	tmp = (tmp ? tmp : ft_strdup(""));
	while (!(size = ft_strchr(tmp, '\n'))
			&& (ret = read(fd, buf, BUFF_SIZE)))
	{
		buf[ret] = 0;
		tmp = ft_strcleanjoin(tmp, buf);
	}
	free(buf);
	if (tmp[0] && (size || (size = ft_strchr(tmp, 0))))
	{
		buf = ft_strsub(tmp, 0, size - tmp);
		size = (size[0] ? ft_strdup(size + 1) : ft_strdup(""));
		free(tmp);
		tmp = size;
		return (buf);
	}
	return (ft_exit(&tmp));
}
