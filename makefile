# (c) UQLX - see COPYRIGHT
#
# UQLX Makefile
#  Targets:
#	all: build uqlx according to ~/.uqlx_cprefs
#	xaw: 		athena widget interface version, override prefs
#       noaw:	  	without xaw interface, override prefs

# Compile Preferences File:
#  ~/.uqlx_cprefs
#
#  you may set this variables:
#    USE_XAW= yes			# or no
#    BUILD_SPECIALFLAGS= -Dxx 		# need some strange define flags ?
#    LOCAL_LIBPATHS= -L /amd/hx/lib/X11/
#    LOCAL_INCLUDES= -I /usr/include/
#    LOCAL_OPTFLAGS= -g -O3 -O0		# or whatever
#    LOCAL_DEFINES=
#    LOCAL_LINKFLAGS=


# the whole Makefile is utterly GNU Make dependent

IMPLPATH := $(shell pwd)
RELSE := $(shell date '+%m/%d/%y  %H:%M:%S')
NRELSE := $(shell date '+%d.%m.%y-%H.%M')
PWD := $(shell pwd)

-include .config

export EXACT_HW


ifndef OS2_SHELL
 XOS := $(shell uname)
 XOSREL := $(shell uname -r)
else 
 XOS := OS2
endif


# don't override CC definitions..
ifndef CC
 CC= gcc
else
# ..unless it isn't gcc
 ifeq (,$(findstring gcc,$(CC)))
   CC= gcc
   REDEF_CC= yes
 endif
endif



# get compile preferences
ifeq (0,$(MAKELEVEL))
-include $(HOME)/.uqlx_cprefs
endif

ifeq ($(USE_XAW), yes)
  XAW_FLAG= -DXAW
else
  XAW_FLAG=
endif

# now try to guess OS and correct BUILDFLAGS ....

ifeq ($(XOS),Darwin)
  BUILDFLAGS := -DUSE_IPC -DXSCREEN -DQVFS -DMOUSE -DUSE_IOSZ -DDO_GRAB -DSERIAL -DNEWSERIAL -DNEWPTY -DUX_WAIT -DFASTLOOP -DSH_MEM -DIPDEV -DBSD44  -DDARWIN -DHAS_STPCPY #-DUSE_VM
#-DDEBUG_ROM # -DBACKING_STORE
endif

ifndef GENERIC_CPU
ifeq (,$(findstring ERROR,$(DEF_CPU)))
   BUILDFLAGS += $(DEF_CPU)
endif
endif

BUILDFLAGS += $(GCC_XFLAGS)

INCLUDES := -I /usr/X11R6/include/ 
LIBS := -L/usr/X11R6/lib/

# append local definitions and preferences
BUILDFLAGS += $(BUILD_SPECIALFLAGS)
BUILDFLAGS += $(LOCAL_DEFINES)
INCLUDES += $(LOCAL_INCLUDES)
LIBS += $(LOCAL_LIBPATHS)

PROFFLAGS= #-pg -a

OPTFLAGS=  $(PROFFLAGS) -O2 $(DEBUG)  $(LOCAL_OPTFLAGS) -fomit-frame-pointer 

DEBUGFLAGS=  #-DTRACE #-DBACKTRACE  #-DDEBUG -DVTIME

LINKFLAGS= $(LOCAL_LINKFLAGS)


WFLAGS= # -Wimplicit -Wno-multichar

ENX :=  '-DIMPL="$(IMPLPATH)/"' 
ENVFLAGS := $(ENX)




XTSRCS := xql.c xtmain.c unixstuff.c xqlmouse.c
XLSRCS := x.c xlmain.c unixstuff.c xqlmouse.c
XWSRCS := x.c xql.c xtmain.c xlmain.c unixstuff.c xqlmouse.c

XTOBJS := xql.o xtmain.o xqlmouse_aw.o unixstuff_aw.o
XLOBJS := x.o xlmain.o xqlmouse.o unixstuff.o



HEADERS := QL.h  QLtypes.h  QDisk.h QL68000.h cond.h iexl.h QSerial.h  unix.h \
	QFilesPriv.h  QL_config.h  QSound.h   QInstAddr.h  QLfiles.h boot.h \
	QDOS.h trace.h uqlx_cfg.h emudisk.h driver.h vm.h \
	misdefs.h xcodes.h QL_screen.h insnx.h vm_linux.h \
	iexl_general.h memaccess.h memaccess_vm.h mmodes.h iexl_ug.h QVFS.h \
        QL_sound.h \
	QLip.h iptraps.h util.h xc68.h xipc.h script.h qx_proto.h vl.h \
        cow.h


OBJ := Init.o general.o instructions_ao.o instructions_pz.o  \
	QLtraps.o QL_hardware.o QL_config.o dummies.o vm.o \
	xqlkey.o qmtrap.o uxfile.o QL_serial.o pty.o uqlx_cfg.o\
	QL_files.o QL_driver.o QDisk.o trace.o version.o QLserio.o \
	QL_screen.o QL_poll.o xcodes.o QL_boot.o QL_basext.o \
	QL_cconv.o  iexl_general.o QVFS.o vm_linux.o QL_sound.o\
	Xscreen.o QLip.o util.o xc68.o xipc.o script.o rompatch.o \
	mach_exception.o siginfo.o vl.o ide.o block.o

#insnx.o insnx2.o

SRC := Init.c general.c instructions_ao.c instructions_pz.c   \
	QLtraps.c QL_hardware.c QL_config.c dummies.c vm.c \
	xqlkey.c qmtrap.c uxfile.c QL_serial.c pty.c uqlx_cfg.c \
	QL_files.c QL_driver.c QDisk.c trace.c version.c QLserio.c \
	QL_screen.c QL_poll.c xcodes.c QL_boot.c QL_basext.c \
	QL_cconv.c insnx.c insnx2.c iexl_general.c QVFS.c vm_linux.c \
	Xscreen.c QLip.c util.c xc68.c xipc.c script.c rompatch.c \
	QL_sound.c vm_win.c mach_exception.c siginfo.c \
	vl.c ide.c block.c


ROMS := romdir/js_rom romdir/min.189

CONFIGFILES := Makefile Xql Xqlaw 

AUX := .gdbinit BOOT.test bench1 bench2 xx.c insnx.c.old Xgui.c \
	guesscpu_linux config MK.all do_install browse_manual vmtest.c \
	zmtest.c uqlx.bat  dtest.c
UTILS := utils/README utils/xheader utils/Makefile utils/qcp.c utils/qls.c \
	utils/qxl_fschk.zip utils/tracesplit


 EXE_NAME= qm

# link in required libraries - and nothing more
ifneq (,$(findstring DXAW,$(XAW_FLAG)))
    XLIBS := -lXaw -lXmu -lXt -lX11 -lXext
else
  ifneq (,$(findstring DSH_MEM,$(BUILDFLAGS)))
    XLIBS :=  -lX11 -lXext
  else
    XLIBS := -lX11
  endif
endif


   WINSRCS := $(XLSRCS)
   WINOBJS := $(XLOBJS)


ifneq (,$(findstring $(DEBUG_ON),$(DEBUG_FILES)))
  DO_DEBUG=true
else 
  DO_DEBUG=false
endif


# export ALL variables (lazy typing)
export

all: printarch qm


noaw:
	$(MAKE) all USE_XAW="no"

xaw:  
	$(MAKE) all USE_XAW="yes"	

##exe:	$(EXE_NAME) qm

.PHONY: printarch
printarch:
	@echo ""
ifeq (,$(UNKNOWN_ARCH))
	@echo "*** Making UQLX for $(XOS) ***" 
ifneq (,$(findstring ERROR,$(DEF_CPU)))
	@echo "couldn't set CPU flags, guesscpu_* returned $(DEF_CPU)"
else
	@echo "$(CC) compiling with $(DEF_CPU) flag"
endif
ifneq (,$(REDEF_CC))
	@echo "Warning: CC redefined to $(CC)"
endif
else
	@echo "*** Wanrning: unknown architecture $(XOS) ***"
endif
	@echo

.PHONY: print_usage
print_usage:
	@echo "Please do 'make config' first"

.PHONY: config
config:

	./config >.config

xqlmouse.o:	xqlmouse.c
	$(CC) $(PROFFLAGS)  -c -O3 $(ENVFLAGS) $(INCLUDES) $(OPTFLAGS) $(BUILDFLAGS) $(WFLAGS) $(XAW_FLAG) $<

xqlmouse_aw.o:	xqlmouse.c
	$(CC) $(PROFFLAGS)  -c -O3 -o xqlmouse_aw.o $(ENVFLAGS) $(INCLUDES) $(OPTFLAGS) $(BUILDFLAGS) $(WFLAGS) $(XAW_FLAG) $<

unixstuff.o:	unixstuff.c
	$(CC) $(PROFFLAGS)  -c  $(DEBUGFLAGS) $(BUILDFLAGS) $(INCLUDES) $(OPTFLAGS)  $(ENVFLAGS) $(WFLAGS) $(XAW_FLAG) $<

unixstuff_aw.o:	unixstuff.c
	$(CC) $(PROFFLAGS)  -c -o unixstuff_aw.o $(DEBUGFLAGS) $(BUILDFLAGS) $(INCLUDES) $(OPTFLAGS) $(ENVFLAGS) $(WFLAGS) $(XAW_FLAG) $<


# place exotic optimisation flags here:
XOPTS=-fcprop-registers -fcrossjumping -fgcse -fgcse-after-reload -foptimize-register-move 

general.o:	general.c instructions_ao.c instructions_pz.c
	$(CC) $(PROFFLAGS)  -c  $(XOPTS) -fexpensive-optimizations -fschedule-insns2  \
	-frerun-cse-after-loop \
		$(OPTFLAGS) $(DEBUGFLAGS) $(BUILDFLAGS) $(INCLUDES) $(WFLAGS) \
	 $<  

iexl_general.o:	iexl_general.c instructions_ao.c instructions_pz.c
	$(CC) $(PROFFLAGS)  -c  $(XOPTS) -fexpensive-optimizations  -fschedule-insns2  \
	-frerun-cse-after-loop \
		$(OPTFLAGS) $(DEBUGFLAGS) $(BUILDFLAGS) $(INCLUDES) $(WFLAGS) \
	 $<  



.c.o:
  ifdef DEBUG_ON
    ifneq (,$(findstring true,$(DO_DEBUG)))
	$(CC) $(PROFFLAGS)  -c  -g $(INCLUDES) $(DEBUGFLAGS) $(BUILDFLAGS) $(ENVFLAGS)  $(WFLAGS) $(XAW_FLAG) $<
    else
	$(CC) $(PROFFLAGS)  -c  $(INCLUDES) $(DEBUGFLAGS) $(OPTFLAGS) $(BUILDFLAGS) $(ENVFLAGS)  $(WFLAGS) $(XAW_FLAG) $<
    endif
  else
   ifneq (,$(strip $(DEBUG_FILES) ))
	$(MAKE) $@ DEBUG_ON=$< DEBUG_TARGET=$@
   else
	$(CC) $(PROFFLAGS)  -c  $(INCLUDES) $(DEBUGFLAGS) $(OPTFLAGS) $(BUILDFLAGS) $(ENVFLAGS)  $(WFLAGS) $(XAW_FLAG) $<
  endif
  endif




qm:     $(OBJ) $(WINOBJS)
	$(CC)  $(PROFFLAGS)  $(LINKFLAGS) $(LIBS) -o $(EXE_NAME)  $(OBJ) $(WINOBJS)  $(XLIBS) $(XLFLAG) 
  ifndef UNKNOWN_ARCH
	@echo
	@echo "*** you have almost done it, ***"
	@echo "the name of the executable is"
	@echo "     >>>  $(EXE_NAME)  <<<"
	@echo 
	@echo "next check/set PREFIX and"
	@echo "'make install'"
	@echo
	@echo "'make gui' will create a simple GUI"
	@echo
  else
	@echo
	@echo "so far everything seems to have worked"
	@echo "- you might want to try out some of the"
	@echo "more difficult BUILDFLAG options"
  endif

Xgui:	util.o Xgui.c
	gcc -o Xgui $(INCLUDES) $(CCFLAGS) $(ENVFLAGS) $(BUILDFLAGS) $(OPTFLAGS) Xgui.c util.o $(LIBS) \
		-lXaw -lXmu -lXt -lXext -lX11 $(XLFLAG) 

gui:	Xgui

README.html: README.aft
	-aft README.aft
	-aft --type=rtf README.aft

README: README.html
	-lynx -dump README.html >README
	-Ted --saveTo README.rtf README.txt

.PHONY : version
version:
	- rm -f version.c
	echo '#include "QL68000.h"' >> version.c
	echo 'char *release="$(RELSE)";' >> version.c
	echo $(RELSE) > $(HOME)/.uqlx_version

.PHONY : clean
clean:
	- rm -f $(OBJ) $(XTOBJS) $(XLOBJS) .config .gcc_version qm
