/*******************************************************************************
 *  File: queue.cpp
 *  
 *  Comments: Implementation of class queue
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
 *      Nov-16-2005: Javier Fuchs                                   |\_/|   
 *                   Phone (+54 2944) 525173                       /     \  
 *                   http://southfox.servehttp.com                /_.~ ~,_\ 
 *                                                                   \@/    
 ******************************************************************************/

#include "util.h"           // Some usefull functions
#include "queue.h"          // The definition of the queue class

/*******************************************************************************
 *  Default constructor
 *  Comments: creates a queue
 ******************************************************************************/
queue::queue(int n) 
{
    size = n;               // Set the queue size to the specified input
    data = new int[size];   // Allocate the necessary memory
    tail = 0;               // Since the first available spot is 0...
    unique = 1;
    sum = 0;
}

/*******************************************************************************
 *  Destructor
 *  Comments: clean memory
 ******************************************************************************/
queue::~queue() 
{                       // The destructor frees the allocated memory
    delete []data;      // Deallocate the dynamic memory
    data=NULL;          // To be safe, set the pointer to null
    unique = 1;
    sum = 0;
}



/*******************************************************************************
 *  Member Function: Push
 *  Comments: Place item onto the tail of the queue
 *  Arguments: the item
 *  Return: Return 1 if successful, 0 if not, 2 if єlement is in the queue
 ******************************************************************************/
int queue::Push(int v)
{  
   if (!Full() )
   {       // Make sure queue is not full
      if (unique == 1) 
      {
         for (int i = 0; i < tail; i++) 
         {
            // only unique elements
            if (data[i] == v)
            return 2;
         }
      }
      data[tail] = v;       // insert element into queue
      sum += v;
      tail++;               // increment tail pointer
      return 1;             // Item was inserted, so return 1
   } else {
      get_memory(&data, size, size*2);
      data[tail] = v;       // insert element into queue
      sum += v;
      tail++;               // increment tail pointer
      return 1;             // Item was inserted, so return 1
   }
   return 0;                // queue was full, so return 0
}


/*******************************************************************************
 *  Member Function: Pop
 *  Comments: Get the item at the first place on the queue
 *  Return: Return the value if successful, 0 if not
 ******************************************************************************/
int queue::Pop() 
{
    if (!Empty())                   // Make sure queue is not empty
    {  
       int t = data[0];             // Copy the head value
       for (int i = 0; i < tail; i++) { 
          data[i] = data[i+1];      // Shift the elements in the queue down
       }
       tail--;                      // Move the tail down by one
       sum -= t;
       return t;                    // Return the value of the head
    } else {      
       return 0;
    }           
}

/*******************************************************************************
 *  Member Function: Empty
 *  Comments: to know if a queue has not elements
 *  Return: Returns 1 if queue is empty, 0 if it is not.
 ******************************************************************************/
int queue::Empty() const 
{    
   return (tail == 0);  // If the head equals the tail the queue is empty
}


/*******************************************************************************
 *  Member Function: Full
 *  Comments: to know if the queue needs more memory
 *  Return: Returns 1 if queue is full, 0 if it is not.
 ******************************************************************************/
int queue::Full() const 
{ 
   return (tail == size);             // If tail==size, the queue is full
}


/*******************************************************************************
 *  Member Function: Clean
 *  Comments: reset the queue
 ******************************************************************************/
void queue::Clean() 
{
   tail = 0;
   sum = 0;
}

/*******************************************************************************
 *  Member Operator: overloading of assignment
 *  Comments: compare the size of the queues if needs more memory, get it.
 *  Arguments: a queue
 *  Return: Returns this
 ******************************************************************************/
queue& queue::operator = (const queue& Q) 
{
   if (size < Q.size)
      get_memory(&data, size, Q.size);

   memcpy(data, Q.data, sizeof(int)*Q.tail);
   size = Q.size;
   tail = Q.tail;
   unique = Q.unique;
   sum = Q.sum;
   return(*this);
}

/*******************************************************************************
 *  Member Operator: overloading of increase operator
 *  Comments: simply add every element of the other queue in this queue
 *  Arguments: a queue
 *  Return: Returns this
 ******************************************************************************/
queue& queue::operator += (const queue& S) {
      for (int i = 0; i < S.tail; i++) {
         Push(S.data[i]);
      }
   return(*this);
}

