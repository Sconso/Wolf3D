/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 21:56:22 by sconso            #+#    #+#             */
/*   Updated: 2014/05/15 18:05:52 by Myrkskog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>
#include <mlx.h>
#include <math.h>

void		fill_image(t_mdata *mdata, float x, float y, unsigned int color)
{
	int			r;

	if (x < 0 || y < 0 || x >= mdata->w || y >= mdata->h)
		return ;
	color = mlx_get_color_value(mdata->mptr, color);
	r = ((int)x * 4) + ((int)y * mdata->sizeline);
	mdata->idata[r] = (color & 0xFF);
	mdata->idata[r + 1] = (color & 0xFF00) >> 8;
	mdata->idata[r + 2] = (color & 0xFF0000) >> 16;
}

void			draw_line2(t_vertex v1, t_vertex v2, t_mdata *mdata)
{
	t_delta		delta;
	int			err;
	int			e2;

	delta.x1 = ft_abs(v2.x - v1.x);
	delta.y1 = ft_abs(v2.y - v1.y);
	delta.x2 = (v1.x < v2.x ? 1 : -1);
	delta.y2 = (v1.y < v2.y ? 1 : -1);
	err = (delta.x1 > delta.y1 ? delta.x1 : -delta.y1) / 2;
	while (42)
	{
		fill_image(mdata, v1.x, v1.y, v1.color);
		if (v1.x == (int)v2.x || v1.y == (int)v2.y)
			break ;
		e2 = err;
		if (e2 > -delta.x1)
		{
			err -= delta.y1;
			v1.x = v1.x + delta.x2;
		}
		if (e2 < delta.y1)
		{
			err += delta.x1;
			v1.y = v1.y + delta.y2;
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
	while (fabs(v1.x - v2.x) >= 1 || fabs(v1.y - v2.y) >= 1)
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

void			draw_rect(t_mdata *mdata, t_vertex v1, t_vertex v2)
{
	while (v1.y <= v2.y)
	{
		draw_line(v1, to_vertex(v2.x, v1.y, 0, v2.color), mdata);
		v1.y++;
	}
}

void			draw_circle(t_mdata *mdata, int ray, t_vertex center, int color)
{
	int			x;
	int			y;
	int			m;

	x = 0;
	y = ray;
	m = 5 - 4 * ray;
	while (x <= y)
	{
		fill_image(mdata, x + center.x, y + center.y, color);
		fill_image(mdata, y + center.x, x + center.y, color);
		fill_image(mdata, -x + center.x, y + center.y, color);
		fill_image(mdata, -y + center.x, x + center.y, color);
		fill_image(mdata, x + center.x, -y + center.y, color);
		fill_image(mdata, y + center.x, -x + center.y, color);
		fill_image(mdata, -x + center.x, -y + center.y, color);
		fill_image(mdata, -y + center.x, -x + center.y, color);
		if (m > 0)
		{
			y = y - 1;
			m = m - 8 * y;
		}
		x++;
		m = m + 8 * x + 4;
	}
}
