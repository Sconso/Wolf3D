/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wolf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 19:57:30 by sconso            #+#    #+#             */
/*   Updated: 2014/05/14 23:37:00 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <mlx.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ft_fc_conversion.h>
#include <math.h>
#include "/usr/X11/include/X11/X.h"

float			to_rad(float deg)
{
	return (M_PI * deg / 180);
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

void			find_walls(t_mdata *mdata, double start)
{
	t_delta		delta;
	int			xgrid;
	int			ygrid;

	delta.x1 = 0;
	delta.x2 = 0;
	delta.y1 = 0;
	delta.y2 = 0;
	if (mdata->p->vangle > 0 && mdata->p->vangle < 180)
	{
		delta.y1 = floor(mdata->p->y / 64) * 64 + 64;
		delta.x1 = mdata->p->x + (mdata->p->y - delta.y1) * tan(start);
		delta.y2 = 64;
	}
	else
	{
		delta.y1 = floor(mdata->p->y / 64) * 64;
		delta.x1 = mdata->p->x + (mdata->p->y - delta.y1) * tan(start);
		delta.y1--;
		delta.y2 = -64;
	}
	if (start != 0 && start != 180)
	{
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
				delta.x1 += mdata->block_size / tan(start);
				delta.y1 += delta.y2;
			}
		}
	}
}
#include <stdio.h>
void			ray(t_mdata *mdata)
{
	t_delta		pos;
	float		dist;
	float		h;

	pos.x1 = mdata->p->x;
	pos.y1 = mdata->p->y;
	if (mdata->p->vangle < 90)
		dist = mdata->w - pos.x1;
	else if (mdata->p->vangle < 180)
		dist = pos.x1;
	else if (mdata->p->vangle < 270)
		dist = pos.x1;
	else
		dist = mdata->w - pos.x1;
	if (mdata->p->vangle == 90 || mdata->p->vangle == 270)
		h = 0;
	else
		h = dist / cos(to_rad(mdata->p->vangle));
	if (mdata->p->vangle > 90 && mdata->p->vangle < 270)
		h = -h;
	pos.x2 = pos.x1 + (cos(to_rad(mdata->p->vangle)) * h);
	pos.y2 = pos.y1 + (-sin(to_rad(mdata->p->vangle)) * h);
	printf("Cos = %f\nSin = %f\n", cos(to_rad(mdata->p->vangle)), sin(to_rad(mdata->p->vangle)));
	printf("dist = %f\nH = %f\n", dist, h);
	printf("%f - %f\n", pos.x2, pos.y2);

	draw_line(to_vertex(pos.x1, pos.y1, 0, 0xFF0000),
			  to_vertex(pos.x2, pos.y2, 0, 0xFF0000),
			  mdata);
}

/*void			ray(t_mdata *mdata)
{
	t_delta		pos;

	pos.x1 = mdata->p->x;
	pos.y1 = mdata->p->y;
	if (mdata->p->vangle < 90 || mdata->p->vangle > 270)
		pos.x2 = (mdata->w - pos.x1) * cos(to_rad(mdata->p->vangle));
	else
		pos.x2 = pos.x1 * cos(to_rad(mdata->p->vangle));
	if (mdata->p->vangle <= 180)
		pos.y2 = pos.y1 * -sin(to_rad(mdata->p->vangle));
	else
		pos.y2 = (mdata->h - pos.y1) * -sin(to_rad(mdata->p->vangle));

	pos.x2 = pos.x1 + pos.x2;
	pos.y2 = pos.y1 + pos.y2;

printf("x = %f\ny = %f\nsin = %f\n", pos.x2, pos.y2, sin(to_rad(mdata->p->vangle)));
	draw_line(to_vertex(pos.x1, pos.y1, 0, 0xFF0000),
			  to_vertex(pos.x2, pos.y2, 0, 0xFF0000),
			  mdata);
			  }*/

void			raycast(t_mdata *mdata)
{
	int			distance;
	double		sub;
	double		start;
	int			i;

	ray(mdata);
	distance = (mdata->w / 2) / (tan((double)mdata->p->fov / 2));
	sub = (double)mdata->p->fov / (double)mdata->w;
	start = mdata->p->vangle - (mdata->p->fov / 2);
	if (start < 0)
		start += 360;
	i = -1;
	while (++i <= mdata->w)
	{
		find_walls(mdata, start);
		start += sub;
		distance++;
	}
}

void			reset_values(t_mdata *mdata)
{
	mdata->block_size = BLOCK_SIZE;
	return ;
}

t_player		*init_player(t_mdata *mdata)
{
	t_player	*p;

	p = (t_player *)malloc(sizeof(t_player));
	ft_assert(p != NULL, "Can't malloc the player... Exiting...\n");
	p->x = 100;
	p->y = 100;
	p->z = 0;
	p->fov = 60;
	p->vangle = 37;
	p->speed = 10;
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
	if (keycode == ESC)
		exit(0);
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
	return (0);
}
#include <stdio.h>
int			loop(t_mdata *mdata)
{
	if (mdata->keys->right)
		mdata->p->vangle = (mdata->p->vangle == 0 ? 359 : mdata->p->vangle - 1);
	if (mdata->keys->left)
		mdata->p->vangle = (mdata->p->vangle == 359 ? 0 : mdata->p->vangle + 1);
	if (mdata->keys->up)
		mdata->p->y--;
	if (mdata->keys->down)
		mdata->p->y++;
	if (mdata->keys->a)
		mdata->p->x--;
	if (mdata->keys->d)
		mdata->p->x++;
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
