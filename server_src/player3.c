/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 15:28:02 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/12 18:17:55 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"
#include <math.h>

/*
**	Turns player direction from radians to 1-9 as defined in the PDF.
**	The radian value input is in the range [-pi,pi]
**	Internally, direction 1 is east, to correspond with 0 rads
*/

static int		normalise_direction(unsigned int player_direction, double radians)
{
	int direction;

	radians += (M_PI * 17 / 16);
	if (radians > M_2_PI)
		radians -= M_2_PI;
	direction = ((int)((radians / M_2_PI) * 8 - 2) + (player_direction * 2))
		% 8 + 1;
	return (direction);
}

/*
** y = mx + c;
** turn radians into direction from 1 - 9
*/

static int		sound_direction(t_player *player1, t_player *player2,
	t_state *state)
{
	long	diff_x;
	long	diff_y;

	diff_x = player2->x - player1->x;
	diff_y = player2->y - player1->y;
	if (!diff_x && !diff_y)
		return (0);
	else if (!diff_x)
	{
		return (normalise_direction(player1->direction,
			diff_y > 0 ? M_PI_2 : -M_PI_2));
	}
	if (diff_x > state->size_x / 2)
		diff_x -= state->size_x;
	else if (-diff_x > state->size_x / 2)
		diff_x += state->size_x;
	if (diff_y > state->size_y / 2)
		diff_y -= state->size_y;
	else if (-diff_y > state->size_y / 2)
		diff_y += state->size_y;
	return (normalise_direction(player1->direction, atan2(diff_y, diff_x)));
}

static void		player_fork_end(t_state *s, int fd, void *unused)
{
	t_egg	egg;
	struct	timespec t;
	t_list	*last;

	(void)unused;
	egg.egg_no = s->n_eggs++;
	egg.player_no = s->n_players++;
	egg.team_no = s->clients[fd].player->team_no;
	clock_gettime(CLOCK_MONOTONIC, &t);
	memcpy(&egg.hatch_time, &t, sizeof(struct timespec));
	memcpy(&egg.death_time, &t, sizeof(struct timespec));
	add_time(&egg.hatch_time, 600.0 / s->time);
	add_time(&egg.death_time, (600.0 + 1260.0) / s->time);
	egg.x = s->clients[fd].player->x;
	egg.y = s->clients[fd].player->y;
	ft_lstappend(&s->teams[egg.team_no].eggs, ft_lstnew(&egg, sizeof(egg)));
	last = s->teams[egg.team_no].eggs;
	while (last->next)
		last = last->next;
	monitor_enw(s, -1, last->content);
	send(fd, "ok\n", 3, 0);
}

void			player_fork(t_state *s, int fd)
{
	set_action(s->clients[fd].player, player_fork_end, 42.0 / s->time, 0);
	monitor_pfk(s, s->clients[fd].player);
}

void			player_broadcast(t_state *s, int fd, void *message)
{
	t_player	*player;
	int			i;
	char		direction;

	player = s->clients[fd].player;
	i = -1;
	while (++i)
		if (s->clients[i].type == PLAYER)
		{
			send(i, "message ", 8, 0);
			direction = sound_direction(player, s->clients[i].player, s) + '0';
			send(i, &direction, 1, 0);
			send(i, ",", 1, 0);
			send(i, message, strlen(message), 0);
		}
	send(fd, "ok\n", 3, 0);
	monitor_pbc(s, player, message);
	free(message);
}
