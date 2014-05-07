/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/06 19:59:07 by sconso            #+#    #+#             */
/*   Updated: 2014/05/07 21:11:47 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

# include <structs.h>

# define BLOCK_SIZE 64

/*
** draw.c
*/
void	draw_line(t_vertex v1, t_vertex v2, t_mdata *mdata);

/*
** ft_errors.c
*/
void	ft_exit(char *str);
void	ft_assert(int check, char *str);
void	ft_strerror(int errnum);
void	ft_error(char *line, int lnb, char *str);

/*
** ft_file.c
*/
int		ft_open(char *file, int mode);
void	ft_close(int fd);
void	ft_ext(char *file, char *ext);

/*
** ft_matrix.h
*/
int		**create_matrix(int fd);
int		**fill_matrix(int fd, int **matrix);
void	print_matrix(int **matrix);

#endif
