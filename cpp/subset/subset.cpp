/*******************************************************************************
 *  File: subset.h
 *  
 *  Comments: Implementation of class subset. See explanation in README file
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

#include "queue.h"
#include "stack.h"
#include "subset.h"                   // The definition of the subset class
#include "util.h"


#ifdef OK
#undef OK
#endif
#define OK  0

#ifdef ERR
#undef ERR
#endif
#define ERR 1


/*******************************************************************************
 *  Default constructor
 *  Comments: creates a subset
 *  A subset of n elements from 1 to n has been created. 
 *  The default value for n is 10. n is the size of the set
 ******************************************************************************/
subset::subset(int n)
{
   size = n;              // Set the subset size to the specified input
   data = new int[n];     // Allocate the necessary memory
   total = 0;
   for (int i = 0; i < n; i++) {
      data[i] = i+1;
      total += data[i];
   }
   last = data[n-1];      // Since the first available spot is 0...
   solution = NULL;
   recursiveSolutionFinded = new stack[total];
   recursiveSolutionFindedOk = 0;
   recursiveSolutionFindedQtty = 0;
   stackSolutionFinded = new stack[total];
   stackSolutionFindedOk = 0;
   stackSolutionFindedQtty = 0;
   queueSolutionFinded = new queue[total];
   queueSolutionFindedOk = 0;
   queueSolutionFindedQtty = 0;
}


/*******************************************************************************
 *  Destructor
 *  Comments: clean memory
 *  The memory used for the subset is deallocated, subset is set to null.
 ******************************************************************************/
subset::~subset() 
{  // The destructor frees the allocated memory
   // Deallocate the dynamic memory
   delete []data;      
   delete []stackSolutionFinded;
   delete []recursiveSolutionFinded;
   delete []queueSolutionFinded;
   recursiveSolutionFindedOk = 0;
   stackSolutionFindedOk = 0;
   queueSolutionFindedOk = 0;
   data=NULL;       // To be safe, set the pointer to null
   total = 0;
   last = 0;
}



/*******************************************************************************
 *  Member Function: find_solution_recursive
 *  Arguments: (int) k is an integer that is the "target" of the sum. 
 *  Comments: returns first solution, it finds recursively
 ******************************************************************************/
int *subset::find_solution_recursive(int k)
{
   int find = ERR;
   int eureka = ERR;
   int *T = NULL;     // result
   cout << endl << "Searching for k = " << k << endl;
   if (k <= 0 || k > total) {
      cout << "Bad: k " << k << " out of range" << endl;
      solution = T = NULL;
      howmany = 0;
   } else if (k == total) {
      cout << "Easy: k " << k << " is the sum of "
           << (*this) << endl;
      T = data;
      solution = T;
      howmany = size;
   } else {
      stack L(total);
      if (recursiveSolutionFindedOk == 0) {
         starttimer();
         create_recursive_solutions(k); // only k
         endtimer();
         find = OK;
      }
      eureka = search(recursiveSolutionFinded, L, k);
      if (eureka == OK) {
         cout << endl << "Finded." << endl;
         if (!L.Empty())
            solution = T = to_integer(L);
      } else {
         cout << endl << "not found!" << endl;
      }
   }
   elapsed_time[RECURSION] = get_elapsed_time();
   cout << "The operation takes " << elapsed_time[RECURSION] << " seconds."<< endl;
   return T;
}

/*******************************************************************************
 *  Member Function: find_solution_stack
 *  Arguments: (int) k is an integer that is the "target" of the sum. 
 *  Comments: returns first solution it finds, iteratively, using a stack
 ******************************************************************************/
int *subset::find_solution_stack(int k)
{
   int find = ERR;
   int eureka = ERR;
   int *T = NULL;     // result
   cout << endl << "Searching for k = " << k << endl;
   starttimer();
   if (k <= 0 || k > total) {
      cout << "Bad: k " << k << " out of range" << endl;
      solution = T = NULL;
      howmany = 0;
   } else if (k == total) {
      cout << "Easy: k " << k << " is the sum of "
           << (*this) << endl;
      T = data;
      solution = T;
      howmany = size;
   } else {
      stack S(total);
      if (stackSolutionFindedOk == 0) {
         starttimer();
         create_solutions(stackSolutionFinded, k); // only k
         endtimer();
         find = OK;
      }
      eureka = search(stackSolutionFinded, S, k);
      if (eureka == OK) {
         cout << endl << "Finded." << endl;
         if (!S.Empty())
            solution = T = to_integer(S);
      } else {
         cout << endl << "not found!" << endl;
      }
   }
   if (find == OK)
      cout << "The operation takes " << get_elapsed_time() << " seconds."<< endl;
   return T;
}

/*******************************************************************************
 *  Member Function: find_solution_queue
 *  Arguments: (int) k is an integer that is the "target" of the sum. 
 *  Comments: returns first solution it finds, iteratively, using a queue
 ******************************************************************************/
int *subset::find_solution_queue(int k)    
{
   int find = ERR;
   int eureka = ERR;
   int *T = NULL;     // result
   cout << endl << "Searching for k = " << k << endl;
   if (k <= 0 || k > total) {
      cout << "Bad: k " << k << " out of range" << endl;
      solution = T = NULL;
      howmany = 0;
   } else if (k == total) {
      cout << "Easy: k " << k << " is the sum of " 
           << (*this) << endl;
      T = data;
      solution = T;
      howmany = size;
   } else {
      queue Q(total);
      if (queueSolutionFindedOk == 0) {
         starttimer();
         create_solutions(queueSolutionFinded, k);
         endtimer();
         find = OK;
      }
      eureka = search(queueSolutionFinded, Q, k);
      if (eureka == OK) {
         cout << endl << "Finded." << endl;
         if (!Q.Empty())
            solution = T = to_integer(Q);
      } else {
         cout << endl << "not found!" << endl;
      }
   }
   if (find == OK)
      cout << "The operation takes " << get_elapsed_time() << " seconds."<< endl;
   return T;
}

/*******************************************************************************
 *  Member Function: print
 *  Comments: prints the last solution found
 ******************************************************************************/
void subset::print()
{
   for (int i = 0; i < howmany; i++)
     cout << solution[i] << ' ';
}


/*******************************************************************************
 *  Member Function: to_integer
 *  Arguments: (adaptor&) Q
 *  Comments: converts the adaptor type (a queue or stack), to an array of
 *            integers
 ******************************************************************************/
template <class adaptor>
int * subset::to_integer(adaptor& Q)
{
   int *T = NULL;
   T = new int[Q.getSize()+1];
   int i = 0;
   while (!Q.Empty()) {
      int value = Q.Pop();
      T[i++] = value;
   }
   howmany = i;
   T[i] = 0;
   return T;
}



/*******************************************************************************
 *  Member Function: search
 *  Arguments: (adaptor&) adaptorResult, all the results
 *             (adaptor&) Q, the target adaptor, where the result is copied
 *             (int) k, is an integer that is the "target" of the sum. 
 *  Comments: converts the elements from adaptorResult to Q when the sum of the
 *            elements is equal k. The adaptor type is a queue or stack.
 ******************************************************************************/
template <class adaptor>
int subset::search(adaptor* adaptorResult, adaptor& Q, int k) 
{
   int eureka = ERR;
   if (k < 1 || k > total) 
      return eureka;
   
   // is empty
   if (adaptorResult[k-1].Empty())
      return eureka;
   eureka = OK;

   adaptor tmp(adaptorResult[k-1].getSize());
   tmp.Clean();
   tmp = adaptorResult[k-1];

   int stotal = 0;
   cout << endl 
        << "Result: k = " << k 
        << " is generated by ";

   while (!tmp.Empty()) {
      int value = tmp.Pop();
      Q.Push(value);
      stotal+=value;
      cout << value << " ";
      if (stotal == k) {
          break;
      }
   }
   cout << endl;
   return eureka;
}


/*******************************************************************************
 *  Member Function: print_all
 *  Arguments: (int) typeofsolution (STACK QUEUE or RECURSION)
 *  Comments: call functions to calculate and print out every solution with 
 *            its elements
 ******************************************************************************/
void subset::print_all(int typeofsolution)
{
   int solutions = 0;
   char answer='x'; 
   switch (typeofsolution) {
    case RECURSION:
      cout << "Using a RECURSION" << endl;
      if (recursiveSolutionFindedOk == 0) {
         starttimer();
         create_recursive_solutions();
         endtimer();
         elapsed_time[typeofsolution] = get_elapsed_time();
         recursiveSolutionFindedOk = 1;
      }
      break;
    case QUEUE:
      cout << "Using a QUEUE" << endl;
      if (queueSolutionFindedOk == 0) {
         starttimer();
         create_solutions(queueSolutionFinded);
         endtimer();
         elapsed_time[typeofsolution] = get_elapsed_time();
         queueSolutionFindedOk = 1;
      }
      break;
    case STACK:
    default:
      cout << "Using a STACK" << endl;
      if (stackSolutionFindedOk == 0) {
         starttimer();
         create_solutions(stackSolutionFinded);
         endtimer();
         elapsed_time[typeofsolution] = get_elapsed_time();
         stackSolutionFindedOk = 1;
      }
      break;
   }
   if (size > 6) {
      do {
         cout << "There are " << total << " results and ";
         switch (typeofsolution) {
          case RECURSION:
            cout << recursiveSolutionFindedQtty << endl;
            break;
          case QUEUE:
            cout << queueSolutionFindedQtty << endl;
            break;
          case STACK:
          default:
            cout << stackSolutionFindedQtty << endl;
            break;
         }
         cout << " solutions." << endl
              << "Do you want to see them? (y/n): " << endl;
         cin >> answer;
      } while (answer != 'y' && answer != 'n');
   } else {
      answer = 'y';
   }
   if (answer == 'y')
      switch (typeofsolution) {
       case RECURSION:
         solutions = print_elements(recursiveSolutionFinded);
         break;
       case QUEUE:
         solutions = print_elements(queueSolutionFinded);
         break;
       case STACK:
       default:
         solutions = print_elements(stackSolutionFinded);
         break;
      }

   switch (typeofsolution) {
    case RECURSION:
      cout << "RECURSION used in this calc." << endl;
      break;
    case QUEUE:
      cout << "QUEUE used in this calc." << endl;
      break;
    case STACK:
    default:
      cout << "STACK used in this calc." << endl;
      break;
   }

   if (answer == 'y') {
      cout << "There are " << total << " results " 
           << "with  " << solutions << " posible solutions, " << endl;
      cout << "using this values: " << (*this) << endl;
   }
   cout << "The operation takes " << elapsed_time[typeofsolution] << " seconds."<< endl;
}


/*******************************************************************************
 *  Member Function: print_elements
 *  Arguments: (adaptor*) S, the source adaptor, where the result is stored
 *  Comments: print out every solution with its elements. Adaptor is a queue or
 *            a stack.
 ******************************************************************************/
template <class adaptor>
int subset::print_elements(adaptor* S)
{
   int solutions = 0;
   
   for (int r = 0; r < total; r++) {
      cout << "[" << r + 1 << "] == ";
      adaptor tmp(S[r].getSize());
      tmp.Clean();
      if (!S[r].Empty()) {
         tmp = S[r];
      } else {
         cout << endl;
         continue;
      }
      int rsum = 0;
      solutions++;
      while (!tmp.Empty()) {
         int value = tmp.Pop();
         rsum += value;
         cout << value;
         if (!tmp.Empty()) {
            if (rsum == (r+1)) {
                cout << " ; ";
                rsum = 0;
                solutions++;
            } else {
               cout << '+';
            }
         }
      }
      cout << endl;
   }
   cout << endl;
   return solutions;
}



/*******************************************************************************
 *  Member Function: recursive_combination
 *  Arguments: (int[]) array, array of integers
 *             (int) arraylength, is the length of array[]
 *             (int) k, is an integer that is the "target" of the sum. 
 *                      default value is 0, when you want a particular solution 
 *                      (k!=0), it search for the target sum only.
 *             (int[]) pre, array of integers, acts as a prefix
 *             (int) prelength, length of pre[]
 *  Comments: try all posible combination in array s[]. For example:
 *            array[] = { 1 2 3 4 }   It generates: 1 2 3 4
 *                                                  1 2 4 3
 *                                                  1 3 2 4
 *                                                  1 3 4 2
 *                                                  1 4 2 3
 *                                                  1 4 3 2
 *                                                  2 1 3 4
 *                                                  2 3 1 4
 *                                                  2 4 1 3
 *                                                  2 4 3 1
 *                                                  ...
 *            Then you have only to make partial sums, to search k = 7.
 *                1+2 ...
 *                1+2+3 ...
 *                1+2+3+4 ... to big
 *                1+2+4 -> EUREKA!!!
 *                1+3 ...
 *                1+3+2 ...
 *                1+3+4 ... to big
 *                ...
 * 
 ******************************************************************************/
int subset::recursive_combination(int array[], int arraylength, int k, int pre[], int prelength)
{
   int finded = 0;
   int n = arraylength;
   if (n == 1) {
#ifdef WIN32
      int *result = new int[prelength+arraylength];
#else
      int result[prelength+arraylength];
#endif
      int iresult = concat(result, pre, prelength, array, arraylength);
      for (int i = 0; i < iresult; i++) {
         int mysum = sumarray(result, (i+1));
         if (k != 0) {
            if (mysum == k) {
               copyarray(recursiveSolutionFinded[mysum-1], result, (i+1));
               finded = 1;
               break;
            }
         } else {
#ifdef WIN32
            int *tmp = new int[i+1];
#else
            int tmp[i+1];
#endif
            copyarray(tmp, result, (i+1));
            bublesort(tmp, (i+1));
            if (!recursiveSolutionFinded[mysum-1].Exist(tmp, (i+1))) {
                copyarray(recursiveSolutionFinded[mysum-1], result, (i+1));
                recursiveSolutionFindedQtty++;
                cout << (!(recursiveSolutionFindedQtty%100) ? '|' :
                        (!(recursiveSolutionFindedQtty%10) ? '+' : '.'));
                fflush(stdout);
            }
#ifdef WIN32
            delete[] tmp;
#endif
         }
      }
#ifdef WIN32
      delete[] result;
#endif
   }
   else {
      for (int i = 0; i < n; i++)
      {
#ifdef WIN32
         int *part1 = new int[n-1];

         int *newpart = new int[prelength+1];
#endif
         int temp = array[i];
         array[i] = array[0];
         array[0] = temp;
#ifndef WIN32
         int part1[n-1];
#endif
         substr(part1,array,1,n-1);
         int part2[1];
         part2[0] = array[0];
         part2[1] = 0;
#ifndef WIN32
         int newpart[prelength+1];
#endif
         int intnewprelengt = concat(newpart, pre, prelength, part2, 1);

         finded = recursive_combination(part1, (n-1), k, newpart, intnewprelengt);
#ifdef WIN32
         delete[] part1;
         delete[] newpart;
#endif
         if (k != 0 && finded == 1)
            break;
      }
   }
   return finded;
}



/*******************************************************************************
 *  Member Function: create_recursive_solutions
 *  Arguments: (int) k, is an integer that is the "target" of the sum. 
 *                      default value is 0, when you want a particular solution 
 *                      (k!=0), it search for the target sum only.
 *  Comments: it calls the recursive function to search one solution or all
 ******************************************************************************/
void subset::create_recursive_solutions(int k)
{
   recursiveSolutionFindedQtty = 0;
   stack generator(size); // the values that generate the sum
   cout << "Using this values: " << (*this) << endl;
   cout << "Generating solutions between " << 1 << " and " << total << endl;
   lastsolve = RECURSION;
   int onlyonce = 0;

   int *numbers = new int[size];
   for (int i = 0; i < size; i++) {
      numbers[i] = data[i];
   }
   for (int r = 1; r <= total && onlyonce == 0; r++) {
      if (k != 0) {
         r = k;
         onlyonce = 1;
      }
      recursiveSolutionFinded[r-1].Clean();
      recursiveSolutionFinded[r-1].setNoUnique();
   }

   recursive_combination(numbers, size, k);

#ifdef WIN32
   delete[] numbers;
#endif

   cout << endl;
}

/*******************************************************************************
 *  Member Function: create_solutions
 *  Arguments: (adaptor*) SRR, pointer to the array of adaptors, all posible 
 *                             solutions are saved here.
 *             (int) k, is an integer that is the "target" of the sum. 
 *                      default value is 0, when you want a particular solution 
 *                      (k!=0), it search for the target sum only.
 *  Comments: it iterates and try to find and save the posible solutions.
 *            The info is saved in an adaptor (a queue or stack). Also use
 *            a temporary adaptor (generator), where all partial or potencial
 *            solutions are saved.
 ******************************************************************************/
template <class adaptor>
void subset::create_solutions(adaptor* SRR, int k)
{
   adaptor generator(size); // the values that generate the sum
   cout << "Using this values: " << (*this) << endl;
   cout << "Generating solutions between " << 1 << " and " << total << endl;
   if (SRR->whatIAm() == STACK) {
      stackSolutionFindedQtty = 0;
      lastsolve = STACK;
   } else {
      queueSolutionFindedQtty = 0;
      lastsolve = QUEUE;
   }

   int onlyonce = 0;

   for (int r = 1; r <= total && onlyonce == 0; r++) {
      if (k != 0) {
         r = k;
         onlyonce = 1;
      }
      SRR[r-1].Clean();
      SRR[r-1].setNoUnique();
      if (r >= 1 && r <= size) {
         SRR[r-1].Push(r);
         if (onlyonce) break;
         if (lastsolve == STACK) {
             stackSolutionFindedQtty++;
             cout << (!(stackSolutionFindedQtty%100) ? '|' :
                     (!(stackSolutionFindedQtty%10) ? '+' : '.'));
         } else {
             queueSolutionFindedQtty++;
             cout << (!(queueSolutionFindedQtty%100) ? '|' :
                     (!(queueSolutionFindedQtty%10) ? '+' : '.'));
         }
      }
      generator.Clean();
      int v = 1;
      while (v < r)
      {
         if (v > size) { // big problem
                         // v needs to be between 1 and the size
                         // for example [1,2,3,4] -> 1 and 4

            if (generator.Empty()) // no more back
               break;      // I'm at the beginning of the generators
            // back to the predecessor
            if (lastsolve == STACK)
            {
               v = generator.Pop() + 1;
            } else {
               adaptor tmp(generator.getSize());
               int lastvalue = generator.Pop();
               while (!generator.Empty()) {
                  tmp.Push(lastvalue);
                  lastvalue = generator.Pop();
                  if (generator.Empty())
                     break;
               }
               generator = tmp;
               v = lastvalue + 1;
            }
            continue;
         }
         if ((v + generator.getSum()) > r)
         {
            // problem
            if (lastsolve == STACK)
            {
               v = generator.Pop() + 1;
            } else {
               adaptor tmp(generator.getSize());
               int lastvalue = generator.Pop();
               while (!generator.Empty()) {
                  tmp.Push(lastvalue);
                  lastvalue = generator.Pop();
                  if (generator.Empty())
                     break;
               }
               generator = tmp;
               v = lastvalue + 1;
            }
         } else if ((v + generator.getSum()) == r) {
            SRR[r-1] += generator;
            SRR[r-1].Push(v);
            if (onlyonce) break;

            if (lastsolve == STACK)
            {
               stackSolutionFindedQtty++;
               cout << (!(stackSolutionFindedQtty%100) ? '|' :
                       (!(stackSolutionFindedQtty%10) ? '+' : '.'));
               fflush(stdout);
               v = generator.Pop() + 1;
            } else {
               queueSolutionFindedQtty++;
               cout << (!(queueSolutionFindedQtty%100) ? '|' :
                       (!(queueSolutionFindedQtty%10) ? '+' : '.'));
               fflush(stdout);
               adaptor tmp(generator.getSize());
               int lastvalue = generator.Pop();
               while (!generator.Empty()) {
                  tmp.Push(lastvalue);
                  lastvalue = generator.Pop();
                  if (generator.Empty())
                     break;
               }
               generator = tmp;
               v = lastvalue + 1;
            }


         } else {
            // this is when (generator.getSum() < r) 
            generator.Push(v);
            v++;
         }
      }
   }
   cout << endl;
}


