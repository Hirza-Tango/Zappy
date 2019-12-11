/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:12:12 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/11 14:08:27 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

/*
** Forgive me code spaghetti monster, for I have sinned
*/
void	handle_monitor(t_state *s, int fd, char *buff)
{
	char	*arg1;
	char	*arg2;

	if (!strcmp(buff, "msz\n"))
		monitor_msz(s, fd);
	else if (!strncmp(buff, "bct", 3))
		(arg1 = strchr(buff, ' ')) && (arg2 = strrchr(buff, ' ')) &&
			arg1 != arg2 ? monitor_bct(s, fd, atoi(arg1), atoi(arg2)) :
			send(fd, "sbp\n", 4, 0);
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
	else
		send(fd, "suc\n", 4, 0);
}

//TODO: stop handling until ready
//TODO: reduce to 25 lines or split
void	handle_player(t_state *s, int fd, char *buff)
{
	t_player *player = s->clients[fd].player;

	if (!strcmp(buff, "advance\n"))
		set_action(player, player_advance, 7.0 / s->time, NULL);
	else if (!strcmp(buff, "left\n"))
		set_action(player, player_left, 7.0 / s->time, NULL);
	else if (!strcmp(buff, "right\n"))
		set_action(player, player_right, 7.0 / s->time, NULL);
	else if (!strcmp(buff, "see\n"))
		set_action(player, player_see, 7.0 / s->time, NULL);
	else if (!strcmp(buff, "inventory\n"))
		set_action(player, player_inventory, 1.0 / s->time, NULL);
	else if (!strncmp(buff, "take", 4))
		set_action(player, player_take, 7.0 / s->time, strdup(buff + 4));
	else if (!strncmp(buff, "put", 3))
		set_action(player, player_put, 7.0 / s->time, strdup(buff + 3));
	else if (!strcmp(buff, "kick\n"))
		set_action(player, player_kick, 7.0 / s->time, NULL);
	else if (!strncmp(buff, "broadcast", 9))
		set_action(player, player_broadcast, 7.0 / s->time, strdup(buff + 10));
	else if (!strcmp(buff, "incantation"))
		player_incantation_start(s, fd);
	else if (!strcmp(buff, "fork\n"))
		set_action(player, player_kick, 42.0 / s->time, NULL);
	else if (!strcmp(buff, "connect_nbr\n"))
		player_connect_nbr(s, fd);
	else
		send(fd, "Unknown command\n", 16, 0);
}

void	handle_unknown(t_state *s, int fd, char *buff)
{
	size_t	i;
	t_egg	*egg;

	if (!strcmp(buff, "GRAPHIC\n"))
	{
		s->clients[fd].type = MONITOR;
		return (init_monitor(s, fd));
	}
	i = -1UL;
	buff[strlen(buff) - 1] = 0;
	while (++i < s->n_teams)
		if (!strcmp(s->teams[i].name, buff))
			break ;
	if (i == s->n_teams)
		return (send(fd, "Unknown team\n", 13, 0));
	if (!s->teams[i].nb_client)
		return ((void)send(fd, "Team is full\n", 13, 0));
	if ((egg = s->teams[i].eggs))
		s->teams[i].eggs = s->teams[i].eggs->next;
	s->clients[fd].type = PLAYER;
	s->clients[fd].player = new_player(s, buff, egg);
	egg ? free(egg) : (void)0;
	send(fd, buff, snprintf(buff, STRBUFF_SIZE, "%u\n%u %u\n",
		s->teams[i].nb_client, s->size_x, s->size_y), 0);
	monitor_pnw(s, -1, s->clients[fd].player);
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

//FIXME: randomly won't read commands
//FIXME: commands aren't added to queue
void	client_read(t_state *s, int fd)
{
	char	buff[STRBUFF_SIZE];
	int		r;

	r = recv(fd, buff, STRBUFF_SIZE - 1, 0);
	if (r <= 0)
	{
		close(fd);
		if (s->clients[fd].type == PLAYER)
			player_death(s, fd);
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
