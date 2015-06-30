#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_forkLowest(void){ //#stride 100 tickets
    return forkLowest();
}
int sys_forkLow(void){ //#stride 300 tickets
    return forkLow();
    
}
int sys_forkMedium(void){ //#stride 500 tickets
    return forkMedium();
}
int sys_forkHigh(void){ //#stride 750 tickets
    return forkHigh();
}
int sys_forkHighest(void){ //#stride 990 tickets
    return forkHighest();
}

int
sys_fork(void) //#stride 500 tickets (default)
{
  return fork(tickets);
}
int sys_switchDebug(void){ //#stride alterna modo de debug
    switchScheduler();
    return 0;
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

void
sys_pidInfo(int pid){
    cprintf("\n===================\nPid: %d\nStride: %d\nPass: %d\nLimitPass %d\nTickets: %d",proc->pid,proc->pass,proc->limitpass, proc->tickets);
}



