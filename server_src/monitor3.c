/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:33:13 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 10:21:37 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	monitor_pex(t_state *s, t_player *p)
{
	char	buff[256];
	snprintf(buff, 256, "pex #%zu\n", p->player_no);
	send_all_monitors(s, buff);
}

void	monitor_pbc(t_state *s, t_player *p, char *message)
{
	char	buff[256];
	snprintf(buff, 256, "pbc #%zu %s\n", p->player_no, message);
	send_all_monitors(s, buff);
}

/* TODO: when implementing levelling, players can't act during incantation*/
void	monitor_pic(t_state *s, t_player *player1)
{
	char		buff[256];
	size_t		i;
	t_player	*player;

	snprintf(buff, 256, "pic %u %u %u", player1->x, player1->y, player1->level);
	send_all_monitors(s, buff);
	i = -1;
	while(++i < s->max_clients)
	{
		if (s->clients[i].type != PLAYER)
			continue ;
		player = s->clients[i].player;
		if (player->x == player1->x && player->y == player1->y &&
			player1->level)
		{
			snprintf(buff, 256, " #%zu", player->player_no);
			send_all_monitors(s, buff);
		}
	}
	send_all_monitors(s, "\n");
}

void	monitor_pie(t_state *s, unsigned int x, unsigned int y, int r)
{
	char	buff[256];

	snprintf(buff, 256, "pie %u %u %d\n", x, y, r);
	send_all_monitors(s, buff);
}

void	monitor_pfk(t_state *s, t_player *p)
{
	char	buff[256];

	snprintf(buff, 256, "pfk #%zu\n", p->player_no);
	send_all_monitors(s, buff);
}
