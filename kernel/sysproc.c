#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  backtrace();
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sigalarm(void)
{
  int ticks;
  uint64 fn;
  argint(0, &ticks);
  argaddr(1, &fn);
  struct proc *p = myproc();
  p->ticks = ticks;
  p->handler = fn;
  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p = myproc();
  p->trapframe->ra = p->restore.ra;
  p->trapframe->sp = p->restore.sp;
  p->trapframe->s0 = p->restore.s0;
  p->trapframe->s1 = p->restore.s1;
  p->trapframe->s2 = p->restore.s2;
  p->trapframe->s3 = p->restore.s3;
  p->trapframe->s4 = p->restore.s4;
  p->trapframe->s5 = p->restore.s5;
  p->trapframe->s6 = p->restore.s6;
  p->trapframe->s7 = p->restore.s7;
  p->trapframe->s8 = p->restore.s8;
  p->trapframe->s9 = p->restore.s9;
  p->trapframe->s10 = p->restore.s10;
  p->trapframe->s11 = p->restore.s11;
  p->trapframe->a0 = p->restore.a0;
  p->trapframe->a1 = p->restore.a1;
  p->trapframe->a2 = p->restore.a2;
  p->trapframe->a3 = p->restore.a3;
  p->trapframe->a4 = p->restore.a4;
  p->trapframe->a5 = p->restore.a5;
  p->trapframe->a6 = p->restore.a6;
  p->trapframe->a7 = p->restore.a7;
  p->trapframe->epc = p->restore.epc;
  p->alarm_return = 0;
  return p->restore.a0;
}