/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 10:21:41 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/13 18:33:19 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*ret;

	ret = malloc(sizeof(t_list));
	if (!ret)
		return (NULL);
	if (!content)
	{
		ret->content = NULL;
		ret->content_size = 0;
	}
	else
	{
		ret->content = malloc(content_size);
		memcpy(ret->content, content, content_size);
		ret->content_size = content_size;
	}
	ret->next = NULL;
	return (ret);
}
