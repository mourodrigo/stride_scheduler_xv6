//
//  teste2.h
//  stride_scheduler_xv6
//
//  Created by Rodrigo Bueno Tomiosso
//  Copyright (c) 2015 mourodrigo. All rights reserved.
//

#ifndef __stride_scheduler_xv6__teste2__
#define __stride_scheduler_xv6__teste2__
#include "types.h"
#include "stat.h"
#include "user.h"


//Recebe como parametros a prioridade do processo e número de rodadas que realizará o cálculo, em seguida calcula o fatorial de x pelo numero de rodadas
void zombie(int prioridade,int rodadas){
    printf(0, "\nIniciando pid %d com prioridade %d",getpid(),prioridade);
    uint x=0; uint y=0; uint turn = rodadas;
    uint limit = 12;
    uint result = 0;
    uint systemTime = uptime();
    
    for(turn=turn;turn!=1;turn--){
        for(x=1;x<limit;x++){
            result = x;
            for(y=x;y!=1;y--){
                result = result*y;
            }
            
        }
        result=result-10;
    }
    printf(0, "\n Pid %d com prioridade %d finalizado em %d segundos\n\n" ,getpid(),prioridade,uptime()-systemTime);
    exit();
}

//Recebe um numero identificador e numero de tickets que é utilizado na chamada fork
int forkTest(int priority,int rodadas){
    int pid = 0;
    switch (priority) {
        case 1:
            pid = forkLowest();
            break;
        case 2:
            pid = forkLow();
            break;
        case 3:
            pid = forkMedium();
            break;
        case 4:
            pid = forkHigh();
            break;
        case 5:
            pid = forkHighest();
            break;
    }
    
    if (pid == 0)
    {
        // processo filho
        zombie(priority,rodadas);
    }
    else if (pid > 0)
    {
        // processo pai
        //zombie(i);
    }
    else
    {
        // falha no fork
        printf(1, "fork(1) failed!\n");
        return 1;
    }
    
    return 0;
}

#endif /* defined(__stride_scheduler_xv6__teste2__) */
