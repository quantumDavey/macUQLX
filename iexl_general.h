/*
 * (c) UQLX - see COPYRIGHT
 */
#ifndef IEXL_GENERAL_H
#define UEXL_GENERAL_H

#ifdef USE_VM

#if 0
#define D_ISREG short isreg;
#else
#define D_ISREG
#endif

#define FIX_INS


/* define a few FAST inlines */

#define ReadByte(_addr_)    ({\
  w32 __x_addr=(_addr_);\
  __x_addr&=ADDR_MASK;\
  FIX_INS\
  *((w8*)theROM+__x_addr);\
})
#define inl_ReadByte ReadByte

#if HOST_ALIGN>1
#define ReadWord(_addr_)   ({\
  w32 __x_addr=(_addr_);\
  if(((char)__x_addr&1)!=0) \
    {   \
      exception=3; \
      extraFlag=true; \
      nInst2=nInst; \
      nInst=0; \
      badAddress=__x_addr; \
      readOrWrite=16;\
      return; \
    } \
  __x_addr&=ADDR_MASK_E; \
  FIX_INS\
  RW((w16*)((Ptr)theROM+__x_addr));\
})
#else
#define ReadWord(_addr_)   ({\
  w32 __x_addr=(_addr_);\
  __x_addr&=ADDR_MASK;\
  FIX_INS\
  RW((w16*)((Ptr)theROM+__x_addr));\
})
#endif

#if HOST_ALIGN>1
#define ReadLong(_addr_)   ({\
  w32 __x_addr=(_addr_);\
  if(((char)__x_addr&1)!=0) \
    {   \
      exception=3; \
      extraFlag=true; \
      nInst2=nInst; \
      nInst=0; \
      badAddress=__x_addr; \
      readOrWrite=16;\
      return; \
    } \
  __x_addr&=ADDR_MASK_E; \
  FIX_INS\
  RL((Ptr)theROM+__x_addr);\
})
#else
#define ReadLong(_addr_)   ({\
  w32 __x_addr=(_addr_);\
  __x_addr&=ADDR_MASK;\
  FIX_INS\
  RL((Ptr)theROM+__x_addr);\
})
#endif

#define inl_ReadLong ReadLong
#define inl_ReadWord ReadWord

#define WriteByte(_addr_,_d_)      ({\
  w32 __x_addr=(_addr_);\
  w8 __x_d=(_d_);\
  __x_addr&=ADDR_MASK;\
  FIX_INS\
  *((w8*)theROM+__x_addr)=__x_d;\
})
#define inl_WriteByte WriteByte

#if HOST_ALIGN>1
#define WriteWord(_addr_,_d_)        ({\
  w32 __x_addr=(_addr_);\
  w16 __x_d=(_d_);\
  if(((char)__x_addr&1)!=0) \
    {   \
      exception=3; \
      extraFlag=true; \
      nInst2=nInst; \
      nInst=0; \
      badAddress=__x_addr; \
      readOrWrite=16;\
      return ; \
    } \
  __x_addr&=ADDR_MASK_E; \
  FIX_INS\
  WW((Ptr)theROM+__x_addr,__x_d);\
})
#else
#define WriteWord(_addr_,_d_)        ({\
  w32 __x_addr=(_addr_);\
  w16 __x_d=(_d_);\
  __x_addr&=ADDR_MASK;\
  FIX_INS\
  WW((Ptr)theROM+__x_addr,__x_d);\
})
#endif

#define inl_WriteWord WriteWord

#if HOST_ALIGN>1
#define WriteLong(_addr_,_d_)        ({\
  w32 __x_addr=(_addr_);\
  w32 __x_d=(_d_);\
  if(((char)__x_addr&1)!=0) \
    {   \
      exception=3; \
      extraFlag=true; \
      nInst2=nInst; \
      nInst=0; \
      badAddress=__x_addr; \
      readOrWrite=16;\
      return ; \
    } \
  __x_addr&=ADDR_MASK_E; \
  FIX_INS\
  WL((Ptr)theROM+__x_addr,__x_d);\
})
#else
#define WriteLong(_addr_,_d_)        ({\
  w32 __x_addr=(_addr_);\
  w32 __x_d=(_d_);\
  __x_addr&=ADDR_MASK;\
  FIX_INS\
  WL((Ptr)theROM+__x_addr,__x_d);\
})
#endif

#define inl_WriteLong WriteLong


#if 1
     /****************************************************/
#ifndef QM_BIG_ENDIAN
static int isreg=0;
#endif

static REGP2 rw8 ModifyAtEA_b(ashort mode,ashort r)
{   
  shindex displ;
  w32     addr;
  switch(mode) {
  case 0:
    dest=(Ptr)(&reg[r])+RBO;
    return *((w8*)dest);
  case 2:addr=aReg[r];
    break;
  case 3:addr=aReg[r]++;
    if(r==7) (*sp)++;
    break;
  case 4:
    if(r==7) (*sp)--;
    addr=--aReg[r];
    break;
  case 5:addr=aReg[r]+(w16)RW(pc++);
    break;
  case 6:
    displ=(w16)RW(pc++);
    if((displ&2048)!=0) addr=reg[(displ>>12) & 15]+
			  aReg[r]+(w32)((w8)displ);
    else addr=(w32) ((w16)(reg[(displ>>12) & 15]))+
	   aReg[r]+(w32)((w8)displ);
    break;
  case 7:
    switch(r) {
    case 0:addr=(w16)RW(pc++);
      break;
    case 1:addr=RL((w32*)pc);
      pc+=2;
      break;
    default:
      exception=4;
      extraFlag=true;
      nInst2=nInst;
      nInst=0;
      dest=(Ptr)(&dummy);
      return 0;
    }
    break;
  default:
    exception=4;
    extraFlag=true;
    nInst2=nInst;
    nInst=0;
    dest=(Ptr)(&dummy);
    return 0;
  }

  addr&=ADDR_MASK;

  dest=(Ptr)theROM+addr;
  return *((w8*)dest);
}

static REGP2 rw16 ModifyAtEA_w(ashort mode,ashort r)
{  
  shindex displ;
  w32     addr;
#ifndef QM_BIG_ENDIAN
  isreg=0;
#endif
  switch(mode) {
  case 0:
    dest=(Ptr)(&reg[r])+RWO;
#ifndef QM_BIG_ENDIAN
    isreg=1;
#endif
    return *((w16*)dest);
  case 1:
#ifndef QM_BIG_ENDIAN
    isreg=1;
#endif
    dest=(Ptr)(&aReg[r])+RWO;
    return *((w16*)dest);
  case 2:addr=aReg[r];
    break;
  case 3:addr=aReg[r];
    aReg[r]+=2;
    break;
  case 4:addr=(aReg[r]-=2);
    break;
  case 5:addr=aReg[r]+(w16)RW(pc++);
    break;
  case 6:
    displ=(w16)RW(pc++);
    if((displ&2048)!=0) addr=reg[(displ>>12) & 15]+
			  aReg[r]+(w32)((w8)displ);
    else addr=(w32)((w16)(reg[(displ>>12) & 15]))+
	   aReg[r]+(w32)((w8)displ);
    break;
  case 7:
    switch(r) {
    case 0:addr=(w16)RW(pc++);
      break;
    case 1:addr=RL((w32*)pc);
      pc+=2;
      break;
    default:
      exception=4;
      extraFlag=true;
      nInst2=nInst;
      nInst=0;
      dest=(Ptr)(&dummy);
      return 0;
    }
    break;
  }
#if HOST_ALIGN>1
  if(((short)addr&1)!=0)
    { 
      exception=3;
      extraFlag=true;
      nInst2=nInst;
      nInst=0;
      badAddress=addr;
      readOrWrite=16;
      dest=(Ptr)(&dummy);
      return 0;
    }
  addr&=ADDR_MASK_E;
#else
  addr&=ADDR_MASK;
#endif  

  dest=(Ptr)theROM+addr;
  return RW((w16*)dest);
}



static REGP2 rw32 ModifyAtEA_l(ashort mode,ashort r)
{   
  shindex displ;
  w32     addr;

#ifndef QM_BIG_ENDIAN
  isreg=0;
#endif
  switch(mode) {
  case 0:
#ifndef QM_BIG_ENDIAN
    isreg=1;
#endif
    dest=(Ptr)(&reg[r]);
    return *((w32*)dest);
  case 1:
#ifndef QM_BIG_ENDIAN
    isreg=1;
#endif
    dest=(Ptr)(&aReg[r]);
    return *((w32*)dest);
  case 2:addr=aReg[r];
    break;
  case 3:addr=aReg[r];
    aReg[r]+=4;
    break;
  case 4:addr=(aReg[r]-=4);
    break;
  case 5:addr=aReg[r]+(w16)RW(pc++);
    break;
  case 6:
    displ=(w16)RW(pc++);
    if((displ&2048)!=0) addr=reg[(displ>>12) & 15]+
			  aReg[r]+(w32)((w8)displ);
    else addr=(w32)((w16)(reg[(displ>>12) & 15]))+
	   aReg[r]+(w32)((w8)displ);
    break;
  case 7:
    switch(r) {
    case 0:addr=(w16)RW(pc++);
      break;
    case 1:addr=RL((w32*)pc);
      pc+=2;
      break;
    default:
      exception=4;
      extraFlag=true;
      nInst2=nInst;
      nInst=0;
      dest=(Ptr)(&dummy);
      return 0;
    }
    break;
  }
#if HOST_ALIGN>1
  if(((short)addr&1)!=0)
    { 
      exception=3;
      extraFlag=true;
      nInst2=nInst;
      nInst=0;
      badAddress=addr;
      readOrWrite=16;
      dest=(Ptr)(&dummy);
      return 0;
    }
  addr&=ADDR_MASK_E;
#else
  addr&=ADDR_MASK;
#endif
  dest=(Ptr)theROM+addr;
  return RL((w32*)dest);
}


#define RewriteEA_b(_d_)   (*((w8*)dest)=_d_)  

#if 0
static REGP1 inline void RewriteEA_b(aw8 d) 
{
  *((w8*)dest)=d;
}
#endif


#if 1
#ifdef QM_BIG_ENDIAN
#define RewriteEA_w(_d_)    (WW((Ptr)dest,_d_))
#else /* little endian */
#define RewriteEA_w(_d_)  {if (isreg) *((w16*)dest)=_d_; \
                           else   WW((Ptr)dest,_d_);}
#endif
#else
static REGP1 inline void RewriteEA_w(aw16 d)
{  
#ifndef QM_BIG_ENDIAN
  if (isreg) *((w16*)dest)=d;
  else 
#endif
    WW((Ptr)dest,d);
}
#endif

#if 1
#ifdef QM_BIG_ENDIAN
#define RewriteEA_l(_d_)    (WL((Ptr)dest,_d_))
#else /* little endian */
#define RewriteEA_l(_d_)  {if (isreg) *((w32*)dest)=_d_; \
                           else   WL((Ptr)dest,_d_);}
#endif
#else
static REGP1 inline void RewriteEA_l(aw32 d)
{    
#ifndef QM_BIG_ENDIAN
      if (isreg) *((w32*)dest)=d;
      else
#endif 
	WL((Ptr)dest,d);

}
#endif


     /****************************************************/


#else  /* dead code right now..*/

#define RTX(val) ({rtn=val;goto retn;})
/* these are redefined as macros to allow global variable shadowing */
/* maybe the compiler can figure something out */
#define  ModifyAtEA_b(_mode_,_r_)         ({\
 __label__ retn;\
 short __x_mode=(_mode_);      \
 short __x_r=(_r_);    \
  w16 displ;\
  w32     addr;\
  uw32 rtn;\
  switch(__x_mode) \
    {\
    case 0:\
      dest=(Ptr)(&reg[__x_r])+RBO;\
      RTX(*((w8*)dest));\
    case 2:addr=aReg[__x_r];\
      break;\
    case 3:addr=aReg[__x_r]++;\
      if(__x_r==7) (*sp)++;\
      break;\
    case 4:\
      if(__x_r==7) (*sp)--;\
      addr=--aReg[__x_r];\
      break;\
    case 5:addr=aReg[__x_r]+(w16)RW(pc++);\
      break;\
    case 6:\
      displ=(w16)RW(pc++);\
      if((displ&2048)!=0) addr=reg[(displ>>12) & 15]+\
			    aReg[__x_r]+(w32)((w8)displ);\
      else addr=(w32) ((w16)(reg[(displ>>12) & 15]))+\
	     aReg[__x_r]+(w32)((w8)displ);\
      break;\
    case 7:\
      switch(__x_r) \
	{\
	case 0:addr=(w16)RW(pc++);\
	  break;\
	case 1:addr=RL((w32*)pc);\
	  pc+=2;\
	  break;\
	default:\
	  exception=4;\
	  extraFlag=true;\
	  nInst2=nInst;\
	  nInst=0;\
	  dest=(Ptr)(&dummy);\
	  RTX(0);\
	}\
      break;\
    default:\
      exception=4;\
      extraFlag=true;\
      nInst2=nInst;\
      nInst=0;\
      dest=(Ptr)(&dummy);\
      RTX(0);\
    }\
  addr&=ADDR_MASK;\
  dest=(Ptr)theROM+addr;\
  FIX_INS\
  rtn=*((w8*)dest);\
 retn: rtn;\
})

#define ModifyAtEA_w(_mode_,_r_)    ({\
  __label__ retn;\
  short __x_mode=(_mode_);\
  short __x_r=(_r_);\
  w16 displ,tmp;\
  w32     addr;\
 uw32 rtn;\
\
  if (big_endian)\
    isreg=0;\
\
  switch(__x_mode) {\
  case 0:\
    dest=(Ptr)(&reg[__x_r])+RWO;\
    if (big_endian)\
      isreg=1;\
\
    RTX(*((w16*)dest));\
  case 1:\
\
    if(big_endian)\
      isreg=1;\
\
    dest=(Ptr)(&aReg[__x_r])+RWO;\
    RTX( *((w16*)dest));\
  case 2:addr=aReg[__x_r];\
    break;\
  case 3:addr=aReg[__x_r];\
    aReg[__x_r]+=2;\
    break;\
  case 4:addr=(aReg[__x_r]-=2);\
    break;\
  case 5:addr=aReg[__x_r]+(w16)RW(pc++);\
    break;\
  case 6:\
    displ=(w16)RW(pc++);\
    if((displ&2048)!=0) addr=reg[(displ>>12) & 15]+\
			  aReg[__x_r]+(w32)((w8)displ);\
    else addr=(w32)((w16)(reg[(displ>>12) & 15]))+\
	   aReg[__x_r]+(w32)((w8)displ);\
    break;\
  case 7:\
    switch(__x_r) {\
    case 0:addr=(w16)RW(pc++);\
      break;\
    case 1:addr=RL((w32*)pc);\
      pc+=2;\
      break;\
    default:\
      exception=4;\
      extraFlag=true;\
      nInst2=nInst;\
      nInst=0;\
\
      dest=(Ptr)(&dummy);\
      RTX(0);\
    }\
    break;\
  }\
  if(((short)addr&1)!=0)\
    { \
      exception=3;\
      extraFlag=true;\
      nInst2=nInst;\
      nInst=0;\
      badAddress=addr;\
      readOrWrite=16;\
      dest=(Ptr)(&dummy);\
      RTX(0);\
    }\
\
  addr&=ADDR_MASK_E;\
  \
  dest=(Ptr)theROM+addr;\
  FIX_INS\
  RTX(RW((w16*)dest));\
  retn: rtn;\
})

#define ModifyAtEA_l(_mode_,_r_)  ({\
  __label__ retn;\
  short __x_mode=(_mode_);\
  short __x_r=(_r_);\
  w16 displ;\
  w32     addr,tmp;\
  uw32 rtn;\
  isreg=0;\
  switch(__x_mode) {\
  case 0:\
    isreg=1;\
    dest=(Ptr)(&reg[__x_r]);\
    RTX(*((w32*)dest));\
  case 1:\
    isreg=1;\
    dest=(Ptr)(&aReg[__x_r]);\
    RTX(*((w32*)dest));\
  case 2:addr=aReg[__x_r];\
    break;\
  case 3:addr=aReg[__x_r];\
    aReg[__x_r]+=4;\
    break;\
  case 4:addr=(aReg[__x_r]-=4);\
    break;\
  case 5:addr=aReg[__x_r]+(w16)RW(pc++);\
    break;\
  case 6:\
    displ=(w16)RW(pc++);\
    if((displ&2048)!=0) addr=reg[(displ>>12) & 15]+\
			  aReg[__x_r]+(w32)((w8)displ);\
    else addr=(w32)((w16)(reg[(displ>>12) & 15]))+\
	   aReg[__x_r]+(w32)((w8)displ);\
    break;\
  case 7:\
    switch(__x_r) {\
    case 0:addr=(w16)RW(pc++);\
      break;\
    case 1:addr=RL((w32*)pc);\
      pc+=2;\
      break;\
    default:\
      exception=4;\
      extraFlag=true;\
      nInst2=nInst;\
      nInst=0;\
      dest=(Ptr)(&dummy);\
      RTX( 0);\
    }\
    break;\
  }\
  if(((short)addr&1)!=0)\
    { \
      exception=3;\
      extraFlag=true;\
      nInst2=nInst;\
      nInst=0;\
      badAddress=addr;\
      readOrWrite=16;\
      dest=(Ptr)(&dummy);\
      RTX(0);\
    }\
\
  addr&=ADDR_MASK_E;\
\
  dest=(Ptr)theROM+addr;\
  FIX_INS\
  RTX(RL((w32*)dest));\
 retn: rtn;\
})

#define RewriteEA_b(d)  ({ *((w8*)dest)=d;})

#define RewriteEA_w(d) ({\
  if (!big_endian)\
    {\
      if (isreg) *((w16*)dest)=d;\
      else WW((Ptr)dest,d);\
    }\
  else WW((Ptr) dest,d); \
})

#define RewriteEA_l( d)  ({\
  if (!big_endian)\
    {\
      if (isreg) *((w32*)dest)=d;\
      else WL((Ptr)dest,d); \
    }\
  else WL((Ptr)dest,d);\
})
#endif

  

#endif
