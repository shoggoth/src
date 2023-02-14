//
//  spring.h
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//

#ifndef SM_SPRING_H
#define SM_SPRING_H

#include "mass.h"

typedef struct {
    
    sm_mass         *mass1, *mass2;
    float           k;
    float           l;
    float           f;
    
} sm_spring;

#endif
