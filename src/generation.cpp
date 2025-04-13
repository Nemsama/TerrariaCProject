#include <generation.h>

void flip_terrain( int* output, int* input, int size ) {
    if ( output == NULL || input == NULL ) return;

    for ( int i = 0; i < size; i++ ) {
        output[i] = input[size - 1 - i];
    }
}

void convert_terrain_to_world_height( int* world_height, int size, int surface_height ) {
    if ( world_height == NULL ) return;

    for ( int i = 0; i < size; i++ ) {
        world_height[i] = surface_height - world_height[i];
    }
}

void random_terrain( int* terrain, int size, int starting_height, float up_proba, float down_proba, int min_level_width, int max_jump_height, float jump_proba ) {
    int i = 0;
    int level_width = 1;
    int height = starting_height;
    float random_value;

    while ( i < size ) {
        if ( level_width < min_level_width ) {
            terrain[i] = height;
            level_width++;
            i++;
            continue;
        }

        random_value = (float)rand() / (float)RAND_MAX;
        if ( random_value < up_proba ) {
            height++;
            for ( int j = 1; j < max_jump_height; j++ ) {
                if ( rand()/(float)RAND_MAX < jump_proba ) height++;
            }
            terrain[i] = height;
            level_width = 1;
        }
        else if ( random_value < up_proba + down_proba ) {
            height--;
            for ( int j = 1; j < max_jump_height; j++ ) {
                if ( rand()/(float)RAND_MAX < jump_proba ) height--;
            }
            terrain[i] = height;
            level_width = 1;
        }
        else {
            terrain[i] = height;
        }
        i++;
    }
}

// plain parameters : 
//        up_proba = 0.1f;
//      down_proba = 0.1f;
// min_level_width = 6;
// max_jump_height = 1;
//      jump_proba = 0.0f;
void plain( int* terrain, int size, int starting_height ) {
    // default parameters
    float      up_proba = 0.1f;
    float    down_proba = 0.1f;
    int min_level_width = 6;
    int max_jump_height = 1;
    float    jump_proba = 0.0f;

    random_terrain( terrain, size, starting_height, up_proba, down_proba, min_level_width, max_jump_height, jump_proba );
}

// rough plain parameters : 
//        up_proba = 0.18f;
//      down_proba = 0.18f;
// min_level_width = 1;
// max_jump_height = 2;
//      jump_proba = 0.8f;
void rough_plain( int* terrain, int size, int starting_height ) {
    // default parameters
    float      up_proba = 0.18f;
    float    down_proba = 0.18f;
    int min_level_width = 1;
    int max_jump_height = 3;
    float    jump_proba = 0.23f;

    random_terrain( terrain, size, starting_height, up_proba, down_proba, min_level_width, max_jump_height, jump_proba );
}

// slope parameters :
//        up_proba = 0.45f;
//      down_proba = 0.1f;
// min_level_width = 1;
// max_jump_height = 5;
//      jump_proba = 0.05f;
void up_slope( int* terrain, int size, int starting_height ) {
    // default parameters
    float      up_proba = 0.45f;
    float    down_proba = 0.1f;
    int min_level_width = 1;
    int max_jump_height = 5;
    float    jump_proba = 0.05f;

    random_terrain( terrain, size, starting_height, up_proba, down_proba, min_level_width, max_jump_height, jump_proba );
}
void down_slope( int* terrain, int size, int starting_height ) {
    // default parameters
    float      up_proba = 0.1f;
    float    down_proba = 0.45f;
    int min_level_width = 1;
    int max_jump_height = 5;
    float    jump_proba = 0.05f;

    random_terrain( terrain, size, starting_height, up_proba, down_proba, min_level_width, max_jump_height, jump_proba );
}

// sharp slope parameters :
//        up_proba = 0.8f;
//      down_proba = 0.0f;
// min_level_width = 1;
// max_jump_height = 5;
//      jump_proba = 0.4f;
void sharp_up_slope( int* terrain, int size, int starting_height ) {
    // default parameters
    float      up_proba = 0.8f;
    float    down_proba = 0.0f;
    int min_level_width = 1;
    int max_jump_height = 5;
    float    jump_proba = 0.4f;

    random_terrain( terrain, size, starting_height, up_proba, down_proba, min_level_width, max_jump_height, jump_proba );
}
void sharp_down_slope( int* terrain, int size, int starting_height ) {
    // default parameters
    float      up_proba = 0.0f;
    float    down_proba = 0.8f;
    int min_level_width = 1;
    int max_jump_height = 5;
    float    jump_proba = 0.4f;

    random_terrain( terrain, size, starting_height, up_proba, down_proba, min_level_width, max_jump_height, jump_proba );
}

float mountain_func( float x, float height, float width ) {
    // used function is x(1-x) rescaled with height and width
    return 4 * height/width * x * ( 1 - x/width );
}
// flatness is the probability to offset the mountain in the opposite direction (downward when going up and upward when going down)
// negative flatness will have the opposite effect
void random_mountain( int* terrain, int size, int starting_height, int mountain_height, float noise_proba, float flatness, int left_offset, int right_offset ) {
    int i = 0;
    float x = left_offset;
    float mountain_width = (float)size + left_offset - right_offset - 1;
    int vertical_offset = 0;
    int first_height = starting_height - (int)mountain_func( x, mountain_height, mountain_width );

    while ( i < size ) {
        if ( rand()/(float)RAND_MAX < noise_proba ) {
            vertical_offset += rand()%2 == 0 ? -1 : 1;
        }
        if ( rand()/(float)RAND_MAX < fabs( flatness ) ) {
            if ( flatness > 0 ) {
                vertical_offset += ( x > size/2.0f ) ? 1 : -1;
            }
            else {
                vertical_offset += ( x > size/2.0f ) ? -1 : 1;
            }
        }

        terrain[i] = first_height + (int)mountain_func( x, mountain_height, mountain_width ) + vertical_offset;
        i++;
        x += 1.0f;
    }
}

// good mountain parameters :
//            size = 50;
// mountain_height = 30.0f;
//     noise_proba = 0.3f;
//        flatness = 0.1f;
//     left_offset = 0;
//    right_offset = 0;

// high mountain parameters :
//            size = 55 : 60;
// mountain_height = 30.0f;
//     noise_proba = 0.3f;
//        flatness = 0.1f;
//     left_offset = -3;
//    right_offset =  3;

void small_mountain( int* terrain, int size, int starting_height ) {
    // default parameters
    float mountain_height = 30.0f;
    float noise_proba = 0.3f;
    float flatness = 0.1f;
    int left_offset = 2;
    int right_offset = -2;

    random_mountain( terrain, size, starting_height, mountain_height, noise_proba, flatness, left_offset, right_offset );
}
void big_mountain( int* terrain, int size, int starting_height ) {
    // default parameters
    float mountain_height = 30.0f;
    float noise_proba = 0.3f;
    float flatness = 0.1f;
    int left_offset = -3;
    int right_offset = -3;

    random_mountain( terrain, size, starting_height, mountain_height, noise_proba, flatness, left_offset, right_offset );
}
void down_mountain( int* terrain, int size, int starting_height ) {
    // default parameters
    float mountain_height = 30.0f;
    float noise_proba = 0.3f;
    float flatness = 0.1f;
    int left_offset = 4;
    int right_offset = 2;

    random_mountain( terrain, size, starting_height, mountain_height, noise_proba, flatness, left_offset, right_offset );
}
void up_mountain( int* terrain, int size, int starting_height ) {
    // default parameters
    float mountain_height = 30.0f;
    float noise_proba = 0.3f;
    float flatness = 0.1f;
    int left_offset = -2;
    int right_offset = -4;

    random_mountain( terrain, size, starting_height, mountain_height, noise_proba, flatness, left_offset, right_offset );
}



void generate_starting_plain_segment( int* world_height, int* segment_size, int starting_height ) {
    if ( world_height == NULL ) return;

    *segment_size = rand()%51 + 100;
    plain( world_height, *segment_size, starting_height );
}
void generate_segment( int* terrain, int* segment_size, int max_size, int starting_height, float mountain_proba, float slope_proba, float sharp_slope_proba, float rough_plain_proba ) {
    float random_value = (float)rand() / (float)RAND_MAX;

    if ( max_size < 60 ) {
        rough_plain( terrain, max_size, starting_height );
        *segment_size = max_size;
        return;
    }

    // switch
    if ( random_value < mountain_proba ) {
        *segment_size = rand()%11 + 50;
        small_mountain( terrain, *segment_size, starting_height );
    }
    else if ( random_value < mountain_proba + slope_proba ) {
        *segment_size = rand()%15 + 6;
        if ( rand()%2 == 0 ) {
            up_slope( terrain, *segment_size, starting_height );
        }
        else {
            down_slope( terrain, *segment_size, starting_height );
        }
    }
    else if ( random_value < mountain_proba + slope_proba + sharp_slope_proba ) {
        *segment_size = rand()%6 + 4;
        if ( rand()%2 == 0 ) {
            sharp_up_slope( terrain, *segment_size, starting_height );
        }
        else {
            sharp_down_slope( terrain, *segment_size, starting_height );
        }
    }
    else if ( random_value < mountain_proba + slope_proba + sharp_slope_proba + rough_plain_proba ) {
        *segment_size = rand()%25 + 6;
        rough_plain( terrain, *segment_size, starting_height );
    }
    else {
        *segment_size = rand()%21 + 10;
        plain( terrain, *segment_size, starting_height );
    }
}
void generate_up_segment( int* terrain, int* segment_size, int max_size, int starting_height ) {
    if ( max_size < 55 ) {
        up_slope( terrain, max_size, starting_height );
        *segment_size = max_size;
        return;
    }

    if ( rand()%2 == 0 ) {
        *segment_size = rand()%15 + 6;
        up_slope( terrain, *segment_size, starting_height );
    }
    else if ( rand()%2 == 0 ) {
        *segment_size = rand()%7 + 6;
        sharp_up_slope( terrain, *segment_size, starting_height );
    }
    else {
        *segment_size = rand()%11 + 45;
        up_mountain( terrain, *segment_size, starting_height );
    }
}
void generate_down_segment( int* terrain, int* segment_size, int max_size, int starting_height ) {
    if ( max_size < 55 ) {
        down_slope( terrain, max_size, starting_height );
        *segment_size = max_size;
        return;
    }

    if ( rand()%2 == 0 ) {
        *segment_size = rand()%15 + 6;
        down_slope( terrain, *segment_size, starting_height );
    }
    else /* if ( rand()%2 == 0 ) */ {
        *segment_size = rand()%7 + 6;
        sharp_down_slope( terrain, *segment_size, starting_height );
    }/* 
    else {
        *segment_size = rand()%11 + 45;
        down_mountain( terrain, *segment_size, starting_height );
    } */
}
void generate_world_height( int* world_height, int size, int starting_height, int up_limit, int down_limit ) {
    if ( world_height == NULL ) return;

    // default parameters
    float mountain_proba = 0.05f;
    float slope_proba = 0.2f;
    float sharp_slope_proba = 0.08f;
    float rough_plain_proba = 0.32f;
    // float plain_proba = 0.35f; // plain is generated when nothing else is generated

    int i = 0;
    int segment_size = 0;

    // generate starting plain segment
    generate_starting_plain_segment( world_height, &segment_size, starting_height );
    i += segment_size;
    starting_height = world_height[i-1];

    while ( i < size ) {
        if ( starting_height < down_limit ) {
            generate_up_segment( world_height+i, &segment_size, size-i, starting_height );
        }
        else if ( starting_height > up_limit ) {
            generate_down_segment( world_height+i, &segment_size, size-i, starting_height );
        }
        else {
            generate_segment( world_height+i, &segment_size, size-i, starting_height, mountain_proba, slope_proba, sharp_slope_proba, rough_plain_proba );
        }

        i += segment_size;
        starting_height = world_height[i-1];
    }
}
