/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:12:12 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/26 10:34:17 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	handle_monitor(t_state *s, int fd)
{

}

void	handle_player(t_state *s, int fd)
{

}

t_player	*new_player(t_state *s, int fd, char *buff, t_egg *egg)
{
	t_list		*elem;
	t_player	player;
	int			i;

	bzero(&player, sizeof(player));
	player.player_no = fd;
	i = 0;
	while (strcmp(s->teams[i].name, buff))
		i++;
	s->teams[i].nb_client--;
	player.team_no = i;
	elem = ft_lstnew(&player, sizeof(t_player));
	ft_lstadd(&s->players, elem);
	player.direction = RAND(4);
	player.x = egg ? egg->x : RAND(s->size_x);
	player.y = egg ? egg->y : RAND(s->size_y);
	return (elem->content);
}

void	handle_unknown(t_state *s, int fd)
{
	static char	buff[256];
	size_t	i;
	t_egg	*egg;

	if (!cbuff_read(s->clients[fd].buf_read, buff))
		return ;
	if (!strcmp(buff, "GRAPHIC\n"))
	{
		s->clients[fd].type = MONITOR;
		//TODO: send stuff
		return ;
	}
	i = 0;
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
				send(fd, buff, snprintf(buff, 256, "%u\n%u %u\n",
					s->teams[i].nb_client, s->size_x, s->size_y), 0);
			}
			return ;
		}
		i++;
	}
	send(fd, "Unknown team\n", 13, 0);
}

void	client_read(t_state *s, int fd)
{
	char	buff[256];
	int		r;

	recv(fd, buff, 255, 0);
	if (r <= 0)
	{
		close(fd);
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
