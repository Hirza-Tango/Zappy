/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:12:12 by dslogrov          #+#    #+#             */
/*   Updated: 2020/01/28 13:38:28 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

/*
** Forgive me code spaghetti monster, for I have sinned
*/
static void	handle_monitor(t_state *s, int fd, char *buff)
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

static void	handle_player(t_state *s, int fd, char *buff, t_player *p)
{
	if (!strcmp(buff, "advance\n"))
		return (set_action(p, player_advance, 7.0 / s->time, NULL));
	else if (!strcmp(buff, "left\n"))
		return (set_action(p, player_left, 7.0 / s->time, NULL));
	else if (!strcmp(buff, "right\n"))
		return (set_action(p, player_right, 7.0 / s->time, NULL));
	else if (!strcmp(buff, "see\n"))
		return (set_action(p, player_see, 7.0 / s->time, NULL));
	else if (!strcmp(buff, "inventory\n"))
		return (set_action(p, player_inventory, 1.0 / s->time, NULL));
	else if (!strncmp(buff, "take", 4))
		return (set_action(p, player_take, 7.0 / s->time, strdup(buff + 4)));
	else if (!strncmp(buff, "put", 3))
		return (set_action(p, player_put, 7.0 / s->time, strdup(buff + 3)));
	else if (!strcmp(buff, "kick\n"))
		return (set_action(p, player_kick, 7.0 / s->time, NULL));
	else if (!strncmp(buff, "broadcast", 9))
		return (set_action(p, player_speak, 7.0 / s->time, strdup(buff + 10)));
	else if (!strcmp(buff, "incantation"))
		return (player_incantation_start(s, fd));
	else if (!strcmp(buff, "fork\n"))
		return (player_fork(s, fd));
	else if (!strcmp(buff, "connect_nbr\n"))
		return (player_connect_nbr(s, fd));
		send(fd, "Unknown command\n", 16, 0);
}

static void	handle_unknown(t_state *s, int fd, char *buff)
{
	size_t	i;
	t_list	*egg;

	if (!strcmp(buff, "GRAPHIC\n"))
		return (init_monitor(s, fd));
	i = -1UL;
	buff[strlen(buff) - 1] = 0;
	while (++i < s->n_teams)
		if (!strcmp(s->teams[i].name, buff))
			break ;
	if (i == s->n_teams)
		return ((void)send(fd, "Unknown team\n", 13, 0));
	if (!s->teams[i].nb_client)
		return ((void)send(fd, "Team is full\n", 13, 0));
	egg = NULL;
	if (s->teams[i].eggs &&
		!((t_egg *)(s->teams[i].eggs)->content)->hatch_time.tv_sec)
		(egg = s->teams[i].eggs) && (s->teams[i].eggs = egg->next);
	s->clients[fd].type = PLAYER;
	s->clients[fd].player = new_player(s, buff, egg ? egg->content : NULL);
	egg ? free(egg->content) : (void)0;
	egg ? free(egg) : (void)0;
	send(fd, buff, snprintf(buff, STRBUFF_SIZE, "%u\n%u %u\n",
		s->teams[i].nb_client, s->size_x, s->size_y), 0);
}

void		handle(t_state *s)
{
	static char	buff[STRBUFF_SIZE];
	int i;

	i = -1;
	while (++i <= s->max_fd)
	{
		if (!FD_ISSET(i, &s->fd_read))
			continue ;
		if (s->clients[i].type == PLAYER && s->clients[i].player->next_action)
			continue ;
		if (!cbuff_read(s->clients[i].buf_read, buff))
			continue ;
		if (s->clients[i].type == UNKNOWN)
			handle_unknown(s, i, buff);
		else if (s->clients[i].type == PLAYER)
			handle_player(s, i, buff, s->clients[i].player);
		else if (s->clients[i].type == MONITOR)
			handle_monitor(s, i, buff);
	}
}

void		client_read(t_state *s, int fd)
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
	cbuff_write(s->clients[fd].buf_read, buff);
}
