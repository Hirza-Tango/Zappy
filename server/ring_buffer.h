/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_buffer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 17:04:08 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/19 17:25:54 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RING_BUFFER_H
# define RING_BUFFER_H

# include <string.h>
# include <stdlib.h>
# include "../mini_libft/libft.h"

struct	s_cbuff
{
	char	*buffer;
	void	*head;
	void	*tail;
	size_t	capacity;
	size_t	entry_size;
	char	is_full;
};

typedef struct s_cbuff* t_cbuff;

t_cbuff	cbuff_create(size_t entry_size, size_t n_entries);
void	cbuff_destroy(t_cbuff cbuff);
void	cbuff_reset(t_cbuff cbuff);
int		cbuff_write(t_cbuff cbuff, char *data);
int		cbuff_read(t_cbuff cbuff, char *string_buff);
char	cbuff_isempty(t_cbuff cbuff);
char	cbuff_isfull(t_cbuff cbuff);
size_t	cbuff_cap(t_cbuff cbuff);
size_t	cbuff_size(t_cbuff cbuff);
size_t	cbuff_ready(t_cbuff cbuff);

#endif
