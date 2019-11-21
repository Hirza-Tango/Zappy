/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:54:31 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/21 09:24:19 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

long	resource_compare(const void *a, const void *b)
{
	if (((t_resource *)a)->x != ((t_resource *)b)->x)
		return ((t_resource *)a)->x - ((t_resource *)b)->x;
	return (((t_resource *)a)->y - ((t_resource *)b)->y);
}

long	player_compare(const void *a, const void *b)
{
	if (((t_player *)a)->x != ((t_player *)b)->x)
		return ((t_player *)a)->x - ((t_player *)b)->x;
	return (((t_player *)a)->y - ((t_player *)b)->y);
}

/*
**	this function assumes that the lists are sorted
**		using resource_compare() and player_compare() respectively
*/

char	*look(t_state *s, unsigned x, unsigned y)
{
	static char	buffer[STRBUFF_SIZE];
	t_list		*current;

	buffer[0] = 0;
	current = s->resources;
	while (current && ((t_resource *)current->content)->x < x)
		current = current->next;
	while (s->resources && ((t_resource *)current->content)->y < y)
		current = current->next;
	while (current && ((t_resource *)current->content)->x == x &&
		((t_resource *)current->content)->y == y)
	{
		strlcat(buffer, " ", STRBUFF_SIZE);
		strlcat(buffer, g_resources_info
			[((t_resource *)current->content)->resource_no].name, STRBUFF_SIZE);
	}
	current = s->players;
	while (current && ((t_player *)current->content)->x < x)
		current = current->next;
	while (s->resources && ((t_player *)current->content)->y < y)
		current = current->next;
	while (current && ((t_player *)current->content)->x == x &&
		((t_resource *)current->content)->y == y)
		strlcat(buffer, " player", STRBUFF_SIZE);
	return (buffer);
}

void	gen_board(t_state *s)
{
	size_t		i;
	size_t		j;
	t_resource	current;

	s->resources = 0;
	((t_resource_entry *)g_resources_info)[0].team_requirement =
		s->size_x * s->size_y / LIFE_DURATION * TEAM_MAX_SIZE * FOOD_MULTIPLIER;
	i = 0;
	while (i < NUM_RESOURCES)
	{
		j = 0;
		while (j < g_resources_info[i].team_requirement * RESOURCE_MULTIPLIER)
		{
			current.resource_no = i;
			current.x = RAND(s->size_x);
			current.y = RAND(s->size_y);
			ft_lstadd(&(s->resources), ft_lstnew(&current, sizeof(current)));
			j++;
		}
		i++;
	}
	ft_lstsort(&(s->resources), &resource_compare);
}
