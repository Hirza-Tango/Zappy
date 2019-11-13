#ifndef ZAPPY_SERVER_H
# define ZAPPY_SERVER_H

#include <sys/queue.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAX(x,y) x > y ? x : y

enum	exit_codes
{
	INVALID_FLAG = 2,
	MISSING_OPTION,
	BAD_ADDR,
	BIND_FAIL,
	SELECT_FAIL,
	LISTEN_FAIL
};

typedef struct	s_player
{
	unsigned int	x;
	unsigned int	y;
	unsigned char	direction;
}				t_player;

typedef struct	s_server_state
{
	char			*port;
	int				fd_listen;
	unsigned char	current_players;
	unsigned char	allowed_players;
	int				max_fd;
	fd_set			fd_read;
}				t_server_state;

typedef struct	s_game_state
{
	t_list			**board;
	unsigned int	size_x;
	unsigned int	size_y;
	char			**teams;
	size_t			n_teams;
	size_t			time;
}				t_game_state;

void			exit_error(char *message, int code);
void			parse_args(int argc, char **argv,
	t_game_state *game_state, t_server_state *server_state);
void			create_listener(t_server_state *s);
void			communicate(t_server_state *s);

#endif
