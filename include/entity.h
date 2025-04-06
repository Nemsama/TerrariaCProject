#ifndef ENTITY_H
#define ENTITY_H

#include <game.h>
#include <sprite.h>
#include <camera.h>

class Entity {
public :
    sprite_t* sprite;
    float x, y;       // world position
    float speed;      // world speed

    int health;

    Entity( sprite_t* sprite, float x, float, y, float speed, int health ) : sprite(sprite), x(x), y(y), speed(speed), health(health) {}

    virtual ~Entity() {
        if ( sprite ) {
            sprite_destroy( sprite );
        }
    }

    virtual void update() {
        // nothing to update for base entity
    }
    
    virtual void render( camera_t* camera, SDL_Renderer* renderer ) {
        int screen_x, screen_y;

        if ( sprite ) {
            camera_worldtoscreen_pos( camera, x, y, &screen_x, &screen_y );
            sprite_set_position( sprite, screen_x, screen_y );
            // sprite_scale( sprite, 1/camera->scale, 1/camera->scale );
            sprite_render( sprite, renderer );
        }
    }
}

#endif