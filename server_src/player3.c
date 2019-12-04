/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 15:28:02 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/04 18:01:27 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"
#include <math.h>

/*
**	Turns player direction from radians to 1-9 as defined in the PDF.
**	The radian value input is in the range [-pi,pi]
**	Internally, direction 1 is east, to correspond with 0 rads
*/

static int	normalise_direction(unsigned int player_direction, double radians)
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

static int	sound_direction(t_player *player1, t_player *player2,
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

void		player_fork(t_state *s, int fd, void *unused)
{
	//TODO:this
}

void		player_broadcast(t_state *s, int fd, void *message)
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

void		player_connect_nbr(t_state *s, int fd)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "%u\n",
		s->teams[s->clients[fd].player->team_no].nb_client);
	send(fd, buff, strlen(buff), 0);
}
