#*******************************************************************************
#   File: Tuple.mk
#   
#   Comments: makefile for GNU/make
#   
#   Compiler: tested in linux only
#      gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 (i386) - kernel 2.4
#      gcc/g++ 3.3.3 20040412 - Fedora Core 2 (i686) - kernel 2.6.10
#      gcc/g++ 3.4.3 20050227 - Fedora Core 3 (i386) - kernel 2.6.12
#      gcc/g++ 4.0.0 20050519 - Fedora Core 4 (i686) - kernel 2.6.11
#      gcc/g++ 4.0.1 20050727 - Fedora Core 4 (x86_64) - kernel 2.6.12
#   
#   History:
#      Oct-20-2005: Javier Fuchs                                   |\_/|   
#                   Phone (+54 2944) 525173                       /     \  
#                   http://southfox.servehttp.com                /_.~ ~,_\ 
#                                                                   \@/    
#*******************************************************************************

# decomment out to compile and link 32 bits in a 64 bit environment 
#####PLATFORM=32BITS


# decomment out to compile and link with debugging symbols
GDBDEBUG=-g -ggdb3


#-------------------------------------------------------------------------------
#-- Directory variables
dirExe=./
dirObj=./
dirSrc=./

#-------------------------------------------------------------------------------
#-- Compilation variables
#   -D_VERSION=$(VERSION) \

INCLUDES=-I/usr/include
COMPILE_32BITS=-m32 
CPPoptions=-Wno-deprecated -Wall $(GDBDEBUG) $(COMPILE_$(PLATFORM))
cco_first_flag=-c ${<D}/${<F} 
cco_second_flag=-o $@ 
_CPP=g++
_C_compile=$(_CPP) $(cco_first_flag) $(CPPoptions) $(Defines) $(Includes) $(cco_second_flag) 


#-------------------------------------------------------------------------------
#-- Link variables
LINK_32BITS=-milp32
LKoptionsGral=-fexternal-templates -Wall $(LINK_$(PLATFORM))
#LKdir=-L/usr/lib
LKdbg=$(GDBDEBUG)
LKoptionsExe=$(LKdbg) $(LKoptionsGral) $(LKdir)
_C_link=$(_CPP) -o $@ $(LKoptionsExe)

#-------------------------------------------------------------------------------
#-- Targets

EXEFILES=$(dirExe)TupleTestDriver $(dirExe)SetTestDriver

all	: clean $(EXEFILES)
	@ echo "Builded...!"

#-------------------------------------------------------------------------------
#-- EXE 

INCLUDEFILES = $(dirObj)Set.h \
               $(dirObj)Tuple.h \
               $(dirObj)Size.h

SOURCEFILES = $(dirObj)Utils.cpp \
              $(dirObj)Set.cpp \
              $(dirObj)Tuple.cpp

OBJFILES = $(dirObj)Utils.o \
           $(dirObj)Set.o \
           $(dirObj)Tuple.o

$(dirExe)TupleTestDriver : $(dirObj)TupleTestDriver.o $(OBJFILES)
	$(_C_link) $(dirObj)TupleTestDriver.o $(OBJFILES)

$(dirExe)SetTestDriver : $(dirObj)SetTestDriver.o $(OBJFILES)
	$(_C_link) $(dirObj)SetTestDriver.o $(OBJFILES)

#------- OBJ -------------------------------------------------------------
#--
$(dirObj)TupleTestDriver.o : $(dirSrc)TupleTestDriver.cpp $(SOURCEFILES) $(INCLUDEFILES)
	$(_C_compile) 

$(dirObj)SetTestDriver.o : $(dirSrc)SetTestDriver.cpp $(SOURCEFILES) $(INCLUDEFILES)
	$(_C_compile)

$(dirObj)Set.o : $(dirSrc)Set.cpp $(INCLUDEFILES)
	$(_C_compile) 

$(dirObj)Tuple.o : $(dirSrc)Tuple.cpp $(dirSrc)Tuple.h $(dirSrc)Size.h 
	$(_C_compile) 

$(dirObj)Utils.o : $(dirSrc)Utils.cpp
	$(_C_compile) 

#------ Clean -----------------------------------------------------------------
#--
clean	:	
	rm -f $(EXEFILES) $(OBJFILES)

