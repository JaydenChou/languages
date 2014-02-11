/*******************************************************************************
 *  File: stack.cpp
 *  
 *  Comments: Implementation of class stack
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

#include <iostream>
using namespace std;
#include "stack.h"
#include "util.h"


/*******************************************************************************
 *  Default constructor
 *  Comments: Initialize the stack
 ******************************************************************************/
stack::stack(int n) 
{                             
    size = n;             // Set the queue size to the specified input
    data = new int[size]; // Allocate the necessary memory
    top = -1;             // stack is empty
    unique = 1;
    sum = 0;
}

/*******************************************************************************
 *  Destructor
 *  Comments: The destructor frees the allocated memory
 ******************************************************************************/
stack::~stack() 
{               
    size = 0;
    top = -1;
    sum = 0;
    unique = 1;
    delete []data;
}


/*******************************************************************************
 *  Member Function: Push
 *  Comments: Place item onto the top of the stack
 *  Arguments: the item
 *  Return: Return 1 if successful, 0 if not, 2 if єlement is in the queue
 ******************************************************************************/
int stack::Push(int v) 
{                                     // Place item on the stack
   if (!Full())  
   {                      // Make sure stack is not full
      if (unique == 1) {
         for (int i = 0; i <= top; i++) 
         {
            // unique elements
            if (data[i] == v)
            return 2;
         }
      }
      top++;                                    // Move the top pointer
      sum += v;
      data[top] = v;                      // insert element into stack
      return 1;                     // regular termination, so return 1
   } else {
      get_memory(&data, size, size*2);
      top++;                                    // Move the top pointer
      sum += v;
      data[top] = v;                      // insert element into stack
      return 0;                        // stack was full, so return 0
   }
}


/*******************************************************************************
 *  Member Function: Exist
 *  Comments: Try to look if an array of items is in the stack
 *  Arguments: the array of items
 *  Return: Return 1 if exist, 0 if not
 ******************************************************************************/
int stack::Exist(int pi[], int psize) 
{                                     // Place item on the list
   if (psize == (top+1)) {
         // compare, easy, the same size
      if (compare(pi, data, psize) == 1) {  
         // are the same
         return 1;   // exist
      }
   } else if (psize < (top+1)) {
      // compare
      // create a tmp array
#ifdef WIN32
      int *tmpa = new int[psize];
#else
      int tmpa[psize];
#endif
      for (int from = 0; from <=((top+1)-psize); from++) {
         substr(tmpa, data, from, psize);
         if (compare(tmpa, pi, psize) == 1) {
            return 1;   // exist
         }
      }
#ifdef WIN32
      delete[] tmpa;
#endif
   }
   // Only if:
   //  - pb is not in data, is too big
   //  - pb is not here, result of successive comparation using substr
   return 0;
}

/*******************************************************************************
 *  Member Function: Push
 *  Comments: Place an array of items onto the top of the stack
 *  Arguments: the item
 *  Return: Return 1 if successful, 0 if not, 2 if єlement is in the queue
 ******************************************************************************/
int stack::Push(int pi[], int psize) 
{                                     // Place item on the list
   if (!Exist(pi, psize)) {
       for (int i = 0; i < psize; i++) {
          Push(pi[i]);
       }
   }
   return 0;
}


/*******************************************************************************
 *  Member Function: Pop
 *  Comments: Get the item at the top place on the stack
 *  Return: Return the value if successful, 0 if not
 ******************************************************************************/
int stack::Pop()
{
   if ( !Empty() ) {
      sum -= data[top];
      return data[top--];
   }
   else return -1; 
}

/*******************************************************************************
 *  Member Function: Empty
 *  Comments: to know if a stack has not elements
 *  Return: Returns 1 if stack is empty, 0 if it is not.
 ******************************************************************************/
int stack::Empty() 
{
   return (top==-1);      // If the top=-1, stack is empty.
}

/*******************************************************************************
 *  Member Function: Full
 *  Comments: to know if the stack needs more memory
 *  Return: Returns 1 if stack is full, 0 if it is not.
 ******************************************************************************/
int stack::Full() 
{
   return ((top+1)==size);   // If (top+1)=size, stack is full.
}


/*******************************************************************************
 *  Member Function: Clean
 *  Comments: reset the stack
 ******************************************************************************/
void stack::Clean() 
{                       
   top = -1;
   sum = 0;
}


/*******************************************************************************
 *  Member Operator: overloading of assignment
 *  Comments: compare the size of the stacks if needs more memory, get it.
 *  Arguments: a queue
 *  Return: Returns this
 ******************************************************************************/
stack& stack::operator = (const stack& S) {
   if (S.top < size) {
      for (int i = 0; i <= S.top; i++) {
         data[i] = S.data[i];
      }
   } else {
      get_memory(&data, size, S.size);

      memcpy(data, S.data, sizeof(int)*(S.top+1));
   }
   size = S.size;
   top = S.top;
   unique = S.unique;
   sum = S.sum;
   return(*this);
}

/*******************************************************************************
 *  Member Operator: overloading of increase operator
 *  Comments: simply add every element of the other stack in this stack
 *  Arguments: a stack
 *  Return: Returns this
 ******************************************************************************/
stack& stack::operator += (const stack& S) {
   if (unique == 1)
   {
      for (int i = 0; i <= S.top; i++)
         Push(S.data[i]);
   } else {
      if ((S.top+top+2) > size) {
         get_memory(&data, size, S.size+(size+S.top+2));
      }
      memcpy(&data[top+1], S.data, ((S.top+1)*sizeof(int)));
      top += (S.top+1);
      sum += S.sum;
   }
   return(*this);
}

