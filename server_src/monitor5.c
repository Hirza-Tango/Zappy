/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 16:28:26 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/11 16:28:35 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	monitor_ebo(t_state *s, unsigned int egg_no)
{
	char		buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "ebo #%u\n", egg_no);
	send_all_monitors(s, buff);
}
