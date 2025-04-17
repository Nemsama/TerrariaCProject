#include <item.h>

item_list_t loaded_items = item_list_new();

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

    strncpy( item->name, name, ITEM_MAX_NAME_LEN );
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

item_list_t item_list_new( void ) {
    return NULL;
}
bool item_list_is_empty( item_list_t item_list ) {
    return NULL == item_list;
}

item_list_t item_list_add( item_list_t item_list, item_t* item ) {
    item_list_t new_link = (item_list_t)calloc( 1, sizeof(*new_link) );
    if ( new_link == NULL ) {
        perror("Memory allocation for item list link failed");
        return NULL;
    }

    new_link->item = item;
    new_link->next = item_list;
    return new_link;
}
item_list_t item_list_add_new( item_list_t item_list, sprite_t* sprite, vector2_t position, char* name, bool is_stackable, int count, bool is_in_inventory ) {
    item_t* new_item = item_init( sprite, position, name, is_stackable, count, is_in_inventory );
    if ( new_item == NULL ) {
        perror("Item creation failed during item list incrementation");
        return item_list;
    }

    return item_list_add( item_list, new_item );
}
item_list_t item_list_remove( item_list_t item_list, item_t* item ) {
    if ( item_list_is_empty( item_list ) ) return item_list;

    // one link case
    if ( item_list_is_empty( item_list->next ) && ( item_list->item == item ) ) {
        free( item_list );
        return item_list_new();
    }

    item_list_t p;
    for ( p = item_list; !item_list_is_empty( p->next ); p = p->next ) {
        if ( p->next->item != item ) continue;

        item_list_t temp = p->next;
        p->next = p->next->next;
        free( temp );
        return item_list;
    }

    if ( p->item == item ) {
        free( p );
    }

    return item_list;
}
item_list_t item_list_grab_first_colliding( item_list_t item_list, rect_t rect, item_t** item ) {
    *item = NULL;
    if ( item_list_is_empty( item_list ) ) return item_list;

    // one link case
    if ( item_list_is_empty( item_list->next ) && rect_collision( &rect, &item_list->item->entity->world_rect ) ) {
        *item = item_list->item;
        free( item_list );
        return item_list_new();
    }

    item_list_t p;
    for ( p = item_list; !item_list_is_empty( p->next ); p = p->next ) {
        if ( !rect_collision( &rect, &p->item->entity->world_rect ) ) continue;

        *item = p->item;
        item_list_t temp = p->next;
        p->next = p->next->next;
        free( temp );
        return item_list;
    }

    if ( rect_collision( &rect, &p->item->entity->world_rect ) ) {
        *item = p->item;
        free( p );
    }

    return item_list;
}
item_list_t item_list_pop( item_list_t item_list ) {
    if ( item_list_is_empty( item_list ) ) return item_list;

    item_list_t temp = item_list->next;
    item_destroy( item_list->item );
    free( item_list );

    return temp;
}
item_list_t item_list_destroy( item_list_t item_list ) {
    while ( !item_list_is_empty( item_list ) ) {
        item_list = item_list_pop( item_list );
    }

    return item_list_new();
}
void item_list_updateall( item_list_t item_list, vector2_t player_pos, float force, float range2, bool print_debug ) {
    if ( print_debug ) printf("updating loaded items...\n");
    while ( !item_list_is_empty( item_list ) ) {
        if ( print_debug ) {
            printf("updating item "); puts(item_list->item->name);
            printf("count = %d ; inventory? : %d\n", item_list->item->count, item_list->item->is_in_inventory );
            printf("position : "); vector2_print( item_list->item->entity->world_rect.position ); puts("");
        }

        item_update( item_list->item, player_pos, force, range2 );
        item_list = item_list->next;
    }
}
void item_list_renderall( item_list_t item_list, camera_t* camera, SDL_Renderer* renderer ) {
    while ( !item_list_is_empty( item_list ) ) {
        item_render_scaled( item_list->item, camera, renderer );
        item_list = item_list->next;
    }
}

// compare the type (name) of items, not count
// note : return true if both items are ITEM_EMPTY_SLOT
bool item_is_same  ( item_t* item1, item_t* item2 ) {
    if ( item1 == ITEM_EMPTY_SLOT && item2 == ITEM_EMPTY_SLOT ) return true;
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
    if ( !item_is_same( item1, item2 ) || item1 == NULL || item2 == NULL ) return -1;

    while ( item1->count < ITEM_MAX_COUNT && item2->count > 0 ) {
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
void item_update( item_t* item, vector2_t player_pos, float force, float range2 ) {
    if ( item == NULL ) return;

    if ( range2 >= vector2_distance2( player_pos, entity_output_pos( item->entity ) ) ) entity_attract( item->entity, player_pos, force );

    entity_add_force( item->entity, vector2_new( 0.0f, GRAVITY ) );

    entity_apply_force( item->entity );
}

// only used for on world items : scale the sprite to match camera's zoom
void item_render_scaled( item_t* item, camera_t* camera, SDL_Renderer* renderer ) {
    if ( item == NULL ) return;
    entity_render( item->entity, camera, renderer );
}
// print the sprite on the renderer at the given position
void item_render       ( item_t* item, int x, int y, SDL_Renderer* renderer ) {
    if ( item == NULL ) return;

    item->entity->sprite->dest_rect.x = x;
    item->entity->sprite->dest_rect.y = y;
    sprite_render( item->entity->sprite, renderer );
}
// print the sprite on the renderer centered on the given position
void item_render_center( item_t* item, int x, int y, SDL_Renderer* renderer ) {
    if ( item == NULL ) return;

    item_render( item, x - item->entity->sprite->src_rect.w/2, y - item->entity->sprite->src_rect.h/2, renderer );
}
