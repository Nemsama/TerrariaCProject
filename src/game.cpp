#include <game.h>
#include <stdio.h>
#include <stdlib.h>

game_t* game_init( void ) {
    // Allocate memory for the game structure
    game_t* game = (game_t*)malloc( sizeof(*game) );
    if ( game == NULL ) {
        perror("Failed to allocate memory for game structure");
        exit( EXIT_FAILURE );
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        perror("Failed to initialize SDL");
        free(game);
        exit(EXIT_FAILURE);
    }

    // Create window
    game->window = SDL_CreateWindow( WINDOW_TITLE,
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WINDOW_WIDTH,
                                     WINDOW_HEIGHT,
                                     SDL_WINDOW_SHOWN );
    if ( game->window == NULL ) {
        perror("Failed to create window");
        SDL_Quit();
        free(game);
        exit( EXIT_FAILURE );
    }

    // Create renderer
    game->renderer = SDL_CreateRenderer( game->window, -1, SDL_RENDERER_ACCELERATED );
    if ( game->renderer == NULL ) {
        perror("Failed to create renderer");
        SDL_DestroyWindow( game->window );
        SDL_Quit();
        free(game);
        exit( EXIT_FAILURE );
    }

    // Load background sprite
    game->background = sprite_init( "assets/background/Blue_sky.png", game->renderer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
    sprite_set_position( game->background, 0, 0 );

    // Set the initial state of the game
    game->running = true;

    return game;
}

void game_quit( game_t* game ) {
    // Set the running state to false
    game->running = false;

    // Free the background sprite
    sprite_free( game->background );

    // Destroy renderer and window
    SDL_DestroyRenderer( game->renderer );
    SDL_DestroyWindow( game->window );

    // Quit SDL
    SDL_Quit();

    // Free the game structure
    free(game);
}


void game_start( game_t* game ) {
    SDL_RenderClear( game->renderer );
}

void game_handle_events( game_t* game ) {
    while ( SDL_PollEvent( &game->event ) ) {
        if ( game->event.type == SDL_QUIT ) {
            game->running = false;
        }
    }
}

void game_update( game_t* game ) {
    // Update game logic here
}

void game_render( game_t* game ) {
    // Clear the screen
    SDL_SetRenderDrawColor( game->renderer, 0, 0, 0, 255 ); // black background
    SDL_RenderClear( game->renderer );

    // Render the background sprite
    sprite_render( game->background, game->renderer );

    // Present the back buffer
    SDL_RenderPresent( game->renderer );
}

void game_run( game_t* game ) {
    game_start( game );

    while ( game->running ) {
        game_handle_events( game );

        game_update( game );

        game_render( game );
    }
}
