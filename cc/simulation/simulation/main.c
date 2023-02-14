//
//  main.c
//  simulation
//
//  Created by Richard Henry on 14/02/2023.
//

#include "space.h"
#include <stdio.h>

int main(int argc, const char * argv[]) {
    
    sm_space *space = dsmsSpaceNew(0, 0, 0);
    
    printf("Hello, World %p\n", space);
    
    dsmsSpaceStep(space);
    
    return 0;
}
