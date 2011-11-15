#ifndef MYTH_MEM_BARRIER_H_
#define MYTH_MEM_BARRIER_H_

#include "myth_config.h"

//A macro for supressing optimization
#define myth_loop_barrier() asm volatile("":::"memory")

#ifdef MYTH_BARRIER_FENCES
//Memory barriers by x86 fence instructions
//Guarantees successive reads to be executed after this
#define myth_rbarrier() asm volatile("lfence\n":::"memory")
//Guarantees former writes to be executed before this
#define myth_wbarrier() asm volatile("sfence\n":::"memory")
//rbarrier+wbarrier
#define myth_rwbarrier() asm volatile("mfence\n":::"memory")
#elif defined MYTH_BARRIER_CILK
//Memory barriers used by Cilk
//Guarantees successive reads to be executed after this
#define myth_rbarrier() {\
	int x=0,y;\
	asm volatile("xchgl %0,%1":"=r"(x):"m"(y),"0"(x):"memory");\
}
//Guarantees former writes to be executed before this
#define myth_wbarrier() asm volatile("":::"memory")
//rbarrier+wbarrier
#define myth_rwbarrier() myth_rbarrier();
#elif defined MYTH_BARRIER_CILK_WEAK
//R->R orderings are guaranteed by x86[_64] architecture
#define myth_rbarrier() asm volatile("":::"memory")
//W->W orderings are guaranteed by x86[_64] architecture
#define myth_wbarrier() asm volatile("":::"memory")
//R->W orderings are NOT guaranteed. Need to serialize by atomic or fence insns
#define myth_rwbarrier() {\
	int x=0,y;\
	asm volatile("xchgl %0,%1":"=r"(x):"m"(y),"0"(x):"memory");\
}
//#define myth_rwbarrier() asm volatile("mfence":::"memory")
#else
#error "Choose memory barrier"
#endif

#endif /* MYTH_BARRIER_H_ */
