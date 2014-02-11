/******************************************************************************
 *  File: Utils.cpp
 *  
 *  Comments: some usefull functions
 *  
 *  Compiler: tested in linux only
 *     gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 (i386) - kernel 2.4
 *     gcc/g++ 3.3.3 20040412 - Fedora Core 2 (i686) - kernel 2.6.10
 *     gcc/g++ 3.4.3 20050227 - Fedora Core 3 (i386) - kernel 2.6.12
 *     gcc/g++ 4.0.0 20050519 - Fedora Core 4 (i686) - kernel 2.6.11
 *     gcc/g++ 4.0.1 20050727 - Fedora Core 4 (x86_64) - kernel 2.6.12
 *  
 *  History:
 *     Oct-20-2005: Javier Fuchs                                   |\_/|   
 *                  Phone (+54 2944) 525173                       /     \  
 *                  http://southfox.servehttp.com                /_.~ ~,_\ 
 *                                                                  \@/    
 ******************************************************************************/

#include <limits.h>
#include <assert.h>

/*******************************************************************************
 *  Function: intPower
 *  Scope: Global
 *  Comments: power function: [y]=[x]^[n]. 
 *            If n is 0, no loop executes. Only works for n>=0.
 *            If n > 0, y = x*x...*x (n times)
 *  Arguments: 2
 *  Return: returns the value of [x]^[n]. 
 *          Between [1 - INT_MAX], INT_MAX = 2147483647
 ******************************************************************************/

int intPower(int x, int n)
{
   // negatives are not contempled
   assert(n >= 0);
   // unsigned int's has more capacity: [0 - UINT_MAX], UINT_MAX = 4294967295U
   unsigned int y = 1;
   // if n > 0, y = x*x...*x (n times)
   for (int i = 0; i < n; i++) {
      y *= x;
      // number too big
      assert(y < INT_MAX);
   }
   // return a signed int
   return ((int)y);
}
