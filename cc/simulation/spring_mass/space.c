//
//  space.c
//  spring_mass
//
//  Created by Richard Henry on 01/01/2011.
//  Copyright 2011 Dogstar Diversions. http://www.dogstar.mobi
//


#include "space.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


static inline float vec2LengthSquared(const vec2 v) { return vec2DotProduct(v, v); }

static void dsmsCalculateSpringForces(sm_space *space) {

    // Calculate spring forces
    for (unsigned i = 0; i < space->number_of_springs; i++) {
        
        sm_spring *spring = space->springs[i];
        assert(spring);
        
        sm_mass *mass1 = spring->mass1, *mass2 = spring->mass2;
        assert(mass1 && mass2);
        
        if (mass1 && mass2) {
            
            vec2    d = vec2Subtract(mass1->pos, mass2->pos);
            float   l = vec2Length(d);
            
            if (l) {
                
                // Calculate spring force
                vec2 forceVector = vec2Multiply(vec2Multiply(d, -1.0 / l), (l - spring->l) * spring->k);
                
                // Subtract spring friction
                forceVector = vec2Add(forceVector, vec2Multiply(vec2Subtract(mass1->vel, mass2->vel), -spring->f));
                
                mass1->frc = vec2Add(mass1->frc, forceVector);
                mass2->frc = vec2Subtract(mass2->frc, forceVector);
            }
        }
    }
}

static int collision_sort_x_space_compare(const void *e1, const void *e2) {

    const sm_mass *mass1 = *(const sm_mass * const *)e1;
    const sm_mass *mass2 = *(const sm_mass * const *)e2;
    const float m1min = mass1->pos.x - mass1->radius;
    const float m2min = mass2->pos.x - mass2->radius;

    if (m1min == m2min) return 0;

    return (m1min > m2min) ? 1 : -1;
}

static void dsmsResolveObjectToObjectCollisions(sm_space *space) {

    // Partition in x space
    qsort(space->masses, space->number_of_masses, sizeof(sm_mass *), collision_sort_x_space_compare);
    
    // Detect and resolve object - object collisions
    for (unsigned i = 0; i < space->number_of_masses; i++) {
        
        sm_mass *mass_i = space->masses[i];
        
        for (unsigned j = i + 1; j < space->number_of_masses; j++) {
            
            sm_mass *mass_j = space->masses[j];
            
            // Collision mask reject
            if (!(mass_i->collision_mask & mass_j->collision_type)) break;
            
            // Partition reject
            if (mass_i->pos.x + mass_i->radius < mass_j->pos.x - mass_j->radius) break;
            
            vec2    collide_normal = vec2Subtract(mass_j->pos, mass_i->pos);
            float   d_squared = vec2LengthSquared(collide_normal);
            float   radius_sum = mass_i->radius + mass_j->radius;
            
            // Compare distances squared
            if (d_squared < radius_sum * radius_sum) {
                
                // Mass - mass collision callback
                if (!space->mass_collision_callback || space->mass_collision_callback(mass_i, mass_j)) {
                    
                    float     l = vec2LengthSquared(collide_normal);
                    
                    if (l) {
                        
                        float   e = mass_i->e + mass_j->e;
                        float   inverse_mass_sum = 1.0 / mass_i->mass + 1.0 / mass_j->mass;
                        vec2    relative_velocity = vec2Multiply(vec2Subtract(mass_j->vel, mass_i->vel), e);
                        
                        // Calculate impulse
                        collide_normal = vec2Normalize(collide_normal);
                        float impulse = vec2DotProduct(relative_velocity, collide_normal) / inverse_mass_sum;
                        
                        if (impulse < 0) {
                            
                            // Calculate exit velocities
                            mass_i->vel = vec2Add(mass_i->vel, vec2Multiply(collide_normal, impulse / mass_i->mass));
                            mass_j->vel = vec2Add(mass_j->vel, vec2Multiply(collide_normal, -impulse / mass_j->mass));
                            
                        } else {
                            
                            // Calculate separation forces
                            mass_i->frc = vec2Add(mass_i->frc, vec2Multiply(collide_normal, -space->separation_force));
                            mass_j->frc = vec2Add(mass_j->frc, vec2Multiply(collide_normal, space->separation_force));
                            
                        }
                    }
                }
            }
        }
    }
}

static void dsmsResolveObjectToPlaneCollisions(sm_space *space) {

    // Detect and resolve object - plane collisions
    for (unsigned i = 0; i < space->number_of_masses; i++) {
        
        sm_mass *mass = space->masses[i];
        
        if (!mass) continue;
        
        for (unsigned j = 0; j < space->number_of_planes; j++) {
            
            sm_plane *plane = space->planes[j];
            float      d = plane->d + vec2DotProduct(mass->pos, plane->normal);
            
            if (d < mass->radius) {
                
                float e = 1.0 + mass->e;
                float impulse = vec2DotProduct(mass->vel, plane->normal) * e;
                
                if (impulse < 0)
                    mass->vel = vec2Add(mass->vel, vec2Multiply(plane->normal, -impulse));
            }
        }
    }
}

#pragma mark Space management

sm_space *dsmsSpaceNew(const unsigned max_masses, const unsigned max_springs, const unsigned max_planes) {

    sm_space *space = malloc(sizeof(space));
    assert(space);

    space->friction = 0.04;
    space->v_factor = 0.02;
    space->a_factor = 0.0004;
    space->separation_force = 1.0;

    space->masses = calloc(max_masses, sizeof(sm_mass *));
    space->springs = calloc(max_springs, sizeof(sm_spring *));
    space->planes = calloc(max_planes, sizeof(sm_plane *));
    assert(space->masses && space->springs && space->planes);

    space->masses_end = &space->masses[max_masses];
    space->springs_end = &space->springs[max_springs];
    space->planes_end = &space->planes[max_planes];

    space->number_of_masses = 0;
    space->number_of_springs = 0;
    space->number_of_planes = 0;

    space->mass_collision_callback = 0;

    return space;
}

void dsmsSpaceFree(sm_space * const space) {

    free(space->masses);
    free(space->springs);
    free(space->planes);

    free(space);
}

#pragma mark Simulation step

void dsmsSpaceStep(sm_space * const space) {
    
    dsmsCalculateSpringForces(space);
    
    dsmsResolveObjectToObjectCollisions(space);
    
    // Calculate mass effect
    for (sm_mass **p = space->masses; p < space->masses_end; p++) {
        
        sm_mass *mass = *p;
        
        if (mass) {
            
            assert(mass->mass > 0.0);
            
            float massTimesFriction = mass->mass * space->friction;
            
            // a' = f / m
            mass->acc = vec2Multiply(vec2Subtract(mass->frc, vec2Multiply(mass->vel, massTimesFriction)), 1.0 / mass->mass);
            // s' = ut + 0.5at^2
            mass->pos = vec2Add(mass->pos, vec2Add(vec2Multiply(mass->vel, space->v_factor), vec2Multiply(mass->acc, space->a_factor)));
            // v' = v + a
            mass->vel = vec2Add(mass->vel, mass->acc);
            
        } else break;
    }
    
    dsmsResolveObjectToPlaneCollisions(space);
}

#pragma mark Mass management

void dsmsSpaceAddMass(sm_space *space, sm_mass *mass) {
    
    for (sm_mass **p = space->masses; p < space->masses_end; p++) {
        
        assert(*p != mass);
        
        if (!*p) {
            
            *p = mass;
            space->number_of_masses++;
            
            // Initialise the mass' contents
            mass->pos = (vec2) { 0, 0 };
            mass->vel = (vec2) { 0, 0 };
            mass->acc = (vec2) { 0, 0 };
            mass->frc = (vec2) { 0, 0 };
            
            mass->e = 1.0;
            mass->mass = 1.0;
            mass->radius = 0.5;
            
            mass->collision_mask = 0;
            mass->collision_type = 0;
            
            mass->user_data = 0;
            
            break;
        }
    }
}

void dsmsSpaceRemMass(sm_space *space, sm_mass *mass) {
    
    int mass_found = 0;
    
    for (sm_mass **p = space->masses; p < space->masses_end; p++) {
        
        if (*p == mass) {
            
            // Remove the mass if we find it
            assert(!mass_found && space->number_of_masses > 0);
            
            *p = 0;
            space->number_of_masses--;
            mass_found = 1;
            
        } else if (mass_found) {
            
            // Compact the array
            *(p - 1) = *p;
            *p = 0;
        }
    }
    
    assert(mass_found);
}

#pragma mark Spring management

void dsmsSpaceAddSpring(sm_space *space, sm_spring *spring) {
    
    for (sm_spring **p = space->springs; p < space->springs_end; p++) {
        
        assert(*p != spring);
        
        if (!*p) {
            
            *p = spring;
            space->number_of_springs++;
            
            spring->k = 1.0;
            spring->l = 1.0;
            spring->f = 0.0;
            spring->mass1 = 0;
            spring->mass2 = 0;
            
            break;
        }
    }
}

void dsmsSpaceRemSpring(sm_space *space, sm_spring *spring) {
    
    int spring_found = 0;
    
    for (sm_spring **s = space->springs; s < space->springs_end; s++) {
        
        if (*s == spring) {
            
            // Remove the spring if we find it
            assert(!spring_found && space->number_of_springs > 0);
            
            *s = 0;
            space->number_of_springs--;
            spring_found = 1;
            
        } else if (spring_found) {
            
            // Compact the array
            *(s - 1) = *s;
            *s = 0;
        }
    }
    
    assert(spring_found);
}

#pragma mark Plane management

void dsmsSpaceAddPlane(sm_space *space, sm_plane *plane) {
    
    for (sm_plane **p = space->planes; p < space->planes_end; p++) {
        
        assert(*p != plane);
        
        if (!*p) {
            
            *p = plane;
            space->number_of_planes++;
            break;
        }
    }
}

void dsmsSpaceRemPlane(sm_space *space, sm_plane *plane) {
    
    int plane_found = 0;
    
    for (sm_plane **p = space->planes; p < space->planes_end; p++) {
        
        if (*p == plane) {
            
            // Remove the plane if we find it
            assert(!plane_found);
            
            *p = 0;
            space->number_of_planes--;
            plane_found = 1;
            
        } else if (plane_found) {
            
            // Compact the array
            *(p - 1) = *p;
            *p = 0;
        }
    }
    
    assert(plane_found);
}
