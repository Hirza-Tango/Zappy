/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 11:03:02 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/13 17:25:43 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_d_list	*ft_dlstnew(void const *content, size_t content_size)
{
	t_d_list	*ret;

	ret = malloc(sizeof(t_d_list));
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
	ret->prev = NULL;
	return (ret);
}
