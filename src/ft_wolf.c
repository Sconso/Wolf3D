/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wolf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 19:57:30 by sconso            #+#    #+#             */
/*   Updated: 2014/05/17 22:00:22 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <mlx.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ft_fc_conversion.h>
#include <math.h>
#include "/usr/X11/include/X11/X.h"

#include <stdio.h>

float				gsin(float deg)
{
	static float	*tsin = NULL;
	int				i;

	if (!tsin)
	{
		if (!(tsin = (float *)malloc(366 * sizeof(*tsin))))
			exit(0);
		i = -1;
		while (++i <= 365)
			tsin[i] = sin(i);
	}
	return (tsin[(int)deg]);
}

float			to_rad(float deg)
{
	return (M_PI * deg / 180);
}

void			find_free_spot(t_mdata *mdata)
{
	int			i;
	int			j;
	int			bs;

	i = mdata->p->y;
	j = mdata->p->x;
	bs = mdata->block_size;
	while (mdata->map[++i / bs] && mdata->map[i / bs][++j / bs] != -999)
	{
		if (mdata->map[i / bs][j / bs] == 0)
		{
			mdata->p->x = j;
			mdata->p->y = i;
			return ;
		}
	}
	i = mdata->p->y;
	j = mdata->p->x;
	while (--i / bs >= 0 && --j / bs >= 0)
	{
		if (mdata->map[i / bs][j / bs] == 0)
		{
			mdata->p->x = j;
			mdata->p->y = i;
			return ;
		}
	}

	exit(0);
}

void			draw_grid(t_mdata *mdata, int side)
{
	int			i;

	i = 0;
	while (i < mdata->w)
	{
		draw_line(to_vertex(i, 0, 0, 0x444444),
				  to_vertex(i, mdata->h, 0, 0x444444),
				  mdata, mdata->ifov);
		i += side;
	}
	i = 0;
	while (i < mdata->h)
	{
		draw_line(to_vertex(0, i, 0, 0x444444),
				  to_vertex(mdata->w, i, 0, 0x444444),
				  mdata, mdata->ifov);
		i += side;
	}
}

void			get_map_size(t_mdata *mdata)
{
	int			x;
	int			y;
	int			**map;

	map = mdata->map;
	x = -1;
	while (map[++x])
	{
		y = -1;
		while (map[x][++y] != -999)
			;
	}
	mdata->mapw = y - 1;
	mdata->maph = x - 1;
}

void			find_x(t_mdata *mdata, double start, int bs)
{
	int			xgrid;
	int			ygrid;
	t_delta		delta;

	if (start > 180)
		delta.y1 = floor(mdata->p->y / bs) * bs + bs;
	else
		delta.y1 = floor(mdata->p->y / bs) * bs - 1;
	delta.x1 = mdata->p->x + ((mdata->p->y - delta.y1) / tan(to_rad(start)));
	delta.y2 = (start > 180 ? bs : -bs);
	while (1)
	{
		xgrid = (int)(delta.x1 / mdata->block_size);
		ygrid = (int)(delta.y1 / mdata->block_size);
		if (xgrid > mdata->mapw || ygrid > mdata->maph || xgrid < 0 || ygrid < 0)
			break ;
		else if (mdata->map[ygrid][xgrid] != 0)
			break ;
		else
		{
			if (start > 180)
				delta.x1 += mdata->block_size / -tan(to_rad(start));
			else
				delta.x1 += mdata->block_size / tan(to_rad(start));
			delta.y1 += delta.y2;
		}
	}
/*	fill_image(mdata, delta.x1, delta.y1, 0x00FF00);*/
	delta.y2 = (delta.y1 < mdata->p->y ? delta.y1 - bs : delta.y1 + bs);
	draw_line(to_vertex(delta.x1, delta.y1, 0, 0x00FF00),
			  to_vertex(delta.x1, delta.y2, 0, 0x00FF00),
			  mdata, mdata->imap);

}

void			find_y(t_mdata *mdata, double start, int bs)
{
	int			xgrid;
	int			ygrid;
	t_delta		delta;

	if (start < 90 || start > 270)
		delta.x1 = floor(mdata->p->x / bs) * bs + bs;
	else
		delta.x1 = floor(mdata->p->x / bs) * bs - 1;
	delta.y1 = mdata->p->y + ((mdata->p->x - delta.x1) * tan(to_rad(start)));
	delta.x2 = (start < 90 || start > 270 ? bs : -bs);
	while (1)
	{
		xgrid = (int)(delta.x1 / mdata->block_size);
		ygrid = (int)(delta.y1 / mdata->block_size);
		if (xgrid > mdata->mapw || ygrid > mdata->maph || xgrid < 0 || ygrid < 0)
			break ;
		else if (mdata->map[ygrid][xgrid] != 0)
			break ;
		else
		{
			if (start < 90 || start > 270)
				delta.y1 += mdata->block_size * -tan(to_rad(start));
			else
				delta.y1 += mdata->block_size * tan(to_rad(start));
			delta.x1 += delta.x2;
		}
	}
/*	fill_image(mdata, delta.x1, delta.y1, 0x00FFFF);*/
	delta.x2 = (delta.x1 < mdata->p->x ? delta.x1 - bs : delta.x1 + bs);
	draw_line(to_vertex(delta.x1, delta.y1, 0, 0x00FF00),
			  to_vertex(delta.x2, delta.y1, 0, 0x00FF00),
			  mdata, mdata->imap);
}

void			find_walls(t_mdata *mdata, double start)
{
	start += (start < 0 ? 360 : 0);
	start -= (start > 360 ? 360 : 0);
	find_x(mdata, start, mdata->block_size);
	find_y(mdata, start, mdata->block_size);
}

#include <stdio.h>

void			draw_vector(t_mdata *mdata, t_img *img, float angle, int color)
{
	float		x;
	float		y;

	if (angle < 0)
		angle += 360;
	else if (angle > 360)
		angle -= 360;
	x = mdata->p->x;
	y = mdata->p->y;
	while (42)
	{
		if (x >= mdata->w || y >= mdata->h || x < 0 || y < 0)
			break ;
		x += (cos(to_rad(angle)) * mdata->block_size);
		y += (-sin(to_rad(angle)) * mdata->block_size);
	}
/* 	printf("x = %f, y = %f\n nx = %f, ny = %f\n", mdata->p->x, mdata->p->y, x, y); */
	draw_line(to_vertex(mdata->p->x, mdata->p->y, 0, color),
			  to_vertex(x, y, 0, color),
			  mdata, img);
}

void			raycast(t_mdata *mdata)
{
	int			distance;
	double		sub;
	double		start;
	int			i;

	if (mdata->grid)
		draw_grid(mdata, mdata->block_size);
	start = (mdata->p->vangle - (mdata->p->fov / 2));
/*	while ((start += 1) < mdata->p->vangle + (mdata->p->fov / 2))
	draw_vector(mdata, start, 0xFFFF00);*/
	draw_vector(mdata, mdata->ifov, mdata->p->vangle - mdata->p->fov / 2, 0xFF0000);
	draw_vector(mdata, mdata->ifov, mdata->p->vangle + mdata->p->fov / 2, 0xFF0000);
/*	draw_vector(mdata, mdata->p->vangle, 0xFF0000);*/
	distance = (mdata->w / 2) / (tan((double)mdata->p->fov / 2));
	sub = (double)mdata->p->fov / (double)mdata->w;
	start = mdata->p->vangle - (mdata->p->fov / 2);
	if (start < 0)
		start += 360;
	if (start > 360)
		start -= 360;
	i = -1;
	while (++i <= mdata->w)
	{
/*		draw_vector(mdata, start, 0xFFFFFF);*/
		find_walls(mdata, start);
		start += sub;
		distance++;
	}
	draw_vector(mdata, mdata->ifov, mdata->p->vangle, 0xFF0000);
}

void			reset_values(t_mdata *mdata)
{
	mdata->block_size = BLOCK_SIZE;
	mdata->grid = 0;
	mdata->rotate = 0;
	return ;
}

t_img			*init_img(t_mdata *mdata, int width, int height)
{
	t_img		*img;

	img = (t_img *)malloc(sizeof(*img));
	ft_assert(img != NULL, "Can't malloc the img... Exiting...\n");
	img->ptr = mlx_new_image(mdata->mptr, width, height);
	img->data = mlx_get_data_addr(img->ptr, &img->bpp,
								   &img->sizeline, &img->endian);
	return (img);
}

t_img			*reinit_img(t_mdata *mdata, t_img *img, int width, int height)
{
	if (img->ptr)
		mlx_destroy_image(mdata->mptr, img->ptr);
	img->ptr = mlx_new_image(mdata->mptr, width, height);
	img->data = mlx_get_data_addr(img->ptr, &img->bpp,
								   &img->sizeline, &img->endian);
	return (img);
}

t_player		*init_player(t_mdata *mdata)
{
	t_player	*p;

	p = (t_player *)malloc(sizeof(t_player));
	ft_assert(p != NULL, "Can't malloc the player... Exiting...\n");
	p->z = 0;
	p->fov = 60;
	p->vangle = 315;
	p->speed = 3;
	p->height = mdata->block_size / 2;
	return (p);
}

static t_keys	*init_keys(void)
{
	t_keys		*keys;

	keys = (t_keys *)malloc(sizeof(t_keys));
	ft_assert(keys != NULL, "Can't malloc the keys... Exiting...\n");
	keys->right = 0;
	keys->left = 0;
	keys->up = 0;
	keys->down = 0;
	keys->w = 0;
	keys->a = 0;
	keys->s = 0;
	keys->d = 0;
	keys->o = 0;
	keys->p = 0;
	keys->minus = 0;
	keys->plus = 0;
	keys->open_b = 0;
	keys->close_b = 0;
	return (keys);
}

int			**ft_init(char *map)
{
	int		fd;
	int		**matrix;

	if (1)
		ft_ext(map, ".w3d");
	fd = ft_open(map, O_RDWR);
	matrix = create_matrix(fd);
	ft_close(fd);
	fd = ft_open(map, O_RDONLY);
	matrix = fill_matrix(fd, matrix);
	return (matrix);
}

#include <stdio.h>

t_img		*mlx_merge_img(t_img *img1, t_img *img2, t_vertex infos, t_mdata *mdata)
{
	t_img	*img;
	int		i;
	int		j;
	int		r;

	img = init_img(mdata, infos.x, infos.y);
	i = -1;
	while (++i < infos.y)
	{
		j = -1;
		while (++j < infos.x)
		{
			r = (j * 4) + (i * img1->sizeline);
			img->data[r] = img1->data[r] + img2->data[r];
			img->data[r + 1] = img1->data[r + 1] + img2->data[r + 1];
			img->data[r + 2] = img1->data[r + 2] + img2->data[r + 2];
		}
	}
	return (img);
}

void			move_loop(t_mdata *mdata, int x, int y)
{
	float		tmp;
	float		speed;
	float		tmpspeed;

	speed = mdata->p->speed;
	if (mdata->keys->up)
	{
		tmpspeed = speed;
		tmp = (y - tmpspeed) / mdata->block_size;
		while (tmp >= 0 && tmp <= mdata->mapw)
		{
			if (mdata->map[(int)tmp][x / mdata->block_size])
				tmp = (y - --tmpspeed) / mdata->block_size;
			else
			{
				mdata->p->y = tmp * mdata->block_size;
				break ;
			}
		}
	}
	if (mdata->keys->down)
	{
		tmpspeed = speed;
		tmp = (y + tmpspeed) / mdata->block_size;
		while (tmp >= 0 && tmp <= mdata->mapw)
		{
			if (mdata->map[(int)tmp][x / mdata->block_size])
				tmp = (y + --tmpspeed) / mdata->block_size;
			else
			{
				mdata->p->y = tmp * mdata->block_size;
				break ;
			}
		}
	}
	if (mdata->keys->a)
	{
		tmpspeed = speed;
		tmp = (x - tmpspeed) / mdata->block_size;
		while (tmp >= 0 && tmp <= mdata->maph)
		{
			if (mdata->map[y / mdata->block_size][(int)tmp])
				tmp = (x - --tmpspeed) / mdata->block_size;
			else
			{
				mdata->p->x = tmp * mdata->block_size;
				break ;
			}
		}
	}
	if (mdata->keys->d)
	{
		tmpspeed = speed;
		tmp = (x + tmpspeed) / mdata->block_size;
		while (tmp >= 0 && tmp <= mdata->maph)
		{
			if (mdata->map[y / mdata->block_size][(int)tmp])
				tmp = (x + --tmpspeed) / mdata->block_size;
			else
			{
				mdata->p->x = tmp * mdata->block_size;
				break ;
			}
		}
	}
}

int			loop(t_mdata *mdata)
{
	float	speed;
	int		x;
	int		y;

	x = (int)mdata->p->x / mdata->block_size;
	y = (int)mdata->p->y / mdata->block_size;
	if (x > mdata->mapw || x < 0 || y > mdata->maph || y < 0)
		find_free_spot(mdata);
	else if (mdata->map[y][x] != 0)
		find_free_spot(mdata);
	x = mdata->p->x;
	y = mdata->p->y;
	speed = mdata->p->speed;
	move_loop(mdata, x, y);
	if (mdata->keys->right)
		mdata->p->vangle = (mdata->p->vangle == 0 ? 359 : mdata->p->vangle - 1);
	if (mdata->keys->left)
		mdata->p->vangle = (mdata->p->vangle == 359 ? 0 : mdata->p->vangle + 1);
	if (mdata->keys->o)
		mdata->p->fov -= (mdata->p->fov == 0 ? 0 : 1);
	if (mdata->keys->p)
		mdata->p->fov += (mdata->p->fov == 360 ? 0 : 1);
	if (mdata->keys->plus)
		mdata->p->speed = (speed + .1 >= 10 ? 10 : speed + .1);
	if (mdata->keys->minus)
		mdata->p->speed = (speed - .1 <= 0 ? 0 : speed - .1);
	if (mdata->keys->open_b)
	{
		mdata->block_size -= (mdata->block_size == 2 ? 0 : 1);
		mdata->imap = reinit_img(mdata, mdata->imap, mdata->w, mdata->h);
	}
	if (mdata->keys->close_b)
	{
		mdata->block_size += (mdata->block_size == 64 ? 0 : 1);
		mdata->imap = reinit_img(mdata, mdata->imap, mdata->w, mdata->h);
	}
	if (mdata->rotate)
		mdata->p->vangle += (mdata->p->vangle == 360 ? -360 : 1);

	mdata->ifov = reinit_img(mdata, mdata->ifov, mdata->w, mdata->h);
	raycast(mdata);
	mdata->img = mlx_merge_img(mdata->imap, mdata->ifov, to_vertex(mdata->w, mdata->h, 0, 0), mdata);
	mlx_put_image_to_window(mdata->mptr, mdata->wptr, mdata->img->ptr, 0, 0);
	mlx_string_put(mdata->mptr, mdata->wptr, 0, 50, 0xFF0000, ft_itoa(mdata->p->vangle));
	return (0);
}

int				expose(t_mdata *mdata)
{
	(void)mdata;
	return (0);
}

t_mdata			*init_mlx(int **map, int userwidth, int userheight)
{
	t_mdata		*mdata;

	mdata = (t_mdata *)malloc(sizeof(t_mdata));
	ft_assert(mdata != NULL, "Can't malloc the mdata... Exiting...\n");
	mdata->w = (userwidth > 0 ? userwidth : 900);
	mdata->h = (userheight > 0 ? userheight : mdata->w * 9 / 16);
	mdata->map = map;
	mdata->mptr = mlx_init();
	mdata->wptr = mlx_new_window(mdata->mptr, mdata->w, mdata->h, "Wolf 3D");
	mdata->imap = init_img(mdata, mdata->w, mdata->h);
	mdata->ifov = init_img(mdata, mdata->w, mdata->h);
	mdata->keys = init_keys();
	reset_values(mdata);
	get_map_size(mdata);
	mdata->p = init_player(mdata);
	find_free_spot(mdata);
	mlx_do_key_autorepeatoff(mdata->mptr);
	mlx_hook(mdata->wptr, KeyPress, KeyPressMask, &key_press, mdata);
	mlx_hook(mdata->wptr, KeyRelease, KeyReleaseMask, &key_release, mdata);
	mlx_loop_hook(mdata->mptr, loop, mdata);
	mlx_expose_hook(mdata->wptr, expose, mdata);
	mlx_loop(mdata->mptr);
	return (mdata);
}

int			main(int ac, char **av)
{
	int		**matrix;

	ft_assert(ac >= 2, "Usage: ./wolf3d map.w3d [width] [height]\n");
	matrix = ft_init(av[1]);
	print_matrix(matrix);
	if (ac == 3)
		init_mlx(matrix, ft_atoi(av[2]), 0);
	else if (ac == 4)
		init_mlx(matrix, ft_atoi(av[2]), ft_atoi(av[3]));
	else
		init_mlx(matrix, 0, 0);
	return (0);
}
