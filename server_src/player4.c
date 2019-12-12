/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 17:35:14 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/12 17:35:23 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void			player_connect_nbr(t_state *s, int fd)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "%u\n",
		s->teams[s->clients[fd].player->team_no].nb_client);
	send(fd, buff, strlen(buff), 0);
}
