//
//  teste1.c
//  stride_scheduler_xv6
//
//  Created by Rodrigo Bueno Tomiosso
//  Copyright (c) 2015 mourodrigo. All rights reserved.
//  Versão modificada do forktest do xv6 onde é passado um número de tickets para o processo conforme o for é percorrido

#include "teste1.h"

#define N  1000

void
forktest(void)
{
    int n, pid;
    
    printf(1, "fork test\n");
    
    for(n=0; n<N; n++){
        pid = forkHighest();
        if(pid < 0)
            break;
        if(pid == 0)
            exit();
    }
    
    if(n == N){
        printf(1, "fork claimed to work N times!\n", N);
        exit();
    }
    
    for(; n > 0; n--){
        if(wait() < 0){
            printf(1, "wait stopped early\n");
            exit();
        }
    }
    
    if(wait() != -1){
        printf(1, "wait got too many\n");
        exit();
    }
    
    printf(1, "Teste de 1000 forks OK\n");
}

int
main(void)
{
    printf(1, "Versao modificada do forktest do xv6 onde e passado um número de tickets (1 a 1000) para o processo conforme o for e percorrido\n");
    sleep(1000);
    forktest();
    exit();
}
