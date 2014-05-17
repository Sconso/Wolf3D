/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 20:04:31 by sconso            #+#    #+#             */
/*   Updated: 2014/05/17 20:58:48 by sconso           ###   ########.fr       */
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
	char			w;
	char			a;
	char			s;
	char			d;
	char			o;
	char			p;
	char			plus;
	char			minus;
	char			open_b;
	char			close_b;
	char			option;
	char			delete;
}					t_keys;

typedef struct		s_player
{
	float			x;
	float			y;
	int				z;
	int				fov;
	int				vangle;
	int				height;
	float			speed;
}					t_player;

typedef struct		s_img
{
	void			*ptr;
	char			*data;
	int				bpp;
	int				sizeline;
	int				endian;
}					t_img;

typedef struct		s_mdata
{
	void			*mptr;
	void			*wptr;
	t_img			*img;
	t_img			*imap;
	t_img			*ifov;
	int				w;
	int				h;
	t_player		*p;
	int				**map;
	int				mapw;
	int				maph;
	t_keys			*keys;
	int				block_size;
	char			grid;
	char			rotate;
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
