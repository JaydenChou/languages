/*******************************************************************************
 *  File: subset.h
 *  
 *  Comments: Declaration of class subset. See explanation in README file
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

#ifndef SUBSET_H
#define SUBSET_H

#include "queue.h"
#include "stack.h"
#include "util.h"


#include <iostream>
using namespace std;

#define RECURSION 0x1

/*******************************************************************************
 * A subset class.
 * The subset sum problem is defined as follows. 
 * In this problem, one is given a set S of n integers and another integer k. 
 * The goal is to find a subset T of S such that the sum of the integers of 
 * T exactly equals k (note that since the solution T is a set, no elements 
 * from S may be repeated in a solution). 
 ******************************************************************************/
class subset {

public:

    subset(int n=10);       // The constructor
    ~subset();              // The destructor

    // The methods to implement the three solutions to this problem.
    // The first use recursion, the second use a stack, and the third 
    // use a queue. 
    // This last part (storing) is handled by the recursion, stack, or queue. 
    // k is an integer that is the "target" of the sum. 

    int *find_solution_recursive(int k); // returns first solution 
                                         // it finds recursively

    int *find_solution_stack(int k);     // returns first solution it finds
                                         // iteratively, using a stack

    int *find_solution_queue(int k);     // returns first solution it finds
                                         // iteratively, using a queue


    void print();                        // prints the last solution found
    // prints all posible solutions
    void print_all(int typeofsolution);

    template <class adaptor>
    int print_elements(adaptor* S);

    template <class adaptor>
    int search(adaptor* adaptorResult, adaptor& Q, int k);

    template <class adaptor>
    int *to_integer(adaptor& Q);

    int recursive_combination(int array[], int arraylength, int k = 0, int pre[] = 0, int prelength = 0);
    void create_recursive_solutions(int k = 0);

    template <class adaptor>
    void create_solutions(adaptor* SRR, int k = 0);

    // out overloaded operators, used to write the elements of subset
    // this are friends
    friend ostream& operator << (ostream& out, subset& S) {
       for (int i = 0; i < S.size; i++) {
         out << S.data[i] << " ";       // Write each of the actual elements
       }

      return out;                                // Always return the stream
    }

private:                                          // The data and some private methods

    // The data used for the subset
    //
    int *data;   // The queue will be stored here
    int size;    // The maximum capacity of the subset
    int last;    // The last element of the subset
    int total;   // The sum of all elements
    int *solution;
    int howmany;
    int lastsolve;

    stack *stackSolutionFinded;
    int stackSolutionFindedOk;
    int stackSolutionFindedQtty;

    queue *queueSolutionFinded;
    int queueSolutionFindedOk;
    int queueSolutionFindedQtty;

    stack *recursiveSolutionFinded;
    int recursiveSolutionFindedOk;
    int recursiveSolutionFindedQtty;

    float elapsed_time[4];
};
#endif // SUBSET_H
