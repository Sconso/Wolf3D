/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wolf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 19:57:30 by sconso            #+#    #+#             */
/*   Updated: 2014/05/18 22:40:37 by Myrkskog         ###   ########.fr       */
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

int				get_wall_color(t_mdata *mdata, int x, int y)
{
	int			bs;

	bs = mdata->block_size;
	if (mdata->map[y / bs][x / bs])
	{
		if (x == ((x / bs) * bs))
			return (0xFF0000);
		else if (x == (((x / bs) + 1) * bs - 1))
			return (0x00FF00);
		else if (y == ((y / bs) * bs))
			return (0x0000FF);
		else if (y == (((y / bs) + 1) * bs - 1))
			return (0xFFFFFF);
		else
			return (0x000000);
	}
	return (0x000000);
}


double			find_x(t_mdata *mdata, double start, int bs, int *color)
{
	int			xgrid;
	int			ygrid;
	t_delta		delta;
	double		dist;

	if ((int)start == 0 || (int)start == 180)
		return (10000000000000);
	if (start > 180)
		delta.y1 = floor((int)mdata->p->y / bs) * bs + bs;
	else
		delta.y1 = floor((int)mdata->p->y / bs) * bs - 1;
	delta.x1 = (mdata->p->x + ((mdata->p->y - delta.y1) / tan(to_rad(start))));
	delta.y2 = (start > 180 ? bs : -bs);
	while (1)
	{
		xgrid = (int)(delta.x1 / mdata->block_size);
		ygrid = (int)(delta.y1 / mdata->block_size);
		if (xgrid > mdata->mapw || ygrid > mdata->maph || xgrid < 0 || ygrid < 0)
			return (10000000000);
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
	*color = get_wall_color(mdata, delta.x1, delta.y1);
	delta.y2 = (delta.y1 < mdata->p->y ? delta.y1 - bs : delta.y1);
	if (delta.x1 > 0 && delta.y1 > 0 && delta.x1 < mdata->w && delta.y1 < mdata->h)
		fill_image(mdata->imap, delta.x1, delta.y1, mlx_get_color_value(mdata->mptr, 0xFF0000));
/*	draw_line(to_vertex(delta.x1, delta.y1, 0, 0x00FF00),
			  to_vertex(delta.x1, delta.y2, 0, 0x00FF00),
			  mdata, mdata->imap);*/
	if (start > 180)
		dist = fabs((mdata->p->y - delta.y1)) / -sin(to_rad(start));
	else
		dist = fabs((mdata->p->y - delta.y1)) / sin(to_rad(start));
	return (dist);
}

double			find_y(t_mdata *mdata, double start, int bs, int *color)
{
	int			xgrid;
	int			ygrid;
	t_delta		delta;
	double		dist;

	if ((int)start == 90 || (int)start == 270)
		return (10000000000000);
	if (start < 90 || start > 270)
		delta.x1 = floor((int)mdata->p->x / bs) * bs + bs;
	else
		delta.x1 = floor((int)mdata->p->x / bs) * bs - 1;
	delta.y1 = mdata->p->y + ((mdata->p->x - delta.x1) * tan(to_rad(start)));
	delta.x2 = (start < 90 || start > 270 ? bs : -bs);
	while (1)
	{
		xgrid = (int)(delta.x1 / mdata->block_size);
		ygrid = (int)(delta.y1 / mdata->block_size);
		if (xgrid > mdata->mapw || ygrid > mdata->maph || xgrid < 0 || ygrid < 0)
			return (10000000000000);
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
	*color = get_wall_color(mdata, delta.x1, delta.y1);
	delta.x2 = (delta.x1 < mdata->p->x ? delta.x1 - bs: delta.x1 + bs);
	if (delta.x1 > 0 && delta.y1 > 0 && delta.x1 < mdata->w && delta.y1 < mdata->h)
		fill_image(mdata->imap, delta.x1, delta.y1, mlx_get_color_value(mdata->mptr, 0xFF0000));
	if (delta.x2 > 0 && delta.y1 > 0 && delta.x2 < mdata->w && delta.y1 < mdata->h)
		fill_image(mdata->imap, delta.x2, delta.y1, mlx_get_color_value(mdata->mptr, 0xFF0000));
/*	draw_line(to_vertex(delta.x1, delta.y1, 0, 0x00FF00),
			  to_vertex(delta.x2, delta.y1, 0, 0x00FF00),
			  mdata, mdata->imap);*/
	if (start >= 180)
		dist = fabs((mdata->p->y - delta.y1)) / -sin(to_rad(start));
	else
		dist = fabs((mdata->p->y - delta.y1)) / sin(to_rad(start));
	return (dist);
}

void				draw_wall(t_mdata *mdata, int height, int slice, int color)
{
	int				center;

	printf("%d\n", height);
	center = mdata->h / 2;
	draw_line(to_vertex(slice, 0, 0, 0x666666),
			  to_vertex(slice, center - height, 0, color),
			  mdata, mdata->game);
	draw_line(to_vertex(slice, center - height, 0, color),
			  to_vertex(slice, center + height, 0, color),
			  mdata, mdata->game);
	draw_line(to_vertex(slice, center + height, 0, 0x444444),
			  to_vertex(slice, mdata->h, 0, color),
			  mdata, mdata->game);
}

void			find_walls(t_mdata *mdata, double start, int slice, int distance)
{
	double		dist_x;
	double		dist_y;
	double		dist;
	double		c_angle;
	int			color;

	color = 0x000000;
	c_angle = mdata->p->vangle - start;
	start += (start < 0 ? 360 : 0);
	start -= (start > 360 ? 360 : 0);
	dist_x = find_x(mdata, start, mdata->block_size, &color) * cos(to_rad(c_angle));
	dist_y = find_y(mdata, start, mdata->block_size, &color) * cos(to_rad(c_angle));
	dist = (dist_x < dist_y ? fabs(dist_x) : fabs(dist_y));
	dist = fabs((int)(mdata->block_size / dist * distance));
	dist = (dist > mdata->h ? mdata->h : dist);
	draw_wall(mdata, dist / 2, slice, color);
}

double			find_walls2(t_mdata *mdata, double start)
{
	double		c_angle;
	t_delta		delta;
	double		dist;
	int			bs;

	if ((int)start == 90 || (int)start == 270)
	{
		printf("oijsfpj\n");
		return (1000000);
	}
	bs = mdata->block_size;
	c_angle = cos(to_rad(mdata->p->vangle - start));
	start += (start < 0 ? 360 : 0);
	start -= (start >= 360 ? 360 : 0);
	delta.x1 = mdata->p->x;
	delta.y1 = mdata->p->y;
	while (42)
	{
		if ((int)delta.x1 / bs >= mdata->mapw || (int)delta.y1 / bs >= mdata->maph)
			break ;
		if ((int)delta.x1 / bs < 0 || (int)delta.y1 / bs < 0)
			break ;
		if (mdata->map[(int)delta.y1 / mdata->block_size][(int)delta.x1 / mdata->block_size])
			break ;
		delta.x1 += cos(to_rad(start)) * 2;
		delta.y1 += -sin(to_rad(start)) * 2;
	}
	while (42)
	{
		if ((int)delta.x1 / bs >= mdata->mapw || (int)delta.y1 / bs >= mdata->maph)
			break ;
		if ((int)delta.x1 / bs < 0 || (int)delta.y1 / bs < 0)
			break ;
		if (mdata->map[(int)delta.y1 / mdata->block_size][(int)delta.x1 / mdata->block_size])
			break ;
		delta.x1 += cos(to_rad(start)) * .1;
		delta.y1 += -sin(to_rad(start)) * .1;
	}
	if (delta.x1 > 0 && delta.y1 > 0 && delta.x1 < mdata->w && delta.y1 < mdata->h)
		fill_image(mdata->ifov, delta.x1, delta.y1, mlx_get_color_value(mdata->mptr, 0xFF00DD));
	if ((int)start != 90 || (int)start != 270)
		dist = (fabs(mdata->p->x - delta.x1) / cos(to_rad(start))) * c_angle;
	else
		dist = (fabs(mdata->p->y - delta.y1) / sin(to_rad(start))) * c_angle;
	if ((int)start == 90 || (int)start == 270)
		printf("Cos = %f\nsin = %f\n", cos(to_rad(start)), sin(to_rad(start)));

	return (dist);
}

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
	draw_vector(mdata, mdata->ifov, mdata->p->vangle - mdata->p->fov / 2, 0xFF0000);
	draw_vector(mdata, mdata->ifov, mdata->p->vangle + mdata->p->fov / 2, 0xFF0000);
	distance = (mdata->w / 2) / (tan(to_rad(mdata->p->fov / 2)));
	sub = (double)mdata->p->fov / (double)mdata->w;
	start = mdata->p->vangle - (mdata->p->fov / 2);
	i = -1;
	while (++i <= mdata->w)
	{
		find_walls(mdata, start, i, distance);
		start += sub;
	}
	draw_vector(mdata, mdata->ifov, mdata->p->vangle, 0xFF0000);

}

void			reset_values(t_mdata *mdata)
{
	mdata->block_size = BLOCK_SIZE;
	mdata->grid = 0;
	mdata->rotate = 0;
	mdata->active_map = 0;
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
	double		tmp_x;
	double		tmp_y;
	double		speed;
	double		tmpspeed;

	speed = mdata->p->speed;
	if (mdata->keys->up)
	{
		tmpspeed = speed;
		tmp_x = (x + round(cos(to_rad(mdata->p->vangle)) * tmpspeed)) / mdata->block_size;
		tmp_y = (y - round(sin(to_rad(mdata->p->vangle)) * tmpspeed)) / mdata->block_size;
		while (tmp_x >= 0 && tmp_y >= 0 && tmp_x <= mdata->maph && tmp_y <= mdata->mapw)
		{
			if (mdata->map[(int)tmp_y][(int)tmp_x])
			{
				--tmpspeed;
				tmp_x = (x + round(cos(to_rad(mdata->p->vangle)) * tmpspeed)) / mdata->block_size;
				tmp_y = (y - round(sin(to_rad(mdata->p->vangle)) * tmpspeed)) / mdata->block_size;
			}
			else
			{
				mdata->p->x = tmp_x * mdata->block_size;
				mdata->p->y = tmp_y * mdata->block_size;
				break ;
			}
		}
	}
	if (mdata->keys->down)
	{
		tmpspeed = speed;
		tmp_x = (x - round(cos(to_rad(mdata->p->vangle)) * tmpspeed)) / mdata->block_size;
		tmp_y = (y + round(sin(to_rad(mdata->p->vangle)) * tmpspeed)) / mdata->block_size;
		while (tmp_x >= 0 && tmp_y >= 0 && tmp_x <= mdata->maph && tmp_y <= mdata->mapw)
		{
			if (mdata->map[(int)tmp_y][(int)tmp_x])
			{
				--tmpspeed;
				tmp_x = (x - round(cos(to_rad(mdata->p->vangle)) * tmpspeed)) / mdata->block_size;
				tmp_y = (y + round(sin(to_rad(mdata->p->vangle)) * tmpspeed)) / mdata->block_size;
			}
			else
			{
				mdata->p->x = tmp_x * mdata->block_size;
				mdata->p->y = tmp_y * mdata->block_size;
				break ;
			}
		}
	}
	if (mdata->keys->a)
	{
		tmpspeed = speed;
		tmp_x = (x + round(cos(to_rad(mdata->p->vangle - 90)) * tmpspeed)) / mdata->block_size;
		tmp_y = (y - round(sin(to_rad(mdata->p->vangle - 90)) * tmpspeed)) / mdata->block_size;
		while (tmp_x >= 0 && tmp_y >= 0 && tmp_x <= mdata->maph && tmp_y <= mdata->mapw)
		{
			if (mdata->map[(int)tmp_y][(int)tmp_x])
			{
				--tmpspeed;
				tmp_x = (x + round(cos(to_rad(mdata->p->vangle - 90)) * tmpspeed)) / mdata->block_size;
				tmp_y = (y - round(sin(to_rad(mdata->p->vangle - 90)) * tmpspeed)) / mdata->block_size;
			}
			else
			{
				mdata->p->x = tmp_x * mdata->block_size;
				mdata->p->y = tmp_y * mdata->block_size;
				break ;
			}
		}
	}
	if (mdata->keys->d)
	{
		tmpspeed = speed;
		tmp_x = (x - round(cos(to_rad(mdata->p->vangle - 90)) * tmpspeed)) / mdata->block_size;
		tmp_y = (y + round(sin(to_rad(mdata->p->vangle - 90)) * tmpspeed)) / mdata->block_size;
		while (tmp_x >= 0 && tmp_y >= 0 && tmp_x <= mdata->maph && tmp_y <= mdata->mapw)
		{
			if (mdata->map[(int)tmp_y][(int)tmp_x])
			{
				--tmpspeed;
				tmp_x = (x - round(cos(to_rad(mdata->p->vangle - 90)) * tmpspeed)) / mdata->block_size;
				tmp_y = (y + round(sin(to_rad(mdata->p->vangle - 90)) * tmpspeed)) / mdata->block_size;
			}
			else
			{
				mdata->p->x = tmp_x * mdata->block_size;
				mdata->p->y = tmp_y * mdata->block_size;
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
	if (mdata->keys->left)
		mdata->p->vangle = (mdata->p->vangle == 0 ? 359 : mdata->p->vangle - 1);
	if (mdata->keys->right)
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

	if (mdata->active_map)
		mdata->ifov = reinit_img(mdata, mdata->ifov, mdata->w, mdata->h);
	else
		mdata->game = reinit_img(mdata, mdata->game, mdata->w, mdata->h);
	raycast(mdata);
	if (mdata->active_map)
	{
		mdata->img = mlx_merge_img(mdata->imap, mdata->ifov, to_vertex(mdata->w, mdata->h, 0, 0), mdata);
		mlx_put_image_to_window(mdata->mptr, mdata->wptr, mdata->img->ptr, 0, 0);
	}
	else
		mlx_put_image_to_window(mdata->mptr, mdata->wptr, mdata->game->ptr, 0, 0);

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
	mdata->game = init_img(mdata, mdata->w, mdata->h);
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
