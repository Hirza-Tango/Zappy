#include "zappy_server.h"

int		main(int argc, char **argv){
	t_game_state	game_state;
	t_server_state	server_state;

	parse_args(argc, argv, &game_state, &server_state);
	//generate gameboard
	create_listener(&server_state);
	server_state.max_fd = server_state.fd_listen;
	while (1){
		communicate(&server_state);
		//handle message instructions
		//handle game events
	}
}
