/*******************************************************************************
 *  File: util.cpp
 *  
 *  Comments: the validation test.
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

#include "queue.h"   // The queue class
#include "subset.h"   // The subset class
#include <fstream> 
using namespace std;

//----------------------------------------------------------------------------
// These functions are explained and implemented below
//

void solveSolution(subset& S, int onlyrecursive = 0)
{
    char menuselect='x'; // The user input is stored in this variable
    int k = 0;
    int finded = 0;
    int *result = NULL;
    while (menuselect!='0') {   // Loop as long as the user doesn't want to quit
     system("clear");
     cout << "Comparative recursion, stacks, and queues." << endl;
     cout << "subset created with the following values: ";
     cout << S << endl;
     cout << " r: Find first solution using recursion" << endl;
     cout << " s: Find first solution using a stack" << endl;
     cout << " q: Find first solution using a queue" << endl;
      if (finded)
         cout << " p: Print the last solution" << endl;
      cout << " R: Calculate and print all posible solutions using recursion" << endl;
      cout << " S: Calculate and print all posible solutions using a stack" << endl;
      cout << " Q: Calculate and print all posible solutions using a queue" << endl;
      cout << " 0: delete the subset and go back" << endl;
      cout << "Your menu selection: ";
      cin >> menuselect;
      switch (menuselect) { 
         case 'p':
             if (finded) {
                cout << "solution" << endl;
                cout << "--------" << endl;
                S.print();
                cout << endl;
             }
             break;
         case 'q':
             cout << "Enter a partial result to search using a queue: ";
             cin >> k;
             result = S.find_solution_queue(k);
             break;
         case 'r':
             cout << "Enter a partial result to search using recursion: ";
             cin >> k;
             result = S.find_solution_recursive(k);
             break;
         case 's':
             cout << "Enter a partial result to search using a stack: ";
             cin >> k;
             result = S.find_solution_stack(k);
             break;
         case 'Q':
             cout << "All posible solutions: Queue" << endl;
             cout << "---------------------" << endl;
             S.print_all(QUEUE);
             cout << endl;
             break;
         case 'R':
             cout << "All posible solutions: Recursion" << endl;
             cout << "---------------------" << endl;
             S.print_all(RECURSION);
             cout << endl;
             break;
         case 'S':
             cout << "All posible solutions: Stack" << endl;
             cout << "---------------------" << endl;
             S.print_all(STACK);
             cout << endl;
             break;
         case '0':
             cout << endl;
             break;
         default:
            cout << "Invalid choice" << endl;
      }
      if (menuselect!='0') {
                        
        cout << "Press enter to continue ";
        char nothing;
        cin.get(nothing); // Do this twice, since the first one reads the
        cin.get(nothing); // newline from the choice above, the second reads
                          // the current enter.  Is there a better way?
      }
       if (result != NULL)
          finded = 1;
       else
          finded = 0;
   }
}
// the main.
int main()
{
   char menuselect='x'; // The user input is stored in this variable
   int n = 0;

   while (menuselect!='0') {   // Loop as long as the user doesn't want to quit
      system("clear");
      cout << "Comparative recursion, stacks, and queues." << endl;
      cout << "Please select an operation:" << endl;
      cout << " 1: Create a subset sum (subset)" << endl;
      cout << " 0: Quit " << endl;
      cout << "Your menu selection: ";
      cin >> menuselect;
      switch (menuselect) { 
         case '1': 
            {
            cout << "Enter an integer: ";
            cin >> n;
            subset S(n);
            solveSolution(S);
            break;
            }
         case '0':
            cout << endl;
            break;
         default:
            cout << "Invalid choice" << endl;
      }
      if (menuselect!='0') {
                        
        cout << "Press enter to continue ";
        char nothing;
        cin.get(nothing); // Do this twice, since the first one reads the
        cin.get(nothing); // newline from the choice above, the second reads
                          // the current enter.  Is there a better way?
      }

   }
return 0;
}

