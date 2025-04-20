#include <item.h>

item_list_t loaded_items = item_list_new();

// need a pre-initiated sprite
item_t* item_init( sprite_t* sprite, vector2_t position, char* name, bool is_stackable, int count, bool is_in_inventory ) {
    item_t* item = (item_t*)calloc( 1, sizeof(*item) );
    if ( item == ITEM_EMPTY_SLOT ) {
        perror("Failed to allocate memory for item structure");
        return ITEM_EMPTY_SLOT;
    }

    item->entity = entity_init( sprite, position );
    if ( item->entity == ITEM_EMPTY_SLOT ) {
        perror("Failed to create entity for item");
        return ITEM_EMPTY_SLOT;
    }

    strncpy( item->name, name, ITEM_MAX_NAME_LEN );
    item->is_stackable = is_stackable;
    item->count = count;
    item->is_in_inventory = is_in_inventory;

    return item;
}
void item_destroy( item_t* item ) {
    if ( item == ITEM_EMPTY_SLOT ) return;
    entity_destroy( item->entity );
    free( item );
}

item_list_t item_list_new( void ) {
    return ITEM_EMPTY_SLOT;
}
bool item_list_is_empty( item_list_t item_list ) {
    return ITEM_EMPTY_SLOT == item_list;
}

item_list_t item_list_add( item_list_t item_list, item_t* item ) {
    item_list_t new_link = (item_list_t)calloc( 1, sizeof(*new_link) );
    if ( new_link == ITEM_EMPTY_SLOT ) {
        perror("Memory allocation for item list link failed");
        return ITEM_EMPTY_SLOT;
    }

    new_link->item = item;
    new_link->next = item_list;
    return new_link;
}
item_list_t item_list_add_new( item_list_t item_list, sprite_t* sprite, vector2_t position, char* name, bool is_stackable, int count, bool is_in_inventory ) {
    item_t* new_item = item_init( sprite, position, name, is_stackable, count, is_in_inventory );
    if ( new_item == ITEM_EMPTY_SLOT ) {
        perror("Item creation failed during item list incrementation");
        return item_list;
    }

    return item_list_add( item_list, new_item );
}
item_list_t item_list_remove( item_list_t item_list, item_t* item ) {
    if ( item_list_is_empty( item_list ) ) return item_list;

    item_list_t p = item_list;
    item_list_t prev = NULL;

    while ( !item_list_is_empty( p ) ) {
        if ( item_list->item == item ) {
            if ( prev == NULL ) { // if first link
                // Removing the head
                item_list_t new_head = p->next;
                free( p );
                return new_head;
            } else {
                // Removing a middle or last element
                prev->next = p->next;
                free( p );
                return item_list;
            }
        }
        prev = p;
        p = p->next;
    }

    return item_list;
}
item_list_t item_list_grab_first_colliding( item_list_t item_list, rect_t rect, item_t** item ) {
    *item = ITEM_EMPTY_SLOT;
    if ( item_list_is_empty( item_list ) ) return item_list;

    item_list_t p = item_list;
    item_list_t prev = NULL;

    while ( !item_list_is_empty( p ) ) {
        if ( rect_collision( &rect, &p->item->entity->world_rect ) ) {
            *item = p->item;

            if ( prev == NULL ) { // if first link
                // Removing the head
                item_list_t new_head = p->next;
                free( p );
                return new_head;
            } else {
                // Removing a middle or last element
                prev->next = p->next;
                free( p );
                return item_list;
            }
        }
        prev = p;
        p = p->next;
    }

    // No collision found, return unchanged list
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
item_list_t item_list_updateall( item_list_t item_list, vector2_t player_pos, float force, float range2, bool print_debug ) {
    item_list_t list = item_list;
    item_list_t prev = NULL;

    if ( print_debug ) printf("updating loaded items...\n");
    
    while ( !item_list_is_empty( list ) ) {
        if ( print_debug ) {
            printf("updating item "); puts(list->item->name);
            printf("count = %d ; inventory? : %d\n", list->item->count, list->item->is_in_inventory );
            printf("position : "); vector2_print( list->item->entity->world_rect.position ); puts("");
        }

        if ( item_update( list->item, player_pos, force, range2 ) ) {
            prev = list;
            list = list->next;
            continue;
        }
        // else i.e. the item was destroyed
        if ( prev == NULL ) {
            item_list = list->next;
            free( list );
            list = item_list;
        }
        else {
            prev->next = list->next;
            free( list );
            list = prev->next;
        }
    }

    return item_list;
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
    if ( item1 == ITEM_EMPTY_SLOT || item2 == ITEM_EMPTY_SLOT ) return false;

    return ( 0 == strcmp( item1->name, item2->name ) );
}
bool item_is_empty ( item_t* item ) {
    if ( item == ITEM_EMPTY_SLOT ) return true;

    return ( item->count <= 0 );
}
int  item_get_count( item_t* item ) {
    if ( item == ITEM_EMPTY_SLOT ) return 0;

    return item->count;
}

void item_add_one( item_t* item ) {
    if ( item == ITEM_EMPTY_SLOT ) return;

    item->count++;
}
void item_remove_one( item_t* item ) {
    if ( item == ITEM_EMPTY_SLOT ) return;

    item->count--;
}

// if same type, put the most of item2 possible inside item1
// return the remaining count of item2 or -1 if not the same type
int item_group( item_t* item1, item_t* item2 ) {
    if ( item1 == ITEM_EMPTY_SLOT && item2 == ITEM_EMPTY_SLOT ) return 0;
    if ( item1 == ITEM_EMPTY_SLOT || item2 == ITEM_EMPTY_SLOT ) {
        if ( item2 == ITEM_EMPTY_SLOT ) {
            return 0;
        }
        else {
            item1 = item2;
            item2 = ITEM_EMPTY_SLOT;
            return 0;
        }
    }
    if ( !item_is_same( item1, item2 ) ) return -1;

    while ( item1->count < ITEM_MAX_COUNT && item2->count > 0 ) {
        item_remove_one( item2 );
        item_add_one( item1 );
    }
    
    return item2->count;
}

// set the item as outside an inventory and apply 'speed' to its entity component
// if speed is set to 0, a random speed is applied
void item_throw( item_t* item, vector2_t speed ) {
    if ( item == ITEM_EMPTY_SLOT ) return;

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
// if the item is out of render distance, destroy the item and return false
// else return true
bool item_update( item_t* item, vector2_t player_pos, float force, float range2 ) {
    if ( item == ITEM_EMPTY_SLOT ) return true;
    if ( item->is_in_inventory ) return true;

    float distance2 = vector2_distance2( player_pos, entity_output_pos( item->entity ) );

    if ( distance2 > ITEM_RENDER_DISTANCE2 ) {
        item_destroy( item );
        return false;
    }
    if ( distance2 <= range2 ) entity_attract( item->entity, player_pos, force );

    entity_add_force( item->entity, vector2_new( 0.0f, GRAVITY ) );

    entity_apply_force( item->entity );

    return true;
}

// only used for on world items : scale the sprite to match camera's zoom
void item_render_scaled( item_t* item, camera_t* camera, SDL_Renderer* renderer ) {
    if ( item == ITEM_EMPTY_SLOT ) return;
    entity_render( item->entity, camera, renderer );
}
// print the sprite on the renderer at the given position
void item_render       ( item_t* item, int x, int y, SDL_Renderer* renderer ) {
    if ( item == ITEM_EMPTY_SLOT ) return;

    item->entity->sprite->dest_rect.x = x;
    item->entity->sprite->dest_rect.y = y;
    sprite_render( item->entity->sprite, renderer );
}
// print the sprite on the renderer centered on the given position
void item_render_center( item_t* item, int x, int y, SDL_Renderer* renderer ) {
    if ( item == ITEM_EMPTY_SLOT ) return;

    item_render( item, x - item->entity->sprite->src_rect.w/2, y - item->entity->sprite->src_rect.h/2, renderer );
}

void item_print( item_t* item ) {
    printf("name="); puts(item->name);
    printf("count=%d", item->count); puts("");
}
