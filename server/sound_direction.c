/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_direction.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 13:45:13 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/19 14:10:05 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "zappy_server.h"

/*
**	Turns player direction from radians to 1-9 as defined in the PDF.
**	The radian value input is in the range [-pi,pi]
**	Internally, direction 1 is east, to correspond with 0 rads
*/

int	normalise_direction(unsigned int player_direction, double radians)
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

int	sound_direction(t_player *player1, t_player *player2,
	t_game_state game_state)
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
	if (diff_x > game_state.size_x / 2)
		diff_x -= game_state.size_x;
	else if (-diff_x > game_state.size_x / 2)
		diff_x += game_state.size_x;
	if (diff_y > game_state.size_y / 2)
		diff_y -= game_state.size_y;
	else if (-diff_y > game_state.size_y / 2)
		diff_y += game_state.size_y;
	return (normalise_direction(player1->direction, atan2(diff_y, diff_x)));
}
