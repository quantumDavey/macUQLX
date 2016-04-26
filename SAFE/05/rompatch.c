/*
 * (c) UQLX - see COPYRIGHT
 */

/* read list of patches from patchfile and apply */

#include "QL68000.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#include <fcntl.h> 
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#include "xcodes.h"  
#include "QL_config.h" 
#include "QInstAddr.h"
#include "unix.h"
#include "qx_proto.h"

void instrumentCode()
{
}