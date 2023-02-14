//
//  space.h
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//

#ifndef SM_SPACE_H
#define SM_SPACE_H

#include "mass.h"
#include "spring.h"
#include "plane.h"

typedef int(*collide_func)(sm_mass *, sm_mass *);

typedef struct {
    
    float               friction;
    float               v_factor, a_factor;
    float               separation_force;
    
    unsigned            number_of_masses;
    unsigned            number_of_springs;
    unsigned            number_of_planes;
    
    sm_mass             **masses, **masses_end;
    sm_spring           **springs, **springs_end;
    sm_plane            **planes, **planes_end;
    
    collide_func        mass_collision_callback;
    
} sm_space;


sm_space *new_space(const unsigned max_masses, const unsigned max_springs, const unsigned max_planes);
void free_space(sm_space * const space);

void step_space(sm_space * const space);

void add_mass_to_space(sm_space *space, sm_mass *mass);
void remove_mass_from_space(sm_space *space, sm_mass *mass);

void add_spring_to_space(sm_space *space, sm_spring *spring);
void remove_spring_from_space(sm_space *space, sm_spring *spring);

void add_plane_to_space(sm_space *space, sm_plane *plane);
void remove_plane_from_space(sm_space *space, sm_plane *plane);

#endif
