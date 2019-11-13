/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:54:31 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/13 17:07:18 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

/*
**	Each team is 6 players
**	min resources per team:
**		Linemate:	6 + 3 + 6 + 1.5 + 1.5 + 1 + 2	= 21
**		Deraumere:	0 + 3 + 0 + 1.5 + 3 + 2 + 2		= 11.5
**		Sibur:		0 + 3 + 3 + 3 + 2 + 2			= 13
**		Mendiane:	4.5 + 2							= 6.5
**		Phiras:		0 + 6 + 1.5 + 0 + 1 + 2			= 10.5
**		Thystame:	1								= 1
**		Resources will be availible at min per team * teams * 1.5
*/

void	gen_board(t_game_state *s)
{
	int i;
	int j;

	s->board = malloc(sizeof(LIST_HEAD *) * s->size_x);
	//for resource type:
		//for resource number:
			//place at random (possibly overlapping position)

}
