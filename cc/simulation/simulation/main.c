//
//  main.c
//  simulation
//
//  Created by Richard Henry on 13/02/2023.
//

#define DVECTOR_IMPLEMENTATION

#include "space.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {

    void *foo = calloc(1, sizeof(sm_mass *));
    sm_space *space = dsmsSpaceNew(1, 1, 1);
    
    printf("Space: %p\n", space);
    
    dsmsSpaceFree(space);
    
    return 0;
}
