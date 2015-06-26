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
    switchScheduler();
    printf(0, "\nSys uptime %d",uptime());
    exit();
}

