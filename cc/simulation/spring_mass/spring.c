//
//  spring.c
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//

#include "spring.h"
#include <stdlib.h>
#include <assert.h>

sm_spring *new_spring(sm_mass * const m1, sm_mass * const m2, const float k, const float l, const float f) {

    sm_spring *spring = calloc(1, sizeof(sm_spring));
    assert(spring);
    
    spring->mass1 = m1;
    spring->mass2 = m2;
    
    spring->k = k;
    spring->l = l;
    spring->k = f;
    
    return spring;
}
