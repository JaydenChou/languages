/*******************************************************************************
 *  File: util.cpp
 *  
 *  Comments: some usefull functions.
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

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <time.h>
#include "util.h"


/*******************************************************************************
 *  Global data
 *  begincalc and endcalc are used globally to calculate elapsed times.
 ******************************************************************************/
#ifdef WIN32_
struct timeval {
	long tv_sec;	/* seconds */
	long tv_usec;	/* microseconds */
};
#endif

struct timeval begincalc;
struct timeval endcalc;


/*******************************************************************************
 *  Function: bublesort
 *  Comments: sort an array of integers using buble sorting. Modifies array a
 *  Arguments: (int*) a, pointer to array of integers
 *             (int) n, size of the array
 ******************************************************************************/
void bublesort(int* a, int n)
{
   for (int i=1;i<n;i++)
      for (int j=1;j<=n-i;j++)
         if (a[j-1]>a[j]) swap(a[j-1],a[j]);
}

/*******************************************************************************
 *  Function: substr
 *  Comments: extracts between pfrom and (pfrom + psize) integers from array 
 *            ps, and put this info in pa
 *  Arguments: (int[]) pa, array of integers, the target
 *             (int[]) pb, array of integers, the source
 *             (int) pfrom, position to start the copy
 *             (int) psize, position to end the copy
 ******************************************************************************/
void substr(int pa[], int ps[], int pfrom, int psize) {
   for (int i = pfrom, j = 0; i < (pfrom+psize); i++, j++) {
      pa[j] = ps[i];
   }
}


/*******************************************************************************
 *  Function: substr
 *  Comments: compare two arrays of integers of the same size
 *  Arguments: (int[]) pa, array of integers
 *             (int[]) pb, array of integers
 *  Return: 1 if are equals, 0 if not
 ******************************************************************************/
int compare(int pa[], int pb[], int psize) {
   for (int i = 0; i < psize; i++) {
      if (pa[i] != pb[i]) { 
         return 0;
      }
   }
   return 1;
}


/*******************************************************************************
 *  Function: copyarray
 *  Comments: copy the integers from array ps into pa
 *  Arguments: (int[]) pa, array of integers, target
 *             (int[]) pb, array of integers, source
 ******************************************************************************/
void copyarray(int pa[], int ps[], int psize) {
   for (int i = 0; i < psize; i++) {
      pa[i] = ps[i];
   }
}

/*******************************************************************************
 *  Function: copyarray
 *  Comments: copy an array of integers into a stack, calling the method Push()
 *  Arguments: (stack&) spl, reference to an stack of integers
 *             (int[]) ps, array of integers, source
 *  Return: what Push() returns.
 ******************************************************************************/
int copyarray(stack &spl, int ps[], int psize) {
   return spl.Push(ps, psize);
}

/*******************************************************************************
 *  Function: sumarray
 *  Comments: calculates the sum of the array of integers
 *  Arguments: (int[]) pa, array of integers
 *             (int) psize, qtty of elements to sum
 *  Return: the sum
 ******************************************************************************/
int sumarray(int *parray, int psize) {
   int lsum = 0;
   for (int i = 0; i < psize; i++)
   {
      if (parray[i] == -1)
         break;
      if (parray[i] == 0)
         break;
      lsum+= parray[i];
   }
   return lsum;
}

/*******************************************************************************
 *  Function: printarrayerr
 *  Comments: print to standard error the array of integers
 *  Arguments: (int[]) parray, array of integers
 *             (int) psize, qtty of elements to print
 ******************************************************************************/
void printarrayerr(int *parray, int psize) {
   for (int i = 0; i < psize; i++)
   {
      if (parray[i] == -1)
         break;
      if (parray[i] == 0)
         break;
      cerr << parray[i] << ' ';
   }
}


/*******************************************************************************
 *  Function: concat
 *  Comments: concat array pa and pb into pc
 *  Arguments: (int[]) pc, array of integers, target
 *             (int[]) pa, array of integers, source
 *             (int) pasize, qtty of elements of array pa
 *             (int[]) pb, array of integers, source
 *             (int) pbsize, qtty of elements of array pb
 *  Return: return the total of integers concatenated
 ******************************************************************************/
int concat(int pc[], int pa[], int pasize, int pb[], int pbsize) {
   int ltotal = pasize+pbsize;
   int li = 0;
   for (; li < pasize; li++) {
      pc[li] = pa[li];
   }
   for (int lj = 0; li < ltotal; li++, lj++) {
      pc[li] = pb[lj];
   }
   return ltotal;
}

/*******************************************************************************
 *  Function: get_memory
 *  Comments: ask for more memory 
 *  Arguments: (int**) memoryaddress, pointer to a pointer of memory of 
 *                                    integers
 *             (int&) size, reference to the actual size the array, allocated
 *                          in memory
 *             (int) newsize, the new desired size
 ******************************************************************************/
void get_memory(int** memoryaddress, int& size, int newsize)
{
   int *tmp = new int[newsize];
   memcpy(tmp, *memoryaddress, sizeof(int)*(size));
   int *memorytodelete = *memoryaddress;
   delete[] memorytodelete;
   *memoryaddress = tmp;
   size = newsize;
}

/*******************************************************************************
 *  Function: starttimer
 *  Comments: sets an initial time
 ******************************************************************************/


#ifdef WIN32
void gettimeofday(struct timeval* actualtime, struct timezone *tz) 
{
    SYSTEMTIME lSystemTime;
    // The GetSystemTime function retrieves the current system date and time.
    // The system time is expressed in Coordinated Universal Time (UTC).

    GetLocalTime(&lSystemTime);

    actualtime->tv_sec = (lSystemTime.wSecond + 
                          lSystemTime.wMinute * 60 +
                          lSystemTime.wHour * 60 * 60);
    actualtime->tv_usec = lSystemTime.wMilliseconds * 100;
}
#endif


/*******************************************************************************
 *  Function: starttimer
 *  Comments: sets an initial time
 ******************************************************************************/
void starttimer() 
{
   gettimeofday(&begincalc, NULL);
}

/*******************************************************************************
 *  Function: endtimer
 *  Comments: sets a final time
 ******************************************************************************/
void endtimer()
{
   gettimeofday(&endcalc, NULL);
}

/*******************************************************************************
 *  Function: get_elapsed_time
 *  Comments: obtains the difference in seconds.microseconds
 *  Return: the difference
 ******************************************************************************/
float get_elapsed_time()
{
   float timing;
   long seconds = endcalc.tv_sec - begincalc.tv_sec;
   long microseconds = 0;
   if (endcalc.tv_usec >= begincalc.tv_usec)
      microseconds = endcalc.tv_usec - begincalc.tv_usec;
   else {
      seconds--;
      microseconds = 100000 + endcalc.tv_usec - begincalc.tv_usec;
   }
   if (seconds == 0)
      microseconds = endcalc.tv_usec - begincalc.tv_usec;
   
   begincalc.tv_usec = begincalc.tv_sec = endcalc.tv_usec = endcalc.tv_sec = 0;

   timing = (float)seconds + (float)microseconds/100000;

   return timing;
}
