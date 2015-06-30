//
//  switchDebug.c
//  
//
//  Created by Rodrigo Bueno Tomiosso on 27/05/15.
//
//

 #include "switchDebug.h"

int
main(int argc, char *argv[])
{
    
    printf(0, "\nAlternando debug...\n");
    if(switchDebug()){
        printf(0, "\nDEBUG ATIVADO!!!\n");
    }else{
        printf(0, "\nDebug desativado!!\n");
    }
    
    exit();
}

