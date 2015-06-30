//
//  teste3.c
//  stride_scheduler_xv6
//
//  Created by Rodrigo Bueno Tomiosso
//  Copyright (c) 2015 mourodrigo. All rights reserved.
//

#include "teste2.h"

#define SLEEP 100

/
int
main(void)
{
    printf(0, "\nEste teste criara 39 processos com prioridades que variam da mais baixa ate mais alta calcularao o fatorial de 0 ate 12 por 4000000 vezes respectivamente. Apos finalizar o calculo sera apresentado o PID do processo com o tempo de execucao.\n\n");

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
    exit();
}
