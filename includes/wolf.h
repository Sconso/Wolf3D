/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 19:59:07 by sconso            #+#    #+#             */
/*   Updated: 2014/05/14 22:07:33 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

# include <structs.h>

# define BLOCK_SIZE 64

# define ESC (65307)
# define RIGHT (65363)
# define LEFT (65361)
# define UP (65362)
# define DOWN (65364)
# define W (119)
# define A (97)
# define S (115)
# define D (100)

/*
** draw.c
*/
void		draw_line(t_vertex v1, t_vertex v2, t_mdata *mdata);

/*
** draw_utilities.c
*/
int			ft_abs(int nb);
t_vertex	to_vertex(float x, float y, float z, int color);

/*
** ft_errors.c
*/
void		ft_exit(char *str);
void		ft_assert(int check, char *str);
void		ft_strerror(int errnum);
void		ft_error(char *line, int lnb, char *str);

/*
** ft_file.c
*/
int			ft_open(char *file, int mode);
void		ft_close(int fd);
void		ft_ext(char *file, char *ext);

/*
** ft_matrix.h
*/
int			**create_matrix(int fd);
int			**fill_matrix(int fd, int **matrix);
void		print_matrix(int **matrix);

#endif
