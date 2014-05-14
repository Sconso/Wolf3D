/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utilities.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/11 22:07:17 by sconso            #+#    #+#             */
/*   Updated: 2014/05/14 20:06:00 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf.h>

int				ft_abs(int nb)
{
	if (nb < 0)
		return (-nb);
	return (nb);
}

t_vertex		to_vertex(float x, float y, float z, int color)
{
	t_vertex	v;

	v.x = (float)x;
	v.y = (float)y;
	v.z = (float)z;
	v.w = 1.0;
	v.color = color;
	return (v);
}
