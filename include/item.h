#ifndef ITEM_H
#define ITEM_H

#include <common.h>
#include <entity.h>
// dependancies : lower

#define ITEM_MAX_NAME_LEN 64
#define ITEM_MAX_COUNT 9999
#define ITEM_EMPTY_SLOT NULL
#define NO_EXCEPTION NULL

#define ITEM_RENDER_DISTANCE2 20.0f*20.0f
#define ITEM_GROUPING_RANGE2 1.0f*1.0f

#define BLOCK_ITEM_SCALE 0.5f

typedef struct item {
    int id;
    char name[ITEM_MAX_NAME_LEN];
    bool is_stackable;
    int count;

    bool is_in_inventory;
    entity_t* entity;
} item_t;

typedef struct item_link {
    item_t* item;
    struct item_link * next;
} item_link_t, *item_list_t;

extern item_list_t loaded_items;

// need a pre-initiated sprite
item_t* item_init( sprite_t* sprite, vector2_t position, char* name, bool is_stackable, int count, bool is_in_inventory );
SDL_Texture* item_destroy( item_t* item, bool destroy_texture );

item_list_t item_list_new( void );
bool        item_list_is_empty( item_list_t item_list );
item_list_t item_list_destroy( item_list_t item_list );
item_list_t item_list_add( item_list_t item_list, item_t* item );
item_list_t item_list_add_new( item_list_t item_list, sprite_t* sprite, vector2_t position, char* name, bool is_stackable, int count, bool is_in_inventory );
item_list_t item_list_remove( item_list_t item_list, item_t* item );
// remove the item from the list AND destroys it
item_list_t item_list_destroy_item( item_list_t item_list, item_t* item, bool destroy_texture );
item_list_t item_list_updateall( item_list_t item_list, vector2_t player_pos, float force, float range2, world_t* world );
void item_list_renderall( item_list_t item_list, camera_t* camera, SDL_Renderer* renderer );
void item_list_print( item_list_t item_list );

item_list_t item_list_grab_first_colliding( item_list_t item_list, rect_t rect, item_t** item, item_t* exception );
bool item_list_grab_first_verifying( const item_list_t item_list, bool condition(item_t*, entity_t*), entity_t* argument, item_t** item, item_t* exception );

// compare the type (name) of items, not count
bool item_is_same  ( item_t* item1, item_t* item2 );
bool item_is_empty ( item_t* item );
int  item_get_count( item_t* item );

void item_add_one( item_t* item );
void item_remove_one( item_t* item );

item_t* item_copy( item_t* src );

// if same type, put the most of item2 possible inside item1
// return the remaining count of item2 or -1 if not the same type
int item_group( item_t* item1, item_t* item2 );

// set the item as outside an inventory and apply 'speed' to its entity component
// if speed is set to 0, a random speed is applied
void item_throw( item_t* item, vector2_t speed );
// set the item as inside an inventory
void item_pickup( item_t* item );

bool item_is_in_grouping_range( item_t* item, entity_t* entity );
bool item_is_in_pickup_range( item_t* item, entity_t* entity );

// if outside an inventory, apply gravity to the item
// if the item is out of render distance, destroy the item and return false
// else return true
bool item_update( item_t* item, item_list_t* pitem_list, vector2_t player_pos, float force, float range2, world_t* world );

// only used for on world items : scale the sprite to match camera's zoom
void item_render_scaled( item_t* item, camera_t* camera, SDL_Renderer* renderer );
// print the sprite on the renderer at the given position
void item_render       ( item_t* item, int x, int y, SDL_Renderer* renderer );
// print the sprite on the renderer centered on the given position
void item_render_center( item_t* item, int x, int y, SDL_Renderer* renderer );

void item_print( item_t* item );

#endif