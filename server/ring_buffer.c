/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_buffer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 17:03:35 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/21 09:30:43 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ring_buffer.h"

t_cbuff	cbuff_create(size_t entry_size, size_t n_entries)
{
	t_cbuff	cbuff;

	cbuff = malloc(sizeof(struct s_cbuff));
	cbuff->buffer = (char *)malloc(entry_size * n_entries);
	bzero(cbuff->buffer, entry_size * n_entries);
	cbuff->head = cbuff->buffer;
	cbuff->tail = cbuff->buffer;
	cbuff->capacity = n_entries;
	cbuff->is_full = 0;
	cbuff->entry_size = entry_size;
	return (cbuff);
}

void	cbuff_destroy(t_cbuff cbuff)
{
	free(cbuff->buffer);
	free(cbuff);
}

int		cbuff_write(t_cbuff cbuff, char *data)
{
	const size_t len = data ? strnlen(data, cbuff->entry_size) : 0;

	if (!len || len == cbuff->entry_size || cbuff->is_full)
		return (-1);
	if (((char *)cbuff->head)[0])
	{
		if (strlcat(cbuff->head, data, cbuff->entry_size) >= cbuff->entry_size)
		{
			((char *)cbuff->head)[0] = 0;
			return(-2);
		}
	}
	else
		strlcpy(cbuff->head, data, cbuff->entry_size);
	if (memchr(cbuff->head, '\n', cbuff->entry_size))
	{
		cbuff->head += cbuff->entry_size;
		if (cbuff->head >= cbuff->buffer + cbuff->entry_size * cbuff->capacity)
			cbuff->head = cbuff->buffer;
		if (cbuff->head == cbuff->tail)
			cbuff->is_full = 1;
	}
	return (0);
}

int		cbuff_read(t_cbuff cbuff, char *string_buff)
{
	int size;

	if (!memchr(cbuff->tail, '\n', cbuff->entry_size) ||
		(cbuff->head == cbuff->tail && !cbuff->is_full))
		return (0);
	size = strlcpy(string_buff, cbuff->tail, cbuff->entry_size);
	cbuff->tail += cbuff->entry_size;
	if (cbuff->tail >= cbuff->buffer + cbuff->entry_size * cbuff->capacity)
			cbuff->tail = cbuff->buffer;
	cbuff->is_full = 0;
	return (size);
}
