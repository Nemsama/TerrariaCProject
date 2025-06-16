#include <background.h>

// assets placeholders
sprite_t*  forest_trees = NULL;
sprite_t*  forest_background = NULL;
sprite_t*  desert_dunes = NULL;
sprite_t* toundra_trees = NULL;
sprite_t*  jungle_trees = NULL;
sprite_t*   beach_beach = NULL;

sprite_t*          sun = NULL;
sprite_t*         moon = NULL;
sprite_t* all_clouds[15] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
sprite_t**  sunny_cloud = all_clouds;
sprite_t** medium_cloud = all_clouds + 5;
sprite_t**  rainy_cloud = all_clouds + 10;
sprite_t*         rain = NULL;
sprite_t*         snow = NULL;
// sprite_t* fog = NULL;

// cloud list to hold all the clouds
cloud_list_t cloud_list = NULL;
cloud_list_t cloud_list_add( cloud_list_t list, sprite_t* cloud ) {
    // Add a new cloud to the list
    cloud_list_t new_node = (cloud_list_t)calloc( 1, sizeof(*new_node) );
    if ( new_node == NULL ) {
        perror("Failed to allocate memory for cloud list node");
        return list;
    }
    new_node->cloud = cloud;
    new_node->speed = -0.2f;
    new_node->mov_buffer = 0.0f; 
    new_node->next = list;
    return new_node;
}
void cloud_list_destroy( cloud_list_t list ) {
    // Free all the clouds in the list
    cloud_list_t current = list;
    while ( current != NULL ) {
        cloud_list_t to_free = current;
        current = current->next;
        sprite_destroy( to_free->cloud, KEEP_TEXTURE ); // does not destroy the textures, the background_destroy function will do it
        free(to_free);
    }
}

// sun course data
int sun_course_x[SUN_COURSE_SIZE];
int sun_course_y[SUN_COURSE_SIZE];

bool init_sun_course( void ) {
    int file_size = 0;
    float x;
    float y;

    FILE* file = fopen( "assets/data/suncourse.dat", "r" );
    if ( file == NULL ) {
        perror("Failed to open sun course file");
        return false;
    }
    fscanf( file, "%d ", &file_size );
    if ( file_size != SUN_COURSE_SIZE ) {
        perror("Invalid sun course file size");
        fclose(file);
        return false;
    }
    for ( int i = 0; i < SUN_COURSE_SIZE; i++ ) {
        if ( fscanf( file, "%f %f", &x, &y ) != 2 ) {
            perror("Failed to read sun course data");
            fclose(file);
            return false;
        }
        sun_course_x[i] = (int)roundf(x);
        sun_course_y[i] = (int)roundf(y);
    }
    fclose(file);
    return true;
}

// free memory used by the background
void background_destroy( void ) {
    // free all the clouds
    cloud_list_destroy( cloud_list );

    // free all the background assets
    sprite_destroy( forest_trees, DESTROY_TEXTURE );
    sprite_destroy( forest_background, DESTROY_TEXTURE );
    sprite_destroy( desert_dunes, DESTROY_TEXTURE );
    sprite_destroy( toundra_trees, DESTROY_TEXTURE );
    sprite_destroy( jungle_trees, DESTROY_TEXTURE );
    sprite_destroy( beach_beach, DESTROY_TEXTURE );

    sprite_destroy( sun,  DESTROY_TEXTURE );
    sprite_destroy( moon, DESTROY_TEXTURE );
    for ( int i = 0; i < 5; i++ ) {
        if (  sunny_cloud ) sprite_destroy(  sunny_cloud[i], DESTROY_TEXTURE );
        if ( medium_cloud ) sprite_destroy( medium_cloud[i], DESTROY_TEXTURE );
        if (  rainy_cloud ) sprite_destroy(  rainy_cloud[i], DESTROY_TEXTURE );
    }
    sprite_destroy( rain, DESTROY_TEXTURE );
    sprite_destroy( snow, DESTROY_TEXTURE );
    // sprite_destroy( fog, DESTROY_TEXTURE );
}

// load all the background assets
// returns true if all the assets are loaded properly
// return false if an error occured during background initialisation
bool background_init( SDL_Renderer* renderer ) {
    srand( (unsigned int)time(NULL) );

    // Load the background assets
    forest_trees = sprite_init( "assets/background/forest_trees.png",  renderer, 0, 0, 1.0f, 1280, 720 );
    forest_background = sprite_init( "assets/background/forest_background.png", renderer, 0, 0, 1.0f, 1280, 720 );
    //  desert_dunes = sprite_init( "assets/background/desert_dunes.png",  renderer, 0, 0, 1.0f, 1280, 720 );
    // toundra_trees = sprite_init( "assets/background/toundra_trees.png", renderer, 0, 0, 1.0f, 1280, 720 );
    //  jungle_trees = sprite_init( "assets/background/jungle_trees.png",  renderer, 0, 0, 1.0f, 1280, 720 );
    //   beach_beach = sprite_init( "assets/background/beach_beach.png",   renderer, 0, 0, 1.0f, 1280, 720 );
    //

    sun         = sprite_init( "assets/background/sun.png",         renderer, 0, 0, 1.0f,   64,   64 );
    // moon        = sprite_init( "assets/background/moon.png",        renderer, 0, 0, 1.0f,   64,   64 );

    const char* sunny_cloud_path = "assets/background/sunny_cloud";
    const char* medium_cloud_path = "assets/background/medium_cloud";
    const char* rainy_cloud_path = "assets/background/rainy_cloud";
    char  cloud_path[256];
    for ( int i = 0; i < 5; i++ ) {
        sprintf( cloud_path, "%s_%d.png", sunny_cloud_path, i );
        sunny_cloud[i] = sprite_init( cloud_path, renderer, 0, 0, 1.0f, 512, 300 );
        // sprintf( cloud_path, "%s_%d.png", medium_cloud_path, i );
        // medium_cloud[i] = sprite_init( cloud_path, renderer, 0, 0, 1.0f, 512, 512 );
        // sprintf( cloud_path, "%s_%d.png", rainy_cloud_path, i );
        // rainy_cloud[i] = sprite_init( cloud_path, renderer, 0, 0, 1.0f, 512, 512 );

        if ( !sunny_cloud[i] /*|| !rainy_cloud[i] || !medium_cloud[i]*/ ) {
            perror("Failed to load clouds assets");
            background_destroy();
            return false;
        }
    }

    // rain        = sprite_init( "assets/background/rain.png",        renderer, 0, 0, 1.0f, 1500, 9700 );
    // snow        = sprite_init( "assets/background/snow.png",        renderer, 0, 0, 1.0f, 1500, 9700 );
    // fog         = sprite_init( "assets/background/fog.png",         renderer, 0, 0, 1.0f,   512 ,   512 );

    // Check if all assets are loaded successfully
    if ( !forest_trees || !forest_background /*|| !desert_dunes || !toundra_trees || !jungle_trees || !beach_beach*/ ||
         !sun /*|| !rain || !snow*/ /*|| !fog*/ ) {
        perror("Failed to load background assets");
        background_destroy();
        return false;
    }

    if ( !init_sun_course() ) {
        background_destroy();
        return false;
    }

    return true;
}


// Set the background color (or starsky) based on the biome and time of day
void set_bg_color( bg_biome_t biome, int time_of_day, SDL_Renderer* renderer ) {
    if ( 1900 <= time_of_day || time_of_day < 700 ) {
        // Set the background color to black for night
        SDL_SetRenderDrawColor( renderer,   0,  39,  77, 255 );
        return;
    }
    // OR Set the background color based on the biome
    switch ( biome ) {
        case FOREST:
            // SDL_SetRenderDrawColor( renderer,  47, 194, 237, 255 ); // light blue for forest
            sprite_render( forest_background, renderer, false );
            break;
        case DESERT:
            SDL_SetRenderDrawColor( renderer, 255, 248, 213, 255 ); // Orange for desert
            break;
        case TOUNDRA:
            SDL_SetRenderDrawColor( renderer, 213, 239, 255, 255 ); // White for tundra
            break;
        case JUNGLE:
            SDL_SetRenderDrawColor( renderer, 107, 244, 225, 255 ); // light light blue for jungle
            break;
        case BEACH:
            SDL_SetRenderDrawColor( renderer, 107, 244, 255, 255 ); // Light light blue for beach
            break;
    }
}

// Render the background based on the biome
void render_bg_biome( bg_biome_t biome, SDL_Renderer* renderer ) {
    // Render the background based on the biome
    switch ( biome ) {
        case FOREST:
            sprite_render( forest_trees, renderer, false );
            break;
        case DESERT:
            sprite_render( desert_dunes, renderer, false );
            break;
        case TOUNDRA:
            sprite_render( toundra_trees, renderer, false );
            break;
        case JUNGLE:
            sprite_render( jungle_trees, renderer, false );
            break;
        case BEACH:
            sprite_render( beach_beach, renderer, false );
            break;
    }
}

// Render the sun (or moon) based on the time of day
int get_sun_x( int time_of_day ) {
    float day_progress = (time_of_day - 600) / 1200.f; // 700 to 1900
    return sun_course_x[(int)(day_progress * SUN_COURSE_SIZE)];
}
int get_sun_y( int time_of_day ) {
    float day_progress = (time_of_day - 600) / 1200.f; // 700 to 1900
    return sun_course_y[(int)(day_progress * SUN_COURSE_SIZE)];
}
int get_moon_x( int time_of_day ) {
    float night_progress = (time_of_day - 1800); // 1800 to 600
    if ( night_progress < 0 ) {
        night_progress += 1200;
    }
    night_progress /= 1200.f; // 0 to 1
    return sun_course_x[(int)(night_progress * SUN_COURSE_SIZE)];
}
int get_moon_y( int time_of_day ) {
    float night_progress = (time_of_day - 1800); // 1800 to 600
    if ( night_progress < 0 ) {
        night_progress += 1200;
    }
    night_progress /= 1200.f; // 0 to 1
    return sun_course_y[(int)(night_progress * SUN_COURSE_SIZE)];
}   
void render_sun( int time_of_day, SDL_Renderer* renderer ) {
    int x = get_sun_x( time_of_day );
    int y = get_sun_y( time_of_day );
    // Render the sun or moon based on the time of day
    if ( 1900 <= time_of_day || time_of_day < 700 ) {
        sprite_set_center( moon, x, y );
        sprite_render( moon, renderer, false );
    }
    else {
        sprite_set_center( sun, x, y );
        sprite_render( sun, renderer, false );
    }
}

// Render the clouds based on the weather

// add a new cloud at the given position
// the sprite_index is used to select the cloud type in the all_clouds array !!!
void spawn_cloud( int x, int y, int sprite_index ) {
    // Render a cloud at the given position
    sprite_t* cloud = sprite_copy( all_clouds[sprite_index] );
    sprite_set_center( cloud, x, y );

    cloud_list = cloud_list_add( cloud_list, cloud );
}
void spawn_sunny_cloud( void ) {
    // Spawn a sunny cloud at a random position
    int x = WINDOW_WIDTH + rand() % 300; // spawn off screen to the right
    int y = rand() % (WINDOW_HEIGHT / 3); // spawn in the upper third of the screen
    spawn_cloud( x, y, rand() % 5 ); // random sunny cloud type
}
void spawn_medium_cloud( void ) {
    // Spawn a medium cloud at a random position
    int x = WINDOW_WIDTH + rand() % 300; // spawn off screen to the right
    int y = rand() % (WINDOW_HEIGHT / 3); // spawn in the upper third of the screen
    spawn_cloud( x, y, 5 + rand() % 5 ); // random medium cloud type
}
void spawn_rainy_cloud( void ) {
    // Spawn a rainy cloud at a random position
    int x = WINDOW_WIDTH + rand() % 300; // spawn off screen to the right
    int y = rand() % (WINDOW_HEIGHT / 3); // spawn in the upper third of the screen
    spawn_cloud( x, y, 10 + rand() % 5 ); // random rainy cloud type
}

void move_cloud( cloud_link* cloud ) {
    float movement = cloud->mov_buffer + cloud->speed;
    cloud->mov_buffer = movement - (int)movement;
    cloud->cloud->dest_rect.x += (int)movement;
}

void render_clouds( weather_t weather, SDL_Renderer* renderer ) {
    // Update clouds position and style based on the weather
    cloud_list_t current = cloud_list;
    cloud_list_t previous = NULL;
    int cloud_count = 0;
    while ( current != NULL ) {
        // Render the cloud
        sprite_render( current->cloud, renderer, false );

        move_cloud( current );
        // Remove the cloud if it goes off screen
        if ( current->cloud->dest_rect.x + current->cloud->dest_rect.w < 0 ) {
            cloud_list_t to_free = current;
            current = current->next;
            sprite_destroy( to_free->cloud, KEEP_TEXTURE );
            free(to_free);
            if ( previous == NULL ) {
                cloud_list = current; // Remove the first cloud
            }
            else {
                previous->next = current; // Remove the current cloud
            }
        }
        else {
            previous = current;
            current = current->next;
            cloud_count++;
        }
    }

    // Spawn new clouds based on the weather
    switch ( weather ) {
        case SUNNY:
            if ( cloud_count < 5 ) spawn_sunny_cloud();
            break;
        case CLOUDY:
            if ( cloud_count < 8 ) spawn_medium_cloud();
            break;
        case SNOWY:
        case RAINY:
            if ( cloud_count < 12 ) spawn_rainy_cloud();
            break;
        default:
            break;
    }
}
void reset_clouds( weather_t weather ) {
    // Reset the cloud list based on the weather
    cloud_list_destroy( cloud_list );
    cloud_list = NULL;

    int x, y;
    // Spawn initial clouds based on the weather
    switch ( weather ) {
        case SUNNY:
            for ( int i = 0; i < 5; i++ ) {
                x = rand() % WINDOW_WIDTH; // spawn randomly on all the screen
                y = rand() % (WINDOW_HEIGHT / 3); // spawn in the upper third of the screen
                spawn_cloud( x, y, rand() % 5 ); // random sunny cloud type
            }
            break;
        case CLOUDY:
            for ( int i = 0; i < 8; i++ ) {
                x = rand() % WINDOW_WIDTH; // spawn randomly on all the screen
                y = rand() % (WINDOW_HEIGHT / 3); // spawn in the upper third of the screen
                spawn_cloud( x, y, 5 + rand() % 5 ); // random medium cloud type
            }
            break;
        case SNOWY:
        case RAINY:
            for ( int i = 0; i < 12; i++ ) {
                x = rand() % WINDOW_WIDTH; // spawn randomly on all the screen
                y = rand() % (WINDOW_HEIGHT / 3); // spawn in the upper third of the screen
                spawn_cloud( x, y, 10 + rand() % 5 ); // random rainy cloud type
            }
            break;
        default:
            break;
    }
}

// Render the rain or snow based on the weather
void render_rain( weather_t weather, SDL_Renderer* renderer ) {
    // Render the rain or snow based on the weather
    if ( weather == RAINY ) {
        rain->dest_rect.y += 10; // Move the rain down
        if ( rain->dest_rect.y > 0 ) {
            // Reset the rain position to the top of mount everest
            rain->dest_rect.y = -8848;
        }
        // Custom rendering to rotate the rain texture
        SDL_Point center = { WINDOW_WIDTH/2 - rain->dest_rect.x, WINDOW_HEIGHT/2 - rain->dest_rect.h };
        SDL_RenderCopyEx( renderer, rain->texture, &rain->src_rect, &rain->dest_rect, 15.0, &center, SDL_FLIP_NONE );
    }
    else if ( weather == SNOWY ) {
        snow->dest_rect.y += 10; // Move the snow down
        if ( snow->dest_rect.y > 0 ) {
            // Reset the snow position to the top of mount everest
            snow->dest_rect.y = -8848;
        }
        // Custom rendering to rotate the snow texture
        SDL_Point center = { WINDOW_WIDTH/2 - snow->dest_rect.x, WINDOW_HEIGHT/2 - snow->dest_rect.h };
        SDL_RenderCopyEx( renderer, snow->texture, &snow->src_rect, &snow->dest_rect, 15.0, &center, SDL_FLIP_NONE );
    }
}

void background_render( bg_biome_t biome, weather_t weather, int time_of_day, SDL_Renderer* renderer ) {
    // Set the background color (or starsky) based on the biome and time of day
    set_bg_color( biome, time_of_day, renderer );
    // Render the background based on the biome
    render_bg_biome( biome, renderer );
    // Render the sun (or moon) based on the time of day
    render_sun( time_of_day, renderer );
    // Update clouds position and style and Render them based on the weather
    render_clouds( weather, renderer );
    // Render the rain or snow based on the weather
    render_rain( weather, renderer );
}
