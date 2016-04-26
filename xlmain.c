/*
 * (c) UQLX - see COPYRIGHT
 */


/*#include "QLtypes.h"*/
#include "QL68000.h"

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "QL_config.h" 
#include "QInstAddr.h"

#include "unix.h"
#include "boot.h"
#include "qx_proto.h"

static int QLdone=0;

/*extern void process_events(void);*/

extern char **argv;

int main(int ac, char **av)
{
  char *rf;
  int  rl=0;
  void *tbuff;
  int j,c;
  int mem=-1, col=-1, hog=-1;

	tbuff=rf=NULL;
	QLdone=0;
	rl=j=c=0;
	mem=col=hog=-1;
	
  argv=av;

  SetParams(ac, av);
  
  uqlxInit();
  
  QLRun();

  return 0;
}
