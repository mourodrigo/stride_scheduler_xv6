//
//  switchScheduler.c
//  
//
//  Created by Rodrigo Bueno Tomiosso on 27/05/15.
//
//

 #include "switchScheduler.h"

int
main(int argc, char *argv[])
{
    
    printf(0, "\nAlternando escalonador...\n");
    int s = switchScheduler();
    if (s) {
        printf(0, "\nRound Robin...\n");
        
    }else{
        printf(0, "\nStride Scheduler...\n");
        
    }
    exit();
}

