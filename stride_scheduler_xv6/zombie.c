// Create a zombie process that 
// must be reparented at exit.

#include "types.h"
#include "stat.h"
#include "user.h"
int Fibonacci(int n) { 
	if(n <= 0) return 0;
	if(n > 0 && n < 3) return 1;

	int result = 0;
	int preOldResult = 1;
	int oldResult = 1;
	int i=2;
	for (i=2;i<n;i++) { 
	    result = preOldResult + oldResult;
	    preOldResult = oldResult;
	    oldResult = result;
	}

	return result;
}

void zombie(int pi,int tickets){
	printf(0, "\nIniciando pid %d\n",getpid());
	uint x=0; uint y=0; uint turn = 500;
	uint limit = 12;
	uint result = 0;
	for(turn=500;turn!=1;turn--){
		for(x=1;x<limit;x++){
			result = x;
			for(y=x;y!=1;y--){
				result = result*y;
			}
			printf(0, "\nturn %d pid %d tickets %d usage %d Fatorial %d = %d",turn,getpid(),tickets,getusage(),x,result);
			sleep(100);
		}
	}
//	if(getpid()!=0){
	printf(0, "\nturn %d pid %d tickets %d Fatorial %d = %d",turn,getpid(),tickets,x,result);		
	exit();
//	}
}

int forkTest(int i,int tickets){
    int counter = 0;
    int pid = fork(tickets);

    if (pid == 0)
    {
        // child process
        zombie(i,tickets);
    }
    else if (pid > 0)
    {
        // parent process
        //zombie(i);
    }
    else
    {
        // fork failed
        printf(1, "fork(1) failed!\n");
        return 1;
    }

return 0;
}

int
main(void)
{
	int forks=5;
	int childIndex;
	forkTest(2,2);
	forkTest(500,500);
	//forkTest(800,800);
	forkTest(700,700);
	
	//for(childIndex = 0;childIndex!=forks;childIndex++){
	//	forkTest(childIndex);
  	//}

exit();
}
