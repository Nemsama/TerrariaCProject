#include <game.h>

game_t* game_init( void ) {
    game_t* game = (game_t*)calloc( 1, sizeof(*game) );
    if ( game == NULL ) {
        perror("Failed to allocate memory for game structure");
        return NULL;
    }

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        perror("Failed to initialize SDL");
        game_quit( game );
        return NULL;
    }

    game->window = SDL_CreateWindow( WINDOW_TITLE,
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WINDOW_WIDTH,
                                     WINDOW_HEIGHT,
                                     SDL_WINDOW_SHOWN );
    if ( game->window == NULL ) {
        perror("Failed to create window");
        game_quit( game );
        return NULL;
    }

    game->renderer = SDL_CreateRenderer( game->window, -1, SDL_RENDERER_ACCELERATED );
    if ( game->renderer == NULL ) {
        perror("Failed to create renderer");
        game_quit( game );
        return NULL;
    }

    game->background = sprite_init( "assets/background/Blue_sky.png", game->renderer, 0, 0, 2048, 1024 );
    if ( game->background == NULL ) {
        perror("Failed to create background sprite");
        game_quit( game );
        return NULL;
    }
    sprite_set_scale( game->background, WINDOW_WIDTH, WINDOW_HEIGHT );
    sprite_set_pos( game->background, 0, 0 );

    sprite_t* player_sprite = sprite_init( "assets/sprites/Guide_idle_clean.png", game->renderer, 0, 0, 48, 82 );
    game->player = (player_t*)entity_init( player_sprite, 
                                          vector2_new(WORLD_SPAWN_X, WORLD_SPAWN_Y),
                                          PLAYER_MASS,
                                          PLAYER_ACCELERATION,
                                          PLAYER_MAX_SPEED,
                                          PLAYER_JUMP_SPEED );
    if ( game->player == NULL ) {
        perror("Failed to create player");
        game_quit( game );
        return NULL;
    }

    game->camera = camera_init( vector2_zero(), CAMERA_SCALE, CAMERA_SPEED );
    if ( game->camera == NULL ) {
        perror("Failed to create camera");
        game_quit( game );
        return NULL;
    }
    camera_set_pos_center( game->camera, vector2_new(WORLD_SPAWN_X, WORLD_SPAWN_Y) );

    game->world = world_init();
    if ( game->world == NULL ) {
        perror("Failed to create world");
        game_quit( game );
        return NULL;
    }

    // Set the initial state of the game
    game->running = true;
    game->frame_count = 0;
    game->current_time = 0;
    game->frame_time = 0.0;

    return game;
}

void game_quit( game_t* game ) {
    if ( NULL == game ) return;

    game->running = false;

    if ( NULL != game->world ) world_destroy( game->world );
    destroy_blocks_types();
    if ( NULL != game->camera ) camera_destroy( game->camera );

    if ( NULL != game->player ) entity_destroy( game->player );

    if ( NULL != game->background ) sprite_destroy( game->background );

    if ( NULL != game->renderer ) SDL_DestroyRenderer( game->renderer );
    if ( NULL != game->window ) SDL_DestroyWindow( game->window );

    SDL_Quit();

    free(game);
}


void game_start( game_t* game ) {
    SDL_RenderClear( game->renderer );

    // Load blocks textures
    if ( !init_blocks_types( game->renderer ) ) {
        perror("Failed to load blocks textures");
        game_quit( game );
        return;
    }

    game->frame_count = 0;
    game->current_time = clock();
}

void game_handle_keydown( game_t* game, SDL_KeyboardEvent* key ) {

    switch ( key->keysym.sym ) {
        case SDLK_EXCLAIM:
            // Show debug info
            vector2_t player_pos;
            vector2_t camera_pos;
            int mouse_screen_x, mouse_screen_y;
            vector2_t mouse_world_pos;

            camera_get_pos_center( game->camera, &camera_pos );
            entity_get_pos( game->player, &player_pos );
            SDL_GetMouseState( &mouse_screen_x, &mouse_screen_y );
            camera_screentoworld_pos( game->camera, &mouse_world_pos, mouse_screen_x, mouse_screen_y );

            printf("frame count       : %d\n", game->frame_count);
            printf("frame time        : %lf ms\n", game->frame_time);
            printf("player world pos  : "); vector2_print( player_pos );           printf("\n");
            printf("camera center pos : "); vector2_print( camera_pos );           printf("\n");
            printf("mouse screen pos  : (%d, %d)\n", mouse_screen_x, mouse_screen_y);
            printf("mouse world pos   : "); vector2_print( mouse_world_pos );      printf("\n");

            puts("");
            break;
        default:
            break;
    }
}
void game_handle_events( game_t* game ) {
    while ( SDL_PollEvent( &game->event ) ) {
        switch ( game->event.type ) {
            case SDL_QUIT:
                game->running = false;
                break;
            case SDL_KEYDOWN:
                game_handle_keydown( game, &game->event.key );
                break;
            default:
                break;
        }
    }
}

void game_update( game_t* game ) {
    const Uint8* keystate = SDL_GetKeyboardState( NULL );

    player_update( game->player, keystate, game->world );

    camera_update( game->camera, entity_output_pos( game->player ), keystate );
}

void game_render( game_t* game ) {
    // Clear the screen
    SDL_SetRenderDrawColor( game->renderer, 0, 0, 0, 255 ); // black background
    SDL_RenderClear( game->renderer );

    // Render the background
    sprite_render( game->background, game->renderer );

    // Render blocks
    world_render( game->world, game->camera, game->renderer );

    // Render the player
    entity_render( game->player, game->camera, game->renderer );

    // Present the back buffer
    SDL_RenderPresent( game->renderer );
}

void game_delay( game_t* game ) {
    game->frame_time = ( clock() - game->current_time ) * 1000.0 / CLOCKS_PER_SEC;

    if ( game->frame_time < FRAME_TIME ) {
        SDL_Delay( FRAME_TIME - game->frame_time );
    }
    else {
        printf("Frame time exceeded : %lf ms", game->frame_time); puts("");
    }

    // printf("press enter to go to next frame\n");
    // scanf("%*c");

    game->current_time = clock();
}

void game_run( game_t* game ) {
    game_start( game );

    while ( game->running ) {
        game->frame_count++;

        game_handle_events( game );

        game_update( game );

        game_render( game );

        game_delay( game );
    }
}
