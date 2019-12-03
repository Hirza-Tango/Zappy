/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:12:12 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 17:02:59 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	handle_monitor(t_state *s, int fd, char *buff)
{
	if (!strcmp(buff, "msz\n"))
		monitor_msz(s, fd);
	else if (!strncmp(buff, "bct", 3))
	{
		//TODO: test strtok implementation
		monitor_bct(s, fd, atoi(strtok_r(buff + 3, " ", &buff)),
			atoi(strtok_r(buff + 3, " ", &buff)));
		strtok_r(NULL, "", &buff);
	}
	else if (!strcmp(buff, "mct\n"))
		monitor_mct(s, fd);
	else if (!strcmp(buff, "tna\n"))
		monitor_tna(s, fd);
	else if (!strncmp(buff, "ppo", 3))
		monitor_ppo(s, fd, atoi(strchr(buff + 3, '#') + 1));
	else if (!strncmp(buff, "plv", 3))
		monitor_plv(s, fd, atoi(strchr(buff + 3, '#') + 1));
	else if (!strncmp(buff, "pin", 3))
		monitor_pin(s, fd, atoi(strchr(buff + 3, '#') + 1));
	else if (!strcmp(buff, "sgt\n"))
		monitor_sgt(s, fd);
	else if (!strncmp(buff, "sst", 3))
		monitor_sst(s, fd, (s->time = atoi(buff + 3)));
	send(fd, "suc\n", 4, 0);
}

void	handle_player(t_state *s, int fd, char *buff)
{
	t_player *player = s->clients[fd].player;
	if (!strcmp(buff, "advance\n"))
		set_action(player, player_advance, 7 / s->time, NULL);
	else if (!strcmp(buff, "left\n"))
		set_action(player, player_left, 7 / s->time, NULL);
	else if (!strcmp(buff, "right\n"))
		set_action(player, player_right, 7 / s->time, NULL);
	else if (!strcmp(buff, "see\n"))
		set_action(player, player_see, 7 / s->time, NULL);
	else if (!strcmp(buff, "inventory\n"))
		set_action(player, player_inventory, 1 / s->time, NULL);
	else if (!strncmp(buff, "take", 4))
		set_action(player, player_take, 7 / s->time, strdup(buff + 4));
	else if (!strncmp(buff, "put", 3))
		set_action(player, player_put, 7 / s->time, strdup(buff + 3));
	else if (!strcmp(buff, "kick\n"))
		set_action(player, player_kick, 7 / s->time, NULL);
	else if (!strncmp(buff, "broadcast", 9))
		set_action(player, player_broadcast, 7 / s->time, strdup(buff + 10));
	else if (!strcmp(buff, "incantation"))
		player_incantation_start(s, fd);
	else if (!strcmp(buff, "fork\n"))
		set_action(player, player_kick, 42 / s->time, NULL);
	else if (!strcmp(buff, "connect_nbr\n"))
		player_connect_nbr(s, fd);
}

void	handle_unknown(t_state *s, int fd, char *buff)
{
	size_t	i;
	t_egg	*egg;

	if (!strcmp(buff, "GRAPHIC\n"))
	{
		s->clients[fd].type = MONITOR;
		init_monitor(s, fd);
		return ;
	}
	i = 0;
	buff[strlen(buff) - 1] = 0;
	while (i < s->n_teams)
	{
		if (!strcmp(s->teams[i].name, buff))
		{
			if (!s->teams[i].nb_client)
				send(fd, "Team is full\n", 13, 0);
			else
			{
				egg = NULL;//TODO: if egg is available, spawn at egg
				s->clients[fd].type = PLAYER;
				s->clients[fd].player = new_player(s, fd, buff, egg);
				send(fd, buff, snprintf(buff, STRBUFF_SIZE, "%u\n%u %u\n",
					s->teams[i].nb_client, s->size_x, s->size_y), 0);
				monitor_pnw(s, -1, s->clients[fd].player);
			}
			return ;
		}
		i++;
	}
	send(fd, "Unknown team\n", 13, 0);
}

void	handle(t_state *s)
{
	static char	buff[STRBUFF_SIZE];
	int i;

	i = -1;
	while (++i <= s->max_fd)
	{
		if (!FD_ISSET(i, &s->fd_read))
			continue ;
		if (!cbuff_read(s->clients[i].buf_read, buff))
			continue ;
		if (s->clients[i].type == UNKNOWN)
			handle_unknown(s, i, buff);
		else if (s->clients[i].type == PLAYER)
			handle_player(s, i, buff);
		else if (s->clients[i].type == MONITOR)
			handle_monitor(s, i, buff);
	}
}

void	client_read(t_state *s, int fd)
{
	char	buff[STRBUFF_SIZE];
	int		r;

	r = recv(fd, buff, STRBUFF_SIZE - 1, 0);
	if (r <= 0)
	{
		close(fd);
		if (s->clients[fd].type == PLAYER)
		{
			s->teams[s->clients[fd].player->team_no].nb_client++;
			s->n_players--;
			free(s->clients[fd].player);
		}
		//TODO: possibly drop stuff?
		FD_CLR(fd, &(s->fd_read));
		cbuff_destroy(s->clients[fd].buf_read);
		bzero(&s->clients[fd], sizeof(t_client));
		printf("client #%d disconnected\n", fd);
		return ;
	}
	buff[r] = 0;
	//TODO: handle newlines
	// Perhaps stringsplit?
	cbuff_write(s->clients[fd].buf_read, buff);
}
