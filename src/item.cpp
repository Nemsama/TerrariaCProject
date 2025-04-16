#include <item.h>

// need a pre-initiated sprite
item_t* item_init( sprite_t* sprite, vector2_t position, char* name, bool is_stackable, int count, bool is_in_inventory ) {
    item_t* item = (item_t*)calloc( 1, sizeof(*item) );
    if ( item == NULL ) {
        perror("Failed to allocate memory for item structure");
        return NULL;
    }

    item->entity = entity_init( sprite, position );
    if ( item->entity == NULL ) {
        perror("Failed to create entity for item");
        return NULL;
    }

    strncpy( item->name, name, MAX_ITEM_NAME_LEN );
    item->is_stackable = is_stackable;
    item->count = count;
    item->is_in_inventory = is_in_inventory;

    return item;
}
void item_destroy( item_t* item ) {
    if ( item == NULL ) return;
    entity_destroy( item->entity );
    free( item );
}

// compare the type (name) of items, not count
bool item_is_same  ( item_t* item1, item_t* item2 ) {
    if ( item1 == NULL || item2 == NULL ) return false;

    return ( 0 == strcmp( item1->name, item2->name ) );
}
bool item_is_empty ( item_t* item ) {
    if ( item == NULL ) return true;

    return ( item->count <= 0 );
}
int  item_get_count( item_t* item ) {
    if ( item == NULL ) return 0;

    return item->count;
}

void item_add_one( item_t* item ) {
    if ( item == NULL ) return;

    item->count++;
}
void item_remove_one( item_t* item ) {
    if ( item == NULL ) return;

    item->count--;
}

// if same type, put the most of item2 possible inside item1
// return the remaining count of item2 or -1 if not the same type
int item_group( item_t* item1, item_t* item2 ) {
    if ( !item_is_same( item1, item2 ) ) return -1;

    while ( item1->count < MAX_ITEM_COUNT && item2->count > 0 ) {
        item_remove_one( item2 );
        item_add_one( item1 );
    }
    
    return item2->count;
}

// set the item as outside an inventory and apply 'speed' to its entity component
// if speed is set to 0, a random speed is applied
void item_throw( item_t* item, vector2_t speed ) {
    if ( item == NULL ) return;

    item->is_in_inventory = false;
    
    if ( !vector2_is_zero( speed ) ) {
        speed = vector2_random_on_unit_sphere();
        vector2_set_y( &speed, fabs( vector2_get_y( speed ) ) );
    }

    entity_impulse( item->entity, speed );
}
// set the item as inside an inventory
void item_pickup( item_t* item ) {
    if ( item ) item->is_in_inventory = true;
}
void item_update( item_t* item ) {
    if ( item == NULL ) return;

    entity_add_force( item->entity, vector2_new( 0.0f, GRAVITY ) );

    entity_apply_force( item->entity );
}

