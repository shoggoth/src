//
//  space.h
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//


#ifndef SM_SPACE_H
#define SM_SPACE_H

#include "spring.h"
#include "plane.h"


typedef int(*DSSMMassCollisionCallback)(sm_mass *, sm_mass *);

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
    
    DSSMMassCollisionCallback   mass_collision_callback;
    
} sm_space;


sm_space *dsmsSpaceNew(const unsigned max_masses, const unsigned max_springs, const unsigned max_planes);
void dsmsSpaceFree(sm_space * const space);

void dsmsSpaceStep(sm_space * const space);

void dsmsSpaceAddMass(sm_space *space, sm_mass *mass);
void dsmsSpaceRemMass(sm_space *space, sm_mass *mass);

void dsmsSpaceAddSpring(sm_space *space, sm_spring *spring);
void dsmsSpaceRemSpring(sm_space *space, sm_spring *spring);

void dsmsSpaceAddPlane(sm_space *space, sm_plane *plane);
void dsmsSpaceRemPlane(sm_space *space, sm_plane *plane);

#endif
