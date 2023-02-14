//
//  plane.c
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//

#include "plane.h"
#include <stdlib.h>
#include <assert.h>

sm_plane *new_plane(const vec2 n, const float d) {

    sm_plane *plane = calloc(1, sizeof(sm_plane));
    assert(plane);
    
    plane->normal = n;
    plane->d = d;
    
    return plane;
}
