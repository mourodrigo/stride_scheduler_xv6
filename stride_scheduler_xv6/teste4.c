//
//  teste4.c
//  stride_scheduler_xv6
//
//  Created by Rodrigo Bueno Tomiosso
//  Copyright (c) 2015 mourodrigo. All rights reserved.
//

#include "teste4.h"

#define SLEEP 100

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
    int pid;
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

int
main(void)
{
    printf(0, "\nEste teste criara 39 processos com prioridades da mais baixa ate a mais alta, calcularao o fatorial de 0 ate 12 por 2400000 vezes respectivamente. Apos finalizar o calculo sera apresentado o PID do processo com o tempo de execucao.\n\n");
    
    int turns = 2400000;
    
    forkTest(1,turns);
    forkTest(4,turns);
    forkTest(5,turns);
    forkTest(1,turns);
    forkTest(4,turns);
    forkTest(3,turns);
    forkTest(1,turns);
    forkTest(3,turns);
    forkTest(3,turns);
    forkTest(2,turns);
    forkTest(3,turns);
    forkTest(4,turns);
    forkTest(1,turns);
    forkTest(4,turns);
    forkTest(5,turns);
    forkTest(5,turns);
    forkTest(5,turns);
    exit();
}
