//
//  mass.h
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//

#ifndef SM_MASS_H
#define SM_MASS_H

#include "vector.h"

typedef struct {
    
    // Dynamic properties
    vec2            pos;
    vec2            vel;
    vec2            acc;
    vec2            frc;
    
    // Physical properties
    float           e;
    float           mass;
    float           radius;
    
    // Computational properties
    unsigned short  collision_type;
    unsigned short  collision_mask;
    
    void            *user_data;
    
} sm_mass;

sm_mass *new_mass(const float m, const float r);

#endif
