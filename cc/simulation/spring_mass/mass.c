//
//  mass.c
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//

#include "mass.h"
#include <stdlib.h>
#include <assert.h>

sm_mass *new_mass(const float m, const float r) {

    sm_mass *mass = calloc(1, sizeof(sm_mass));
    assert(mass);
    
    mass->mass = m;
    mass->radius = r;
    
    return mass;
}
