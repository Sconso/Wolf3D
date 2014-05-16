/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wolf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 19:57:30 by sconso            #+#    #+#             */
/*   Updated: 2014/05/15 20:57:04 by Myrkskog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <mlx.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ft_fc_conversion.h>
#include <math.h>
#include "/usr/X11/include/X11/X.h"

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

	i = -1;
	while (mdata->map[++i])
	{
		j = -1;
		while (mdata->map[i][++j] != -999)
		{
			if (mdata->map[i][j] == 0)
			{
				mdata->p->x = j * mdata->block_size;
				mdata->p->y = i * mdata->block_size;
				return ;
			}
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
				  mdata);
		i += side;
	}
	i = 0;
	while (i < mdata->h)
	{
		draw_line(to_vertex(0, i, 0, 0x444444),
				  to_vertex(mdata->w, i, 0, 0x444444),
				  mdata);
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
			  mdata);

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
			  mdata);
}

void			find_walls(t_mdata *mdata, double start)
{
	start += (start < 0 ? 360 : 0);
	start -= (start > 360 ? 360 : 0);
	find_x(mdata, start, mdata->block_size);
	find_y(mdata, start, mdata->block_size);
}

#include <stdio.h>

void			draw_vector(t_mdata *mdata, float angle, int color)
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
			  mdata);
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
	draw_vector(mdata, mdata->p->vangle - mdata->p->fov / 2, 0xFF0000);
	draw_vector(mdata, mdata->p->vangle + mdata->p->fov / 2, 0xFF0000);
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
	draw_vector(mdata, mdata->p->vangle, 0xFF0000);
}

void			reset_values(t_mdata *mdata)
{
	mdata->block_size = BLOCK_SIZE;
	mdata->grid = 0;
	mdata->rotate = 0;
	return ;
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

int			key_press(int keycode, t_mdata *mdata)
{
	printf("Code = %d\n", keycode);
	if (keycode == ESC)
		exit(0);
	if (keycode == ZERO)
	{
		mdata->grid = (mdata->grid ? 0 : 1);
		return (1);
	}
	if (keycode == OPTION)
	{
		mdata->rotate = (mdata->rotate ? 0 : 1);
		return (1);
	}
	if (keycode == DELETE)
	{
		find_free_spot(mdata);
		return (1);
	}
	if (keycode == RIGHT)
		mdata->keys->right = 1;
	if (keycode == LEFT)
		mdata->keys->left = 1;
	if (keycode == UP || keycode == W)
		mdata->keys->up = 1;
	if (keycode == DOWN || keycode == S)
		mdata->keys->down = 1;
	if (keycode == A)
		mdata->keys->a = 1;
	if (keycode == D)
		mdata->keys->d = 1;
	if (keycode == O)
		mdata->keys->o = 1;
	if (keycode == P)
		mdata->keys->p = 1;
	if (keycode == PLUS)
		mdata->keys->plus = 1;
	if (keycode == MINUS)
		mdata->keys->minus = 1;
	if (keycode == OPEN_BRACKET)
		mdata->keys->open_b = 1;
	if (keycode == CLOSE_BRACKET)
		mdata->keys->close_b = 1;
	return (0);
}

int			key_release(int keycode, t_mdata *mdata)
{
	if (keycode == RIGHT)
		mdata->keys->right = 0;
	if (keycode == LEFT)
		mdata->keys->left = 0;
	if (keycode == UP || keycode == W)
		mdata->keys->up = 0;
	if (keycode == DOWN || keycode == S)
		mdata->keys->down = 0;
	if (keycode == A)
		mdata->keys->a = 0;
	if (keycode == D)
		mdata->keys->d = 0;
	if (keycode == O)
		mdata->keys->o = 0;
	if (keycode == P)
		mdata->keys->p = 0;
	if (keycode == PLUS)
		mdata->keys->plus = 0;
	if (keycode == MINUS)
		mdata->keys->minus = 0;
	if (keycode == OPEN_BRACKET)
		mdata->keys->open_b = 0;
	if (keycode == CLOSE_BRACKET)
		mdata->keys->close_b = 0;
	return (0);
}
#include <stdio.h>
int			loop(t_mdata *mdata)
{
	float	speed;
	int		tmp;
	int		x;
	int		y;

	x = mdata->p->x / mdata->block_size;
	y = mdata->p->y / mdata->block_size;
	if (x > mdata->mapw || x < 0 || y > mdata->maph || y < 0)
		find_free_spot(mdata);
	else if (mdata->map[y][x] != 0)
		find_free_spot(mdata);
	x = mdata->p->x;
	y = mdata->p->y;

	speed = mdata->p->speed;
	if (mdata->keys->right)
		mdata->p->vangle = (mdata->p->vangle == 0 ? 359 : mdata->p->vangle - 1);
	if (mdata->keys->left)
		mdata->p->vangle = (mdata->p->vangle == 359 ? 0 : mdata->p->vangle + 1);
	if (mdata->keys->up)
	{
		tmp = (y - speed > 0 ? y - speed : 1);
		if (tmp / mdata->block_size < 0 || tmp / mdata->block_size > mdata->mapw)
			return (1);
		if (mdata->map[tmp / mdata->block_size][x / mdata->block_size] == 0)
			mdata->p->y = tmp;
	}
	if (mdata->keys->down)
	{
		tmp = (y + speed < mdata->h ? y + speed : mdata->h - 1);
		if (tmp / mdata->block_size < 0 || tmp / mdata->block_size > mdata->mapw)
			return (1);
		if (mdata->map[tmp / mdata->block_size][x / mdata->block_size] == 0)
			mdata->p->y = tmp;
	}
	if (mdata->keys->a)
	{
		tmp = (x - speed > 0 ? x - speed : 1);
		if (tmp / mdata->block_size < 0 || tmp / mdata->block_size > mdata->maph)
			return (1);
		if (mdata->map[y / mdata->block_size][tmp / mdata->block_size] == 0)
			mdata->p->x = tmp;
	}
	if (mdata->keys->d)
	{
		tmp = (x + speed < mdata->w ? x + speed : mdata->w - 1);
		if (tmp / mdata->block_size < 0 || tmp / mdata->block_size > mdata->maph)
			return (1);
		if (mdata->map[y / mdata->block_size][tmp / mdata->block_size] == 0)
			mdata->p->x = tmp;
	}
	if (mdata->keys->o)
		mdata->p->fov -= (mdata->p->fov == 0 ? 0 : 1);
	if (mdata->keys->p)
		mdata->p->fov += (mdata->p->fov == 360 ? 0 : 1);
	if (mdata->keys->plus)
		mdata->p->speed = (speed + .1 >= 10 ? 10 : speed + .1);
	if (mdata->keys->minus)
		mdata->p->speed = (speed - .1 <= 0 ? 0 : speed - .1);
	if (mdata->keys->open_b)
		mdata->block_size -= (mdata->block_size == 2 ? 0 : 1);
	if (mdata->keys->close_b)
		mdata->block_size += (mdata->block_size == 64 ? 0 : 1);
	if (mdata->rotate)
		mdata->p->vangle += (mdata->p->vangle == 360 ? -360 : 1);

	if (mdata->iptr)
		mlx_destroy_image(mdata->mptr, mdata->iptr);
	mdata->iptr = mlx_new_image(mdata->mptr, mdata->w, mdata->h);
	mdata->idata = mlx_get_data_addr(mdata->iptr, &mdata->bpp,
									 &mdata->sizeline, &mdata->endian);
	raycast(mdata);
	mlx_put_image_to_window(mdata->mptr, mdata->wptr, mdata->iptr, 0, 0);
	mlx_string_put(mdata->mptr, mdata->wptr, 0, 50, 0xFF0000, ft_itoa(mdata->p->vangle));
	return (0);
}

int				expose(t_mdata *mdata)
{
	if (mdata->iptr)
		mlx_destroy_image(mdata->mptr, mdata->iptr);
	mdata->iptr = mlx_new_image(mdata->mptr, mdata->w, mdata->h);
	mdata->idata = mlx_get_data_addr(mdata->iptr, &mdata->bpp,
									 &mdata->sizeline, &mdata->endian);
	raycast(mdata);
	mlx_put_image_to_window(mdata->mptr, mdata->wptr, mdata->iptr, 0, 0);
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
	mdata->iptr = NULL;
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
