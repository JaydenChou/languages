/*******************************************************************************
 *  File: queue.h
 *  
 *  Comments: Declaration of class queue
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

#ifndef QUEUE_H
#define QUEUE_H
#define QUEUE 0x2

#include <iostream>         // Needed to overload << and >>
using namespace std;

class queue {

public:

    // Default constructor
    queue(int n = 100);

    // Destructor
    ~queue();

    // ----------------
    // Member Functions
    // ----------------
   
    // add an element
    int Push(int newItem); 

    // retrieve an element
    int Pop();

    // ask if queue is empty
    int Empty() const;

    // ask if queue is full
    int Full() const; 

    // obtain size (private member)
    int getSize() { return size; };
    // obtain tail (private member)
    int getTail() { return tail; };
    // set unique or not (private member)
    void setNoUnique() { unique = 0; };
    void setUnique() { unique = 1; };
    // Clean que queue
    void Clean();
    // obtain sum of the queue (private member)
    int getSum() { return sum; };
    // return what class is this
    int whatIAm() { return QUEUE; }; 

    // ---------
    // Operators
    // ---------
    queue& operator = (const queue& s);
    queue& operator += (const queue& s);

    friend ostream &operator<<(ostream &out, queue& S) 
    {
      for (int i = 0; i < S.tail; i++) { 
         out << S.data[i] << ' ';                  // Write each of the actual elements
      }
      return out;                              // Always return the stream
    }


protected:
    // ---------
    // atributes
    // ---------

    // The data used for the queue
    int *data;      // The queue will be stored here
    int size;       // The maximum capacity of the queue
    int tail;       // The last element of the queue
    int unique;     // if accept of not repetitive elements
    int sum;        // the sum of all elements in the queue

};
#endif // QUEUE_H
