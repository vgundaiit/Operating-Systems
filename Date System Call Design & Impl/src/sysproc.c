#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

//Function to get rtcdate argument passed from user level program
// nth argument, pointer to pointer of rtcdate and size of the rtcdate
int argrtcdate(int n,struct rtcdate** rpointer,int size){
  int i;
  /* Get current process */
  struct proc *curproc = myproc();
  //argint is in syscall.c
  if(argint(n, &i) < 0){
    return -1;
  }
  if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz){
    return -1;
  }
  *rpointer = (struct rtcdate*)i;
  return 0;
}

/* sys_date function */
int sys_date(void){
  /* Get the location in the memory where r is stored */
  struct rtcdate *r;

  if(argrtcdate(0,&r,sizeof(struct rtcdate)) < 0){
    return -1;
  }
  cmostime(r);
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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
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
