//
//  main.c
//  simulation
//
//  Created by Richard Henry on 14/02/2023.
//

#include "space.h"
#include <stdio.h>

int main(int argc, const char * argv[]) {
    
    sm_space *space = new_space(0, 0, 0);
    
    printf("Hello, World %p\n", space);
    
    step_space(space);
    
    return 0;
}
