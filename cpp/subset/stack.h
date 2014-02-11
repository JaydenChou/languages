/*******************************************************************************
 *  File: stack.h
 *  
 *  Comments: Declaration of class stack
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

#ifndef STACK_H
#define STACK_H 
#define STACK 0x3
#include <iostream>
using namespace std;

class stack 
{
public:

    // Default constructor
    stack(int n = 100);               // The constructor initialize the stack
    // Destructor
    ~stack();                       // The destructor frees the allocated memory

    // ----------------
    // Member Functions
    // ----------------
   
    int Push(int v);                    // Place item on the stack
    int Push(int pi[], int psize);      // push an array
    int Exist(int pi[], int psize);     // look at the array if exist in the stack
    int Pop();                          // Remove top of the stack and return its value 
    int Empty();                        // Return 1 if empty, 0 otherwise
    int Full();                         // Return 1 if full, 0 otherwise
    void Clean();                       // Clean que stack
    int getSum() { return sum; };       // obtain sum of the stack (private member)
    int getSize() { return size; };     // obtain size (private member)
    int getTop() { return top; };       // obtain top of the stack (private member)
    int whatIAm() { return STACK; };    // return what class is this

    // set unique or not (private member)
    void setNoUnique() { unique = 0; };
    void setUnique() { unique = 1; };

    // ---------
    // Operators
    // ---------
    stack& operator = (const stack& s);
    stack& operator += (const stack& s);
    friend ostream &operator<<(ostream &out, stack& S) {
       for (int i = S.top; i >= 0; i--) {
          out << S.data[i] << ' ';       // Write each of the actual elements
       }
       return out;                                // Always return the stream
    }


private:
    // ---------
    // atributes
    // ---------

    int *data;          // The stack elements will be stored here
    int size;           // The capacity of the stack
    int top;            // The top of the stack
    int unique;         // if accept of not repetitive elements
    int sum;            // the sum of all elements in the queue

};
#endif // STACK_H
