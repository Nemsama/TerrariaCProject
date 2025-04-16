#ifndef ITEM_H
#define ITEM_H

#include <common.h>
#include <entity.h>
// dependancies : lower

#define MAX_ITEM_NAME_LEN 64
#define MAX_ITEM_COUNT 9999

typedef struct item {
    char name[MAX_ITEM_NAME_LEN];
    bool is_stackable;
    int count;

    bool is_in_inventory;
    entity_t* entity;
} item_t;

// need a pre-initiated sprite
item_t* item_init( sprite_t* sprite, vector2_t position, char* name, bool is_stackable, int count, bool is_in_inventory );
void item_destroy( item_t* item );

// compare the type (name) of items, not count
bool item_is_same  ( item_t* item1, item_t* item2 );
bool item_is_empty ( item_t* item );
int  item_get_count( item_t* item );

void item_add_one( item_t* item );
void item_remove_one( item_t* item );

// if same type, put the most of item2 possible inside item1
// return the remaining count of item2 or -1 if not the same type
int item_group( item_t* item1, item_t* item2 );

// set the item as outside an inventory and apply 'speed' to its entity component
// if speed is set to 0, a random speed is applied
void item_throw( item_t* item, vector2_t speed );
// set the item as inside an inventory
void item_pickup( item_t* item );
// if outside an inventory, apply gravity to the item
void item_update( item_t* item );

#endif