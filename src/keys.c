/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/17 18:26:58 by sconso            #+#    #+#             */
/*   Updated: 2014/05/17 18:39:47 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <stdio.h>
#include <stdlib.h>

static int		one_shot(int keycode, t_mdata *mdata)
{
	int			check;

	check = 0;
	if (keycode == ESC)
		exit(0);
	else if (keycode == ZERO && ++check)
		mdata->grid = (mdata->grid ? 0 : 1);
	else if (keycode == OPTION && ++check)
		mdata->rotate = (mdata->rotate ? 0 : 1);
	else if (keycode == DELETE && ++check)
		find_free_spot(mdata);
	return (check);
}

static void		move(int keycode, t_mdata *mdata, char state)
{
	if (keycode == RIGHT)
		mdata->keys->right = (state ? 1 : 0);
	if (keycode == LEFT)
		mdata->keys->left = (state ? 1 : 0);
	if (keycode == UP || keycode == W)
		mdata->keys->up = (state ? 1 : 0);
	if (keycode == DOWN || keycode == S)
		mdata->keys->down = (state ? 1 : 0);
	if (keycode == A)
		mdata->keys->a = (state ? 1 : 0);
	if (keycode == D)
		mdata->keys->d = (state ? 1 : 0);
}

int				key_press(int keycode, t_mdata *mdata)
{
	if (one_shot(keycode, mdata))
		return (1);
	printf("Code = %d\n", keycode);
	move(keycode, mdata, PRESS);
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

int				key_release(int keycode, t_mdata *mdata)
{
	move(keycode, mdata, RELEASE);
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
