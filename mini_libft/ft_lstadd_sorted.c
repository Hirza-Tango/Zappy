/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_sorted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 11:33:20 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/19 11:55:25 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Inserts an element at at a sorted position in the given list
*/

long	ft_lstadd_sorted(t_list **list, t_list *elem,
	long (*f)(const void *, const void *))
{
	t_list	*current;
	int		i;

	current = *list;
	if (!current)
	{
		current = elem;
		return (0);
	}
	i = 0;
	while (current && current->next && f(current->content, elem->content) < 0)
	{
		current = current->next;
		i++;
	}
	elem->next = current->next;
	current->next = elem;
	return (i);
}
