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
    isRoundRobin=!isRoundRobin;
    if (isRoundRobin) {
        printf(1, "\nModo RoundRobin ativado!\n");
    }else{
        printf(1, "\nModo Passo Largo ativado!\n");
    }
    
    exit();
}

