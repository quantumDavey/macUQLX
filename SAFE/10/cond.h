/*
 * (c) UQLX - see COPYRIGHT
 */

/* Here are the inline versions of flag-testing functions*/

static inline Cond ccT(void)
{   
  return true;
}

static inline Cond ccF(void)
{    
  return false;
}

static inline Cond ccHI(void)
{   
  return !(carry||zero);
}

static inline Cond ccLS(void)
{    
  return carry||zero;
}

static inline Cond ccCC(void)
{  
  return !carry;
}

static inline Cond ccCS(void)
{     
  return carry;
}

static inline Cond ccNE(void)
{   
  return !zero;
}

static inline Cond ccEQ(void)
{    
  return zero;
}

static inline Cond ccVC(void)
{   
  return !overflow;
}

static inline Cond ccVS(void)
{   
  return overflow;
}

static inline Cond ccPL(void)
{     
  return !negative;
}

static inline Cond ccMI(void)
{    
  return negative;
}

static inline Cond ccGE(void)
{   
  return (negative==overflow);/*  return (negative&&overflow)||(!(negative||overflow));*/
}

static inline Cond ccLT(void)
{    
  return (negative!=overflow);/*  return (negative&&(!overflow))||((!negative)&&overflow);*/
}

static inline Cond ccGT(void)
{    
  return (!zero)&&(negative==overflow);/*  return (!zero)&&((negative&&overflow)||(!(negative||overflow)));*/
}

static inline Cond ccLE(void)
{ 
  return zero||(negative!=overflow);/*  return zero||(negative&&(!overflow))||((!negative)&&overflow);*/
}
