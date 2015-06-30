//
//  teste3.c
//  stride_scheduler_xv6
//
//  Created by Rodrigo Bueno Tomiosso
//  Copyright (c) 2015 mourodrigo. All rights reserved.
//

#include "teste2.h"

#define SLEEP 100

//Recebe um numero identificador e numero de tickets, em seguida calcula o fatorial de x por turn vezes exibindo a cada iteração:
//o turn, indice, pid, numero de tíckets, tempo de CPU, e o fatorial de x.
//OBS: Há um sleep de 100ms para melhor acompanhamento
void zombie(int pi,int tickets,int rodadas){
    printf(0, "\nIniciando pid %d com prioridade %d",getpid(),tickets);
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
        //printf(0, "\nturn %d indice %d pid %d tickets %d tempoCPU %d Fatorial %d = %d",turn,pi,getpid(),tickets,getusage(),x,result);
      //  pidInfo(getpid());
//        switchScheduler();
        //printf(0, "\n\nPid %d Turn %d\n\n", pi,turn);
//        sleep(SLEEP);
        
    }
//    printf(0, "\nturn %d indice %d pid %d tickets %d tempoCPU %d Fatorial %d = %d\n\n",turn,pi,getpid(),tickets,getusage(),x,result);
    printf(0, "\n Pid %d com prioridade %d finalizado em %d segundos\n\n" ,getpid(),tickets,uptime()-systemTime);

    //pidInfo(getpid());
//    switchScheduler();

//    sleep(SLEEP);
    exit();
}

//Recebe um numero identificador e numero de tickets que é utilizado na chamada fork
int forkTest(int priority,int rodadas){
  //  int counter = 0;
    
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
        zombie(priority,priority,rodadas);
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
    printf(0, "\nEste teste criara 39 processos com prioridades que variam da mais baixa ate mais alta calcularao o fatorial de 0 ate 12 por 4000000 vezes respectivamente. Apos finalizar o calculo sera apresentado o PID do processo com o tempo de execucao.\n\n");
  //  switchScheduler();
    
    int turns = 2400000;
    
    forkTest(1,turns);
    forkTest(2,turns);
    forkTest(3,turns);
    forkTest(4,turns);
    forkTest(5,turns);
    forkTest(4,turns);
    forkTest(3,turns);
    forkTest(2,turns);
    forkTest(1,turns);
    forkTest(2,turns);
    forkTest(3,turns);
    forkTest(4,turns);
    forkTest(5,turns);
    forkTest(1,turns);
    forkTest(2,turns);
    forkTest(3,turns);
    forkTest(4,turns);
    forkTest(5,turns);
    forkTest(4,turns);
    forkTest(3,turns);
    forkTest(2,turns);
    forkTest(1,turns);
    forkTest(2,turns);
    forkTest(3,turns);
    forkTest(4,turns);
    forkTest(5,turns);
    forkTest(1,turns);
    forkTest(2,turns);
    forkTest(3,turns);
    forkTest(4,turns);
    forkTest(5,turns);
    forkTest(4,turns);
    forkTest(3,turns);
    forkTest(2,turns);
    forkTest(1,turns);
    forkTest(2,turns);
    forkTest(3,turns);
    forkTest(4,turns);
    forkTest(5,turns);

    
//    settickets(50);
    sleep(500);
    
    exit();
}
