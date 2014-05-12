/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 20:04:31 by sconso            #+#    #+#             */
/*   Updated: 2014/05/12 22:22:12 by sconso           ###   ########.fr       */
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

typedef struct		s_player
{
	int				x;
	int				y;
	int				z;
	int				fov;
	int				vangle;
	int				height;
	int				speed;
}					t_player;

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
	t_player		*p;
	int				**map;
	int				mapw;
	int				maph;
	t_keys			*keys;
	int				block_size;
}					t_mdata;

typedef struct		s_vertex
{
	float			x;
	float			y;
	float			z;
	float			w;
	unsigned int	color;
}					t_vertex;

typedef struct		s_delta
{
	float			x1;
	float			x2;
	float			y1;
	float			y2;
}					t_delta;

#endif
