/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_server.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 18:42:42 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/13 18:44:05 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPPY_SERVER_H
# define ZAPPY_SERVER_H

# include <sys/queue.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include "../mini_libft/libft.h"

# define MAX(x,y) x > y ? x : y
# define RAND(x) (rand() % (x + 1))

enum					e_xit_codes
{
	INVALID_FLAG = 2,
	MISSING_OPTION,
	BAD_ADDR,
	BIND_FAIL,
	SELECT_FAIL,
	LISTEN_FAIL
};

enum					e_resources
{
	FOOD,
	LINEMATE,
	DERAUMERE,
	SIBUR,
	MENDIANE,
	PHIRAS,
	THYSTAME
};

# define NUM_RESOURCES 7
# define NUM_LEVELS 7
# define RESOURCE_MULTIPLIER s->n_teams * 1.5
# define TEAM_MAX_SIZE 6
# define LIFE_DURATION 126
# define FOOD_MULTIPLIER 2

typedef struct			s_resource{
	char	*name;
	int		team_requirement;
	int		level_requirements[NUM_LEVELS];
}						t_resource;

static const t_resource	g_resources[NUM_RESOURCES] = {
	{"food", 0, {0, 0, 0, 0, 0, 0, 0}},
	{"linemate", 21, {1, 1, 2, 1, 1, 1, 2}},
	{"deraumere", 12, {0, 1, 0, 1, 2, 2, 2}},
	{"sibur", 13, {0, 1, 1, 2, 1, 3, 2}},
	{"mendiane", 7, {0, 0, 0, 0, 3, 0, 2}},
	{"phiras", 11, {0, 0, 2, 1, 0, 1, 2}},
	{"thystame", 1, {0, 0, 0, 0, 0, 0, 1}}
};

typedef struct			s_player
{
	unsigned int	x;
	unsigned int	y;
	unsigned char	direction;
}						t_player;

typedef struct			s_server_state
{
	char			*port;
	int				fd_listen;
	unsigned char	current_players;
	unsigned char	allowed_players;
	int				max_fd;
	fd_set			fd_read;
}						t_server_state;

typedef struct			s_game_state
{
	t_list			***board;
	unsigned int	size_x;
	unsigned int	size_y;
	char			**teams;
	size_t			n_teams;
	size_t			time;
}						t_game_state;

void					exit_error(char *message, int code);
void					parse_args(int argc, char **argv,
	t_game_state *game_state, t_server_state *server_state);
void					create_listener(t_server_state *s);
void					communicate(t_server_state *s);

#endif
