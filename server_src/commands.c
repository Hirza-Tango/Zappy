/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:12:12 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 10:50:31 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	handle_monitor(t_state *s, int fd, char *buff)
{

}

void	handle_player(t_state *s, int fd, char *buff)
{

}

t_player	*new_player(t_state *s, int fd, char *buff, t_egg *egg)
{
	t_list		*elem;
	t_player	player;
	int			i;

	bzero(&player, sizeof(player));
	player.player_no = s->n_players++;
	i = 0;
	while (strcmp(s->teams[i].name, buff))
		i++;
	s->teams[i].nb_client--;
	player.team_no = i;
	player.direction = RAND(4);
	player.x = egg ? egg->x : RAND(s->size_x);
	player.y = egg ? egg->y : RAND(s->size_y);
	elem = ft_lstnew(&player, sizeof(t_player));
	return (elem->content);
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
			if (s->teams[i].nb_client < 1)
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

	r = recv(fd, buff, 255, 0);
	if (r <= 0)
	{
		close(fd);
		if (s->clients[fd].type == PLAYER)
		{
			s->teams[s->clients[fd].player->team_no].nb_client++;
			s->n_players--;
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
