/*******************************************************************************
 *  File: util.h
 *  
 *  Comments: header of some exported functions.
 *  
 *  Compiler: 
 *     gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 (i386) - kernel 2.4
 *     gcc/g++ 3.3.3 20040412 - Fedora Core 2 (i686) - kernel 2.6.10
 *     gcc/g++ 3.4.3 20050227 - Fedora Core 3 (i386) - kernel 2.6.12
 *     gcc/g++ 4.0.0 20050519 - Fedora Core 4 (i686) - kernel 2.6.11
 *     gcc/g++ 4.0.1 20050727 - Fedora Core 4 (x86_64) - kernel 2.6.12
 *     WIN32: Visual Studio 6.0 (MS Windows 2000)
 *  
 *  History:
 *     Nov-16-2005: Javier Fuchs                                   |\_/|   
 *                  Phone (+54 2944) 525173                       /     \  
 *                  http://southfox.servehttp.com                /_.~ ~,_\ 
 *                                                                  \@/    
 ******************************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include "stack.h"

// sort an array of integers
void bublesort(int* a, int n);

// extracts between pfrom and (pfrom + psize) integers from array ps, and put
// this info in pa
void substr(int pa[], int ps[], int pfrom, int psize);

// copy the integers from array ps into pa
void copyarray(int pa[], int ps[], int psize);

// compare two arrays of integers of the same size
int compare(int pa[], int pb[], int psize);

// copy an array of integers into a stack.
int copyarray(stack &spl, int ps[], int psize);

// calculates the sum of the array of integers
int sumarray(int *parray, int psize);

// print to standard error the array of integers
void printarrayerr(int *parray, int psize);

// concat array pa and pb into pc, return the total of integers
int concat(int pc[], int pa[], int pasize, int pb[], int pbsize);

// ask for more memory 
void get_memory(int** memoryaddress, int& size, int newsize);

// sets an initial time
void starttimer();

// sets a final time
void endtimer();

// obtains the difference in seconds.microseconds
float get_elapsed_time();

#endif // UTIL_H
