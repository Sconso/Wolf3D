/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wolf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 19:57:30 by sconso            #+#    #+#             */
/*   Updated: 2014/05/07 22:07:34 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <mlx.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ft_fc_conversion.h>

void			find_walls(t_mdata *mdata)
{
	(void)mdata;
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
	p->x = 5;
	p->y = 6;
	p->z = 0;
	p->fov = 60;
	p->vangle = 45;
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
	mdata->iptr = NULL;
	mdata->keys = init_keys();
	reset_values(mdata);
	mdata->p = init_player(mdata);
	mlx_do_key_autorepeatoff(mdata->mptr);
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
