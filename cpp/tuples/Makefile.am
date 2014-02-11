#*******************************************************************************
#   File: Makefile.am
#   
#   Comments: makefile
#   
#   Compiler: tested in linux only
#      gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 (i386) - kernel 2.4
#      gcc/g++ 3.3.3 20040412 - Fedora Core 2 (i686) - kernel 2.6.10
#      gcc/g++ 3.4.3 20050227 - Fedora Core 3 (i386) - kernel 2.6.12
#      gcc/g++ 4.0.0 20050519 - Fedora Core 4 (i686) - kernel 2.6.11
#      gcc/g++ 4.0.1 20050727 - Fedora Core 4 (x86_64) - kernel 2.6.12
#   
#   History:
#       Oct-20-2005: Javier Fuchs                                   |\_/|   
#                    Phone (+54 2944) 525173                       /     \  
#                    http://southfox.servehttp.com                /_.~ ~,_\ 
#                                                                    \@/    
#*******************************************************************************

bin_PROGRAMS = TupleTestDriver SetTestDriver

TupleTestDriver_SOURCES = TupleTestDriver.cpp Utils.cpp Set.cpp Set.h Tuple.cpp Tuple.h Size.h

SetTestDriver_SOURCES = SetTestDriver.cpp Utils.cpp Set.cpp Set.h Tuple.cpp Tuple.h Size.h

# decoment out to compile 32 bits in a 64 bit environment 
#AM_CXXFLAGS=-D_VERSION=$(VERSION) -Wno-deprecated -g -ggdb -m32 
#AM_LDFLAGS=-milp32

AM_CXXFLAGS=-D_VERSION=$(VERSION) -Wno-deprecated -g -ggdb 
#-Wall

INCLUDES=-I/usr/include

