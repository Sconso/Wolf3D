/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/04/27 19:40:46 by sconso            #+#    #+#             */
/*   Updated: 2014/05/06 20:28:56 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <mlx.h>
#include "/usr/X11/include/X11/X.h"
#include <stdlib.h>
#include <fcntl.h>

static t_keys	*init_keys(void)
{
	t_keys		*keys;

	keys = (t_keys *)malloc(sizeof(t_keys));
	ft_assert(keys != NULL, "Cant malloc the keys.. Exiting...\n");
	keys->right = 0;
	keys->left = 0;
	keys->up = 0;
	keys->down = 0;
	keys->more_height = 0;
	keys->less_height = 0;
	keys->more_angle = 0;
	keys->less_angle = 0;
	keys->zoom = 0;
	keys->unzoom = 0;
	keys->rot_z = 0;
	return (keys);
}

static void		reset_init(t_mdata *mdata, float *espace, float *w, float *h)
{
	*espace = ESPACE;
	*w = 0;
	*h = 0;
	mdata->angle = ANGLE;
	mdata->hauteur = HAUTEUR;
	mdata->shadows = 0;
}

void			reset_values(t_mdata *mdata)
{
	float		width;
	float		height;
	int			i;
	int			j;
	float		espace;

	reset_init(mdata, &espace, &width, &height);
	i = -1;
	while (mdata->map[++i])
	{
		++height;
		j = -1;
		while (mdata->map[i][++j] != -999)
			;
		width = (j > width ? j : width);
	}
	while (width * espace > mdata->w || height * espace > mdata->h)
		espace--;
	mdata->espace_x = espace;
	mdata->espace_y = espace;
	mdata->sx = width;
	mdata->sy = height;
	mdata->x = (mdata->w / 2) - (width * mdata->espace_x / 2);
	mdata->y = (mdata->h / 2) - (height * mdata->espace_x / 2);
}

int				**ft_init(char *map)
{
	int			fd;
	int			**matrix;

	if (0)
		ft_ext(map, ".fdf");
	fd = ft_open(map, O_RDWR);
	matrix = create_matrix(fd);
	ft_close(fd);
	fd = ft_open(map, O_RDONLY);
	matrix = fill_matrix(fd, matrix);
	return (matrix);
}

t_mdata			*init_mlx(int **map, int userwidth, int userheight)
{
	t_mdata		*mdata;

	mdata = (t_mdata *)malloc(sizeof(t_mdata));
	ft_assert(mdata != NULL, "Cant malloc the mdata... Exiting...\n");
	mdata->w = (userwidth > 0 ? userwidth : 900);
	mdata->h = (userheight > 0 ? userheight : mdata->w * 9 / 16);
	mdata->map = map;
	mdata->mptr = mlx_init();
	mdata->wptr = mlx_new_window(mdata->mptr, mdata->w, mdata->h, "FDF");
	mdata->iptr = NULL;
	mdata->keys = init_keys();
	mdata->debug = 0;
	mdata->shadows = 0;
	reset_values(mdata);
	mlx_do_key_autorepeatoff(mdata->mptr);
	mlx_hook(mdata->wptr, KeyPress, KeyPressMask, &key_press, mdata);
	mlx_hook(mdata->wptr, KeyRelease, KeyReleaseMask, &key_release, mdata);
	mlx_expose_hook(mdata->wptr, expose, mdata);
	mlx_loop_hook(mdata->mptr, loop, mdata);
	mlx_loop(mdata->mptr);
	return (mdata);
}
