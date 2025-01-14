#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "procparse.h"

extern uint ticks;

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

int
sys_fork(void)
{
  return fork();
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
sys_getppid(void)
{
	return getppid();
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

  // Process's memory size must be protected
  acquire(&ptable.lock);
  addr = myproc()->sz;
  if(growproc(n) < 0) {
    release(&ptable.lock);
  	return -1;
  }
  release(&ptable.lock);
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

int
sys_yield(void) {
	addticks(ticks); // Before giving up CPU, increase tick info
	yield(); // give up using cpu
	return 0;
}

int
sys_getlev(void) {
	return getlev();
}

int
sys_set_cpu_share(void) {
	return set_cpu_share();
}

int
sys_thread_create(void) {
	return thread_create();
}

int
sys_thread_exit(void) {
	thread_exit();
	return 0;
}

int
sys_thread_join(void) {
	return thread_join();
}

int
sys_gettid(void) {
	return gettid();
}

