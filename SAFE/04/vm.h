/*
 * (c) UQLX - see COPYRIGHT
 */

#if defined(USE_VM)
#include <signal.h>

typedef struct STATE {
  uw16 code;
  void *pc;
  uw32 save_aReg[8];
 }x_state;

x_state orig_state,res_state;



#if 0 /*defined(USE_VM) && defined(GREGS)*/
extern void segv_handler(int , siginfo_t  , ucontext_t *);
#else
extern void segv_handler(int , siginfo_t *);
extern void buserr_handler(int , siginfo_t );
#endif
extern void vm_on(void);
extern void vm_off(void);
extern void vm_emureg(void);

#else 
#if  defined(USE_VM) 

typedef struct STATE {
  uw16 code;
  void *pc;
  uw32 save_aReg[8];
 }x_state;

x_state orig_state,res_state;


extern long pagesize;
extern char *scrModTable;
extern int sct_size;
extern char * oldscr;


extern void buserr_handler(int  );
extern void segv_handler(int );


extern void vm_on(void);
extern void vm_off(void);
#endif
#endif

