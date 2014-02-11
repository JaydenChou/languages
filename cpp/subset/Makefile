#******************************************************************************
#  File: Makefile
#  
#  Comments: The focus of this project is recursion, stacks, and queues.
#  
#  Compiler: 
#     gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 (i386) - kernel 2.4
#     gcc/g++ 3.3.3 20040412 - Fedora Core 2 (i686) - kernel 2.6.10
#     gcc/g++ 3.4.3 20050227 - Fedora Core 3 (i386) - kernel 2.6.12
#     gcc/g++ 4.0.0 20050519 - Fedora Core 4 (i686) - kernel 2.6.11
#     gcc/g++ 4.0.1 20050727 - Fedora Core 4 (x86_64) - kernel 2.6.12
#     WIN32: Visual Studio 6.0 (MS Windows 2000) - use test.dsp for build it
#  
#  History:
#      Nov-16-2005: Javier Fuchs                                   |\_/|   
#                   Phone (+54 2944) 525173                       /     \  
#                   http://southfox.servehttp.com                /_.~ ~,_\ 
#                                                                   \@/    
#*****************************************************************************/

CFLAGS= -Wall -O3 -g -ggdb -Wno-deprecated 
CC = g++


test: test.o subset.o queue.o stack.o util.o
	$(CC) $(CFLAGS) test.o subset.o queue.o stack.o util.o -o test

test.o: test.cpp subset.h
	$(CC) -c $(CFLAGS) test.cpp

subset.o: subset.cpp subset.h queue.h stack.h util.h
	$(CC) -c $(CFLAGS) subset.cpp

util.o: util.cpp util.h
	$(CC) -c $(CFLAGS) util.cpp

queue.o: queue.cpp queue.h
	$(CC) -c $(CFLAGS) queue.cpp

stack.o: stack.cpp stack.h
	$(CC) -c $(CFLAGS) stack.cpp

clean: 
	rm -f test *.o
