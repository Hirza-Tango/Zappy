/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 14:43:39 by dslogrov          #+#    #+#             */
/*   Updated: 2020/01/27 15:10:55 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *start, size_t length)
{
	size_t		i;
	char		*dest;

	i = 0;
	dest = malloc(sizeof(char) * (length + 1));
	if (!dest)
		return (NULL);
	while (i < length && start[i])
	{
		dest[i] = start[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
