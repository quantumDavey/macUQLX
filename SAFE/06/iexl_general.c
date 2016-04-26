/*
 * (c) UQLX - see COPYRIGHT
 */

#define STATIC static
#include "QL68000.h"

#include <signal.h>
#include <stdio.h>

#include "vm.h"

//#define NEWINT
void debug(char*);
void debug2(char*,long);

void    (**table)(void);
syntab	*syntable;


#ifdef DEBUG
int trace_rts=0;
#endif

#define INLINE inline

uw32 rtop_hard;
int extInt=0;

#ifdef DEBUG
#define TRR  {trace_rts=20;}
#else 
#define TRR
#endif

#define D_ISREG
#include "memaccess.h"

#include "mmodes.h"

rw32
(*iexl_GetEA[8])(ashort) /*REGP1*/ ={GetEA_mBad,GetEA_mBad,GetEA_m2,GetEA_mBad,GetEA_mBad,
		    GetEA_m5,GetEA_m6,GetEA_m7};
vml rw32 (**ll_GetEA)(ashort) /*REGP1*/ =iexl_GetEA;

rw8
(*iexl_GetFromEA_b[8])(void)={GetFromEA_b_m0,GetFromEA_b_mBad,GetFromEA_b_m2,
			 GetFromEA_b_m3,GetFromEA_b_m4,GetFromEA_b_m5,GetFromEA_b_m6,GetFromEA_b_m7};
vml rw8 (**ll_GetFromEA_b)(void)=iexl_GetFromEA_b;


rw16
(*iexl_GetFromEA_w[8])(void)={GetFromEA_w_m0,GetFromEA_w_m1,GetFromEA_w_m2,
			 GetFromEA_w_m3,GetFromEA_w_m4,GetFromEA_w_m5,GetFromEA_w_m6,GetFromEA_w_m7};
vml rw16 (**ll_GetFromEA_w)(void)=iexl_GetFromEA_w;

rw32
(*iexl_GetFromEA_l[8])(void)={GetFromEA_l_m0,GetFromEA_l_m1,GetFromEA_l_m2,
			 GetFromEA_l_m3,GetFromEA_l_m4,GetFromEA_l_m5,GetFromEA_l_m6,GetFromEA_l_m7};
vml rw32 (**ll_GetFromEA_l)(void)=iexl_GetFromEA_l;

void
(*iexl_PutToEA_b[8])(ashort,aw8) /*REGP2*/ ={PutToEA_b_m0,PutToEA_b_mBad,PutToEA_b_m2,PutToEA_b_m3,PutToEA_b_m4,PutToEA_b_m5,PutToEA_b_m6,PutToEA_b_m7};

vml void (**ll_PutToEA_b)(ashort,aw8) /*REGP2*/ =iexl_PutToEA_b;

void
(*iexl_PutToEA_w[8])(ashort,aw16) /*REGP2*/ ={PutToEA_w_m0,PutToEA_w_m1,PutToEA_w_m2,
			    PutToEA_w_m3,PutToEA_w_m4,PutToEA_w_m5,PutToEA_w_m6,PutToEA_w_m7};

vml void (**ll_PutToEA_w)(ashort,aw16) /*REGP2*/ =iexl_PutToEA_w;

void
(*iexl_PutToEA_l[8])(ashort,aw32) /*REGP2*/ ={PutToEA_l_m0,PutToEA_l_m1,PutToEA_l_m2,
			    PutToEA_l_m3,PutToEA_l_m4,PutToEA_l_m5,PutToEA_l_m6,PutToEA_l_m7};

void (**ll_PutToEA_l)(ashort,aw32) /*REGP2*/ =iexl_PutToEA_l;

Cond
(*ConditionTrue[16])(void)={CondT,CondF,CondHI,CondLS,CondCC,CondCS,CondNE,
			    CondEQ,CondVC,CondVS,CondPL,CondMI,CondGE,CondLT,CondGT,CondLE};
vml Cond (**ll_ConditionTrue)(void)=ConditionTrue;

#ifndef G_reg
w32             reg[16];                        /* registri d0-d7/a0-a7 */
#endif
w32             usp,ssp;                        /* user and system stack
						   pointer (aggiornato solo quello non attivo) */

#ifndef GREGS
uw16    *pc;                            /* program counter : Ptr nella */
gshort    code;
int      nInst;
#endif

Cond    trace,supervisor,xflag,negative,zero,overflow,carry;    /*flags */
char    iMask;                          /* SR interrupt mask */
Cond    stopped;                        /* processor status */
volatile char   pendingInterrupt;       /* interrupt requesting service */

#ifndef NEW_AREG
w32             *sp;                            /* *sp==reg[15] : stack
						   pointer */
w32             *aReg;                          /* aReg=reg+8 : address
						   registers */
#else
w32      *g_reg;
#endif

w32             *theROM;                        /* Ptr to ROM in Mac memory */
w32  *ll_theROM;

w32             *ramTop;                        /* Ptr to RAM top in Mac
						   memory */
w32             RTOP;                           /* QL ram top address */
short   exception;                      /* pending exception */
w32             badAddress;                     /* bad address address */
w16             readOrWrite;            /* bad address action */
w32             dummy;                          /* free 4 bytes for who care */
Ptr             dest;                           /* Mac address for
read+write operations */

#if 1
Cond mea_acc;
#else
Cond    isHW;                           /* dest is a HW register ? */
Cond    isDisplay;                      /* dest is in display RAM ? */
#endif
w32             lastAddr;                       /* QL address for
						   read+write operations */

volatile Cond   extraFlag;      /* signals exception or trace */


char    dispScreen=0;           /* screen 0 or 1 */
Cond    dispMode=0;                     /* mode 4 or 8 */
Cond    dispActive=true;        /* display is on ? */
Cond    badCodeAddress;

int   nInst2;
extern int script;

volatile w8     intReg=0;
volatile w8     theInt=0;

Cond doTrace;            /* trace after current instruction */


/******************************************************************/

#ifdef NEWINT
static void ProcessInterrupts(void)
{       /* gestione interrupts */
	if(pendingInterrupt==7 || pendingInterrupt>iMask)
	  {       
	    if(!supervisor)
	{ 
	  usp=(*sp);
	  (*sp)=ssp;
	}
	    ExceptionIn(24+pendingInterrupt);
	    WriteLong((*sp)-4,(w32)pc-(long)theROM);
	    (*sp)-=6;
	    WriteWord(*sp,GetSR());
	    SetPC(theROM[24+pendingInterrupt]);
	    iMask=pendingInterrupt;
	    pendingInterrupt=0;
	    supervisor=true;
	    trace=false;
	    stopped=false;
	  }
}
#else
void ProcessInterrupts(void) 
{   
	/* gestione interrupts */
	if( exception==0 && (pendingInterrupt==7 || pendingInterrupt>iMask)
	    && !doTrace)
	  {    
	    if(!supervisor)
	{   
	  usp=(*sp);
	  (*sp)=ssp;
	}
	    ExceptionIn(24+pendingInterrupt);
	    WriteLong((*sp)-4,(Ptr)pc-(Ptr)theROM); 
	    (*sp)-=6;
	    WriteWord(*sp,GetSR());
	    SetPCX(24+pendingInterrupt);
	    iMask=pendingInterrupt;
	    pendingInterrupt=0;
	    supervisor=true;
	    trace=false;
	    stopped=false;
	    extraFlag=false;
	  }
}
#endif

rw16 GetSR(void)
{   
	rw16 sr;
	sr=(w16)iMask<<8;
	if(trace) sr|=0x8000;
	if(supervisor) sr|=0x2000;
	if(xflag) sr|=16;
	if(negative) sr|=8;
	if(zero) sr|=4;
	if(overflow) sr|=2;
	if(carry) sr|=1;
	return sr;
}

#ifdef NEWINT
void PutSR(aw16 sr)
{
	Cond oldSuper;
	oldSuper=supervisor;
	trace=(sr&0x8000)!=0;
	supervisor=(sr&0x2000)!=0;
	xflag=(sr&0x0010)!=0;
	negative=(sr&0x0008)!=0;
	zero=(sr&0x0004)!=0;
	overflow=(sr&0x0002)!=0;
	carry=(sr&0x0001)!=0;
	iMask=(char)(sr>>8)&7;
	if(oldSuper!=supervisor)
	  {
	    if(supervisor)
	{
	  usp=(*sp);
	  (*sp)=ssp;
	}
	    else
	{
	  ssp=(*sp);
	  (*sp)=usp;
	}
	  }
	extraFlag=doTrace || trace || exception!=0 || pendingInterrupt==7
	  || pendingInterrupt>iMask;
	if(extraFlag)
	  {
	    nInst2=nInst;
	    nInst=0;
	  }
}

#else
void REGP1 PutSR(aw16 sr)
{  
	Cond oldSuper;
	oldSuper=supervisor;
	trace=(sr&0x8000)!=0;
	extraFlag=doTrace || trace || exception!=0;
	if(extraFlag)
	  { 
	    nInst2=nInst;
	    nInst=0;
	  }
	supervisor=(sr&0x2000)!=0;
	xflag=(sr&0x0010)!=0;
	negative=(sr&0x0008)!=0;
	zero=(sr&0x0004)!=0;
	overflow=(sr&0x0002)!=0;
	carry=(sr&0x0001)!=0;
	iMask=(char)(sr>>8)&7;
	if(oldSuper!=supervisor)
	  { 
	    if(supervisor)
	{ 
	  usp=(*sp);
	  (*sp)=ssp;
	}
	    else
	{ 
	  ssp=(*sp);
	  (*sp)=usp;
	}
	  }
	ProcessInterrupts();
}
#endif

rw16 REGP1 BusErrorCode(aw16 dataOrCode)
{    
	if(supervisor) dataOrCode+=4;
	return dataOrCode+readOrWrite+8;
}


void REGP1 SetPCX(int i)
{
#ifdef BACKTRACE
	Ptr p=pc;
#endif

	pc=(uw16*)((Ptr)theROM+(RL(&theROM[i])&ADDR_MASK));
	
#ifdef TRACE 
	CheckTrace();
#ifdef BACKTRACE
	AddBackTrace(p,-i);
#endif
#endif
	
	if(((char)(int)pc&1)!=0) 
	  { 
	    exception=3;
	    extraFlag=true;
	    nInst2=nInst;
	    nInst=0;
	    readOrWrite=16;
	    badAddress=(Ptr)pc-(Ptr)theROM;
	    badCodeAddress=true;
	  }
}

#ifdef BACKTRACE
void SetPCB(w32 addr, int type)
{
	/*  printf("SetPC: addr=%x\n",addr); */

	Ptr p=pc;

	
	if(((char)addr&1)!=0)
	  { 
	    exception=3;
	    extraFlag=true;
	    nInst2=nInst;
	    nInst=0;
	    readOrWrite=16;
	    badAddress=addr;
	    badCodeAddress=true;

	    return;
	  }
	
	pc=(uw16*)((Ptr)theROM+(addr&ADDR_MASK));

	CheckTrace();
	AddBackTrace(p,type);
}

#endif 

INLINE void REGP1 SetPC(w32 addr)
{
	/*  printf("SetPC: addr=%x\n",addr); */

	if(((char)addr&1)!=0)
	  { 
	    exception=3;
	    extraFlag=true;
	    nInst2=nInst;
	    nInst=0;
	    readOrWrite=16;
	    badAddress=addr;
	    badCodeAddress=true;

	    return;
	  }
	
	pc=(uw16*)((Ptr)theROM+(addr&ADDR_MASK));
#ifdef TRACE
	CheckTrace();
#endif
}

#if 1
void ShowException(void){}
#else
void ShowException(void)
{    
	short i;
	int p1,p2,p4;
	unsigned char *p3;

	long xc=exception+3;

	if (exception==0) return;
 	

	p1=(xc);
	p2=((Ptr)pc-(Ptr)theROM-(xc==4? 0:2));
	if(xc==4)
	  { 
	    p3="Illegal code=";
	    p4=(code);
	  }
	else
	  { 
	    p3="";
	    if(xc==3) p3="address error";
	    if(xc==5) p3="divide by zero";
	    if(xc==6) p3="CHK instruction";
	    if(xc==7) p3="TRAPV instruction";
	    if(xc==8) p3="privilege violation";
	    if(xc==9) p3="trace xc";
	    if(xc==10) p3="Axxx instruction code";
	    if(xc==11) p3="Fxxx instruction code";
	    if(xc>35 && xc<48) {p3="TRAP instruction"; p4=xc-35;}
	    else p4=0;
	  }
	printf("Exception %s %d at PC=%x, xx=%d\n",p3,p1,p2,p4);
}
#endif

extern int tracetrap;

#define UpdateNowRegisters()
#if 0
#define ExceptionIn(x)
#else
void REGP1 ExceptionIn(char x)
{
	if (!tracetrap) return;

	printf("Entering TRAP #%d\n",x-32);
	DbgInfo();
}
#endif
#if 1
void ExceptionOut()
{
	if (!tracetrap) return;
	
	printf("RTE\n");
	DbgInfo();
}

#endif

#ifdef NEWINT
void ExceptionProcessing()
{
	if(exception!=0)
	      {
	  if(exception==8) pc--;
	  if(exception<32 || exception>36) /* tutte le eccezioni
					      tranne le trap 0-4 */
	    {       
	      extraFlag=exception<3 || (exception>9 &&
					exception<32) || exception>47;
	      if(!extraFlag) extraFlag=ReadLong(0x28050l)==0;
	      if(extraFlag)
	                      {  
			  UpdateNowRegisters();
			  /*                              ShowException(); */
			  nInst=nInst2=0;
	                      }
	    }
	  if(!supervisor)
	    { 
	      usp=(*sp);
	      (*sp)=ssp;
	    }
	  ExceptionIn(exception);
	  (*sp)-=6;
	  WriteLong((*sp)+2,(w32)pc-(w32)theROM);
	  WriteWord((*sp),GetSR());
	  SetPC(theROM[exception]);
	  if(exception==3) /* address error */
	    {
	      (*sp)-=8;
	      WriteWord((*sp)+6,code);
	      WriteLong((*sp)+2,badAddress);
	      WriteWord((*sp),BusErrorCode(badCodeAddress? 2:1));
	      badCodeAddress=false;
	    }
	  supervisor=true;
	  trace=false;
	      }
	if(doTrace && exception!=3 && exception!=4 && exception!=8)
	  { 
	    if(!supervisor)
	{
	  usp=(*sp);
	  (*sp)=ssp;
	}
	    ExceptionIn(9);
	    (*sp)-=6;
	    WriteLong((*sp)+2,(w32)pc-(long)theROM);
	    WriteWord((*sp),GetSR());
	    SetPC(theROM[9]);
	    supervisor=true;
	    trace=false;
	    stopped=false;
	  }
	if(pendingInterrupt!=0) ProcessInterrupts();
	exception=0;
	extraFlag=doTrace=trace;
	if(extraFlag)
	  {
	    nInst2=nInst;
	    nInst=2;
	  }
}

#else
void ExceptionProcessing()
{
	if(pendingInterrupt!=0 && !doTrace) ProcessInterrupts();
	if(exception!=0)
	  {
	    if(exception<32 || exception>36) /* tutte le eccezioni
					  tranne le trap 0-4 */
	{  
	  extraFlag=exception<3 || (exception>9 &&
				    exception<32) || exception>47;
	  if(!extraFlag) extraFlag=ReadLong(0x28050l)==0;
	  if(extraFlag)
	    {    
	      UpdateNowRegisters();
	      /*ShowException(); */
	      nInst=nInst2=0;
	    }
	}
	    if(!supervisor)
	{ 
	  usp=(*sp);
	  (*sp)=ssp;
	}
	    ExceptionIn(exception);
	    (*sp)-=6;
	    WriteLong((*sp)+2,(w32)pc-(w32)theROM);
	    WriteWord((*sp),GetSR());
	    SetPCX(exception);
	    if(exception==3) /* address error */
	{ 
	  (*sp)-=8;
	  WriteWord((*sp)+6,code);
	  WriteLong((*sp)+2,badAddress);
	  WriteWord((*sp),BusErrorCode(badCodeAddress? 2:1));
	  badCodeAddress=false;
	  if(nInst) exception=0;
	} else exception=0; /* allow interrupts */
	    extraFlag=false;
	    supervisor=true;
	    trace=false;
	  }
	 if(doTrace)
	  {      
	    if(!supervisor)
	{ 
	  usp=(*sp);
	  (*sp)=ssp;
	}
	    ExceptionIn(9);
	    (*sp)-=6;
	    WriteLong((*sp)+2,(Ptr)pc-(Ptr)theROM);
	    WriteWord((*sp),GetSR());
	    SetPCX(9);
	    if(nInst==0) exception=9;       /* no interrupt allowed here */
	    supervisor=true;
	    /*doTrace=*/trace=false;
	    extraFlag=false;
	    stopped=false;
	  }
	doTrace=trace;
	if(doTrace) {nInst2=nInst;nInst=1;}
	if(pendingInterrupt!=0 && !doTrace)   /* delay interrupt after trace exception */
	  {
	    extraFlag=true;
	    nInst2=nInst;
	    nInst=0;
	  }
	
}
#endif

/******************************************************************/
/* now read in ReadByte etc macros */

#if 0
#define D_ISREG
#include "memaccess.h"
#endif //0

rw32 AREGP GetEA_mBad(ashort r)
{    
	exception=4;
	extraFlag=true;
	nInst2=nInst;
	nInst=0;
	return 0;
}



/********************************************************************/

/* fetch and dispatch loop: 68K machine code version
void ExecuteLoop(void)  esecuzione di nInst istruzioni
{       void    *where;         pc in asm =E8 un nome riservato !!!

	      where=&pc;
	      asm{
	              MOVEM.L   A2-A4,-(A7)
	              MOVEA.L   where,A2
	              LEA       nInst,A3
	              MOVEA.L   table,A4
	              BRA.S     @doLoop
loop:   MOVEA.L   (A2),A0
	              ADDQ.L    #2,(A2)
	              MOVEQ     #0,D0
	              MOVE.W    (A0),D0
	              MOVE.W    D0,code
	              LSL.L     #2,D0
	              MOVEA.L   0(A4,D0.L),A0
	              JSR       (A0)
doLoop: SUBQ.L    #1,(A3)
	              BGE.S     @loop
	              TST.B     extraFlag
	              BEQ.S     @endLoop
	              MOVE.L    nInst2,(A3)
	              JSR       ExceptionProcessing
	              TST.L     (A3)
	              BGT.S     @loop
endLoop: MOVEM.L   (A7)+,A2-A4
	              }
}
*/



/*  */

extern volatile int doPoll;

#ifdef FASTLOOP
static int instbl_valid=0;

void ExecuteLoop(void)
{
	static void *instbl[65536];
	static syntab syntbl[65536];
	static char littab[][20]={"","#1","#2","#4","CCR","SR","USP","","D%1d","A%1d","(A%1d)","(A%1d)+","-(A%1d)",
						"$%04x(A%1d)","$%02x(A%1d,%1s%1d)", "$%04x.W", "$%08x.L","$%04x(PC)","$%02x(PC,%1s%1d)" };
	
	static char OpCode[]="\0Invalid\0a_line\0abcd\0add.b\0add.l\0add.w\0adda.l\0adda.w\0addi.b\0addi.l\0addi.w\0addq.b\0addq.l\0addq.w\0addx.b\0addx.l\0addx.w\0and.b\0and.l\0and.w\0andi\0andi.b\0andi.l\0andi.w\0asl\0asl.b\0asl.l\0asl.w\0asr\0asr.b\0asr.l\0asr.w\0bcc.l\0bcc.s\0bcc_BAD\0bccc.s\0bchg\0bchg.s\0bclr\0bclr.s\0bcs.s\0beq.l\0beq.s\0bge.s\0bgt.s\0bhi.s\0ble.s\0bls.s\0blt.s\0bmi.s\0bne.l\0bne.s\0bpl.s\0bra.l\0bra.s\0bset\0bset.s\0bsr\0btst\0btst.s\0bvc.s\0bvs.s\0chk.w\0clr.b\0clr.l\0clr.w\0cmp.b\0cmp.l\0cmp.w\0cmpa.l\0cmpa.w\0cmpi.b\0cmpi.l\0cmpi.w\0cmpm.b\0cmpm.l\0cmpm.w\0dbcc\0dbcs\0dbeq\0dbf\0dbge\0dbgt\0dbhi\0dble\0dbls\0dblt\0dbmi\0dbne\0dbpl\0dbt\0dbvc\0dbvs\0divs.w\0divu.w\0eor.b\0eor.l\0eor.w\0eori\0eori.b\0eori.l\0eori.w\0exg_a\0exg_ad\0exg_d\0ext.l\0ext.w\0f_line\0illegal\0jmp\0jsr\0lea\0link\0lsl\0lsl.b\0lsl.l\0lsl.w\0lsr\0lsr.b\0lsr.l\0lsr.w\0move\0move.b\0move.l\0move.w\0movea.l\0movea.w\0movem.l\0movem.w\0movep.l\0movep.w\0moveq\0muls.w\0mulu.w\0nbcd\0neg.b\0neg.l\0neg.w\0negx.b\0negx.l\0negx.w\0nop\0not.b\0not.l\0not.w\0or.b\0or.l\0or.w\0ori\0ori.b\0ori.l\0ori.w\0pea\0reset\0rol\0rol.b\0rol.l\0rol.w\0ror\0ror.b\0ror.l\0ror.w\0roxl\0roxl.b\0roxl.l\0roxl.w\0roxr\0roxr.b\0roxr.l\0roxr.w\0rte\0rtr\0rts\0sCC\0sbcd\0scc\0scs\0seq\0sf\0sge\0sgt\0shi\0sle\0sls\0slt\0smi\0sne\0spl\0st\0stop\0sub.b\0sub.l\0sub.w\0s ubi.b\0subi.l\0subi.w\0subq.b\0subq.l\0subq.w\0subx.b\0subx.l\0subx.w\0svc\0svs\0swap\0tas\0trap\0trap#0\0trap#1\0trap#2\0trap#3\0trap#4\0trapv\0tst.b\0tst.l\0tst.w\0unlk\0\0";


	register void **itab;


#if  1 /*MANYREGS*/
#ifndef G_reg
#ifdef NEW_AREG
	w32*  reg=g_reg;
#else
	w32 *aReg=reg+8;
	w32* reg=aReg-8;
#endif
#endif
	w32 (**GetEA)(ashort) /*REGP1*/ = iexl_GetEA;
	rw8  (**GetFromEA_b)(void) = iexl_GetFromEA_b;
	rw16 (**GetFromEA_w)(void) = iexl_GetFromEA_w;
	rw32 (**GetFromEA_l)(void) = iexl_GetFromEA_l;
	void (**PutToEA_b)(ashort,aw8) /*REGP2*/ = iexl_PutToEA_b;
	void (**PutToEA_w)(ashort,aw16) /*REGP2*/ = iexl_PutToEA_w;
	void (**PutToEA_l)(ashort,aw32) /*REGP2*/ = iexl_PutToEA_l;
	Cond (**ConditionTrue)(void) = ll_ConditionTrue;

	w32 *theROM=ll_theROM;
#else
#define GetFromEA_b iexl_GetFromEA_b
#define GetFromEA_w iexl_GetFromEA_w
#define GetFromEA_l iexl_GetFromEA_l
#define PutToEA_b iexl_PutToEA_b
#define PutToEA_w iexl_PutToEA_w
#define PutToEA_l iexl_PutToEA_l
#endif

	code=0;   /* loop possibly sets only 16 bits ! */

	itab=instbl;
	
#define IE_XL

	goto run_it; 
	
	/*#include "iexl_general.h"*/

#include "instructions_ao.c"    /* include instructions */
#include "instructions_pz.c"
	/*#include "iexl_ug.h"*/

run_it:
	if (unlikely(!instbl_valid))
	  {
#define IE_XL_II 
#include "Init.c"
	    /*XSetTable(instbl);*/
	    instbl_valid=1;
	  }
	theROM=ll_theROM;
	ENTER_IEXL;                  /* load vars into regs etc .*/

nextI: 
	if (likely(--nInst>=0))
	  {
#ifdef TRACE
	    if (pc>=tracelo) DoTrace();
#endif
	    /*DbgInfo();*/
	    
#if defined(ASSGN_486)
	    goto *itab[ASSGN_486()];
#else
	    goto *itab[ASSGN_CODE(RW(pc++))];
#endif
	  } 
	
#if 0
	if (regEmux)
	  {
	    nInst= (extraFlag ? 0 : reInst);     
	    vm_regemu();
 
	    goto nextI;
	  }
#endif 

	if (doPoll) dosignal();

	if(extraFlag)
	  {
	    nInst=nInst2;
	    ExceptionProcessing();
	    if(nInst>0) goto nextI;
	  }
}

#else
void ExecuteLoop(void)  /* fetch and dispatch loop */
{     
	register void           (**tab)(void);

	tab=table;

rep: 
	while(--nInst>=0)    
	  {

#ifdef TRACE 
	    if (pc>tracelo) DoTrace();
#endif
	    
	    tab[code=RW(pc++)]();
	  }
	
	if (doPoll) dosignal();

	if(extraFlag)
	  {
	    nInst=nInst2;
	    ExceptionProcessing();
	    if(nInst>0) goto rep;
	  }
}
#endif

void ExecuteChunk(long n)       /* execute n emulated 68K istructions */
{  
	ll_theROM=theROM;

	if((long)pc&1) return;

	extraFlag=false;
	ProcessInterrupts();

	if(stopped) return;
	exception=0;
#ifdef NEWINT
	extraFlag=extraFlag || trace || doTrace;
#else
	extraFlag=trace || doTrace || pendingInterrupt==7 ||
	  pendingInterrupt>iMask;
#endif
	nInst=n+1;
	if(extraFlag)
	  { 
	    nInst2=nInst;
#ifdef NEWINT
	    nInst=2;
#else
	    nInst=0;
#endif
	  }

 restart:
	ExecuteLoop();

}




void InitCPU(void) /* 68K state when powered on */
{ 
	ssp=*sp=RL(&theROM[0]);
	SetPC(RL(&theROM[1]));
	if(V3)printf("initial PC=%x SP=%x\n",(void*)pc-(void*)theROM,ssp);
	
	iMask=7;
	supervisor=true;
	trace=doTrace=false;
	exception=0;
	extraFlag=false;
	pendingInterrupt=0;
	stopped=false;
	badCodeAddress=false;
}




















