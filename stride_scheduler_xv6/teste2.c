//
//  teste2.c
//  stride_scheduler_xv6
//
//  Created by Rodrigo Bueno Tomiosso
//  Copyright (c) 2015 mourodrigo. All rights reserved.
//

#include "teste2.h"

#define SLEEP 100

//Recebe um numero identificador e numero de tickets, em seguida calcula o fatorial de x por turn vezez exibindo a cada iteração:
//o turn, indice, pid, numero de tíckets, tempo de CPU, e o fatorial de x.
//OBS: Há um sleep de 100ms para melhor acompanhamento
void zombie(int pi,int tickets){
    printf(0, "\nIniciando pid %d\n",getpid());
    uint x=0; uint y=0; uint turn = 20;
    uint limit = 12;
    uint result = 0;
    for(turn=turn;turn!=1;turn--){
        for(x=1;x<limit;x++){
            result = x;
            for(y=x;y!=1;y--){
                result = result*y;
            }
            
        }
        printf(0, "\nturn %d indice %d pid %d tickets %d tempoCPU %d Fatorial %d = %d",turn,pi,getpid(),tickets,getusage(),x,result);
        sleep(SLEEP);
    }
    printf(0, "\nturn %d indice %d pid %d tickets %d tempoCPU %d Fatorial %d = %d",turn,pi,getpid(),tickets,getusage(),x,result);
    sleep(SLEEP);
    exit();
}

//Recebe um numero identificador e numero de tickets que é utilizado na chamada fork
int forkTest(int i,int tickets){
    int counter = 0;
    int pid = fork(tickets);
    
    if (pid == 0)
    {
        // processo filho
        zombie(i,tickets);
    }
    else if (pid > 0)
    {
        // processo pai
        //zombie(i);
        exit();
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
    printf(0, "\nEste teste criara 5 processos com o mesmo numero de tickets que calcularao o fatorial de 0 ate 12 por 20 vezes com um intervalo de 100ms");
    sleep(SLEEP*10);
    forkTest(1,200);
    sleep(SLEEP);
    forkTest(2,200);
    sleep(SLEEP);

    forkTest(3,200);
    sleep(SLEEP);

    forkTest(4,200);
    sleep(SLEEP);

    forkTest(5,200);

    exit();
}
