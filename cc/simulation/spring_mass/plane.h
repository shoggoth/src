//
//  plane.h
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//

#ifndef SM_PLANE_H
#define SM_PLANE_H

#include "vector.h"

typedef struct {
    
    vec2            normal;
    float           d;
    
} sm_plane;

sm_plane *new_plane(const vec2 n, const float d);

#endif
