#include <SDL2/SDL.h>
#include <stdio.h>

#include <game.h>

int main() {
    game_t* game = game_init();
    if ( NULL == game ) {
        perror("Error during game initialisation");
        exit( EXIT_FAILURE );
    }
    
    game_run( game );
    
    game_quit( game );
    
    return 0;
}
