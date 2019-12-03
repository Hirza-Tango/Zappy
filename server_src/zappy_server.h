/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_server.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 18:42:42 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 17:03:48 by dslogrov         ###   ########.fr       */
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
# include <time.h>
# include "../mini_libft/libft.h"
# include "ring_buffer.h"

# define MAX(x,y) x > y ? x : y
# define RAND(x) (rand() % x)
# define STRBUFF_SIZE 256

enum					e_client_type
{
	NOT_CLIENT,
	UNKNOWN,
	SERVER,
	MONITOR,
	PLAYER
};

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

typedef struct			s_resource_entry{
	char	*name;
	int		team_requirement;
	int		level_requirements[NUM_LEVELS];
}						t_resource_entry;

/*
**	Below is a lookup table of all information required for resources
**	Fuck lookup tables though
*/
static const t_resource_entry	g_resources_info[NUM_RESOURCES] = {
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
	size_t			player_no;
	size_t			team_no;
	void			(*next_action)();
	time_t			resolution_time;
	unsigned int	level;
	unsigned int	inventory[NUM_RESOURCES];
	void			*option;
	unsigned char	direction;
}						t_player;

typedef struct			s_team
{
	char			*name;
	unsigned int	nb_client;
}						t_team;

typedef struct			s_client
{
	enum e_client_type	type;
	t_player			*player;
	t_cbuff				buf_read;
}						t_client;

typedef struct			s_egg
{
	unsigned int	x;
	unsigned int	y;
	size_t			player_no;
	unsigned int	egg_no;
	unsigned int	team_no;
	time_t			spawn_time;
}						t_egg;

typedef struct			s_state
{
	unsigned int	***board;
	char			*port;
	t_client		*clients;
	size_t			max_clients;
	unsigned char	allowed_players;
	int				max_fd;
	fd_set			fd_read;
	t_team			*teams;
	size_t			n_teams;
	size_t			n_players;
	t_list			*eggs;
	size_t			n_eggs;
	unsigned int	size_x;
	unsigned int	size_y;
	size_t			time;
}						t_state;

void					exit_error(char *message, int code);
void					parse_args(int argc, char **argv, t_state *state);
void					create_listener(t_state *s);
void					communicate(t_state *s);
void					gen_board(t_state *s);
void					client_read(t_state *s, int fd);
void					handle(t_state *s);
void					execute(t_state *s);

t_player				*new_player(t_state *s, int fd, char *buff, t_egg *egg);
void					set_action(t_player *player,
	void (f)(t_state *, int, void *), time_t t, void *option);
void					player_advance(t_state *s, int fd, void *unused);
void					player_left(t_state *s, int fd, void *unused);
void					player_right(t_state *s, int fd, void *unused);
void					player_see(t_state *s, int fd, void *unused);
void					player_inventory(t_state *s, int fd, void *unused);
void					player_take(t_state *s, int fd, void *num);
void					player_put(t_state *s, int fd, void *num);
void					player_incantation_start(t_state *s, int fd);
void					player_incantation_end(t_state *s, int fd, void *unused);
void					player_kick(t_state *s, int fd, void *unused);
void					player_fork(t_state *s, int fd, void *unused);
void					player_broadcast(t_state *s, int fd, void *message);
void					player_connect_nbr(t_state *s, int fd);

void					init_monitor(t_state *s, int fd);
void					send_all_monitors(t_state *s, char *buffer);
void					monitor_seg(t_state *s, char *team);
void					monitor_msz(t_state *s, int fd);
void					monitor_bct(t_state *s, int fd,
	unsigned int x, unsigned int y);
void					monitor_mct(t_state *s, int fd);
void					monitor_tna(t_state *s, int fd);
void					monitor_sgt(t_state *s, int fd);
void					monitor_sst(t_state *s, int fd, int time);
void					monitor_pnw(t_state *s, int fd, t_player *player);
void					monitor_enw(t_state *s, int fd, t_egg *egg);
void					monitor_ppo(t_state *s, int fd, unsigned int player_no);
void					monitor_plv(t_state *s, int fd, unsigned int player_no);
void					monitor_pin(t_state *s, int fd, unsigned int player_no);
void					monitor_pex(t_state *s, t_player *p);
void					monitor_pbc(t_state *s, t_player *p, char *message);
void					monitor_pic(t_state *s, t_player *player1);
void					monitor_pie(t_state *s,
	unsigned int x, unsigned int y, int r);
void					monitor_pfk(t_state *s, t_player *p);
void					monitor_pdr(t_state *s, t_player *p, int i);
void					monitor_pgt(t_state *s, t_player *p, int i);
void					monitor_pdi(t_state *s, t_player *p);
void					monitor_eht(t_state *s, t_egg *e);
void					monitor_edi(t_state *s, t_egg *e);

#endif
