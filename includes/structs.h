/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 20:04:31 by sconso            #+#    #+#             */
/*   Updated: 2014/05/06 20:06:35 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct		s_keys
{
	char			right;
	char			left;
	char			up;
	char			down;
}					t_keys;

typedef struct		s_mdata
{
	void			*mptr;
	void			*wptr;
	void			*iptr;
	char			*idata;
	int				bpp;
	int				sizeline;
	int				endian;
	int				w;
	int				h;
	int				**map;
}					t_mdata;

#endif
