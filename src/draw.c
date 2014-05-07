/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/04/27 19:36:40 by sconso            #+#    #+#             */
/*   Updated: 2014/05/07 21:02:42 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <mlx.h>
#include <math.h>

static void		fill_image(t_mdata *mdata, float x, float y, unsigned int color)
{
	int			r;
	int			i;
	int			j;

	if (x < 0 || y < 0 || x > mdata->w || y > mdata->h)
		return ;
	color = mlx_get_color_value(mdata->mptr, color);
	r = ((int)x * 4) + ((int)y * mdata->sizeline);
	mdata->idata[r] = (color & 0xFF);
	mdata->idata[r + 1] = (color & 0xFF00) >> 8;
	mdata->idata[r + 2] = (color & 0xFF0000) >> 16;
	if ((i = 0))
	{
		while (++i < mdata->espace_x - 1)
		{
			j = 0;
			while (++j < mdata->espace_y - 1)
			{
				r = (((int)x + i) * 4) + (((int)y + j) * mdata->sizeline);
				mdata->idata[r] = (0x000000 & 0xFF);
				mdata->idata[r + 1] = (0x000000 & 0xFF00) >> 8;
				mdata->idata[r + 2] = (0x000000 & 0xFF0000) >> 16;
			}
		}
	}
}

void			draw_line(t_vertex v1, t_vertex v2, t_mdata *mdata)
{
	t_delta		delta;

	delta.x1 = 1;
	delta.y1 = 1;
	delta.x2 = fabs(v1.x - v2.x);
	delta.y2 = fabs(v1.y - v2.y);
	if (delta.y2 && delta.x2 / delta.y2 < 1)
		delta.x1 = (delta.y2 ? delta.x2 / delta.y2 : 0);
	else if (delta.x2)
		delta.y1 = (delta.x2 ? delta.y2 / delta.x2 : 0);
	while (ft_abs(v1.x - v2.x) >= 1 || ft_abs(v1.y - v2.y) >= 1)
	{
		fill_image(mdata, v1.x, v1.y, v1.color);
		if (v1.x < v2.x)
			v1.x += delta.x1;
		else if (v1.x > v2.x)
			v1.x -= delta.x1;
		if (v1.y < v2.y)
			v1.y += delta.y1;
		else if (v1.y > v2.y)
			v1.y -= delta.y1;
	}
}
