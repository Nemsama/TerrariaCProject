#ifndef BACKG_H
#define BACKG_H

#include <sprite.h>
#include <camera.h>

#define SUN_COURSE_SIZE 1024

typedef enum weather {
    SUNNY,
    CLOUDY,
    RAINY,
    SNOWY,
    FOGGY,
    WINDY
} weather_t;
typedef enum biome {
    FOREST,
    DESERT,
    TOUNDRA,
    JUNGLE,
    BEACH
} background_biome_t, bg_biome_t;
/* List of needed assets : 
1. Background color (can be done with a simple SDL_SetRenderDrawColor)
2. forest_trees
3. desert_dunes
4. toundra_trees
5. jungle_trees
6. beach_beach
7. sun
8. moon
9. sunny_cloud (5 variants)
(10. moderately_dark_cloud (5 variants))
11. rainy_cloud (5 variants)
12. rain (maybe variations)
13. snow (maybe variations)
(14. fog)
*/

/* 
typedef struct background_status {
    background_biome_t biome;
    weather_t weather;
    Uint8 time_of_day; // 0-2400
} background_status_t, bg_status_t;

bg_status_t get_background_status( bg_biome_t biome, weather_t weather, Uint8 time_of_day ); */

typedef struct cloud_link {
    sprite_t* cloud;
    float speed; // speed of the cloud in pixels per frame
    float mov_buffer;
    struct cloud_link * next;
} cloud_link_t, *cloud_list_t;


// scale the background sprite if the camera is zoomed in
/* void background_scale( sprite_t* background, camera_t* camera ); */
// render the background according to the given status
void background_render( bg_biome_t biome, weather_t weather, int time_of_day, SDL_Renderer* renderer );

void reset_clouds( weather_t weather );

// (for now) loads all the background assets
// returns true if all the assets are loaded properly
// return false if an error occured during background initialisation
bool background_init( SDL_Renderer* renderer );
// free memory used by the background
void background_destroy( void );

#endif