/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 10:52:30 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 11:29:21 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

t_player	*new_player(t_state *s, int fd, char *buff, t_egg *egg)
{
	t_player	*player;
	int			i;

	player = (t_player *)malloc(sizeof(t_player));
	bzero(&player, sizeof(player));
	player->player_no = s->n_players++;
	i = 0;
	while (strcmp(s->teams[i].name, buff))
		i++;
	s->teams[i].nb_client--;
	player->team_no = i;
	player->direction = RAND(4);
	player->x = egg ? egg->x : RAND(s->size_x);
	player->y = egg ? egg->y : RAND(s->size_y);
	return (player);
}
