/*******************************************************************************
 *  File: Tuple.h
 *  
 *  Comments: A tuple is a list of integers that has a particular order, so the
 *            tuple (1 2 3) is different from the tuple (2 3 1) since the 
 *            elements are ordered differently. Duplicates are allowed, so 
 *            (2 2 7) is a valid tuple. The elements of the tuples will be 
 *            integers, so each tuple will be a sequence of ints. Since we 
 *            don't know how many elements will be in a given tuple, we must
 *            use dynamic memory to implement the class.
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

#ifndef __TUPLE_H__
#define __TUPLE_H__

#include <algo.h>
#include "Size.h"

extern int intPower(int x, int n);

class Tuple {

public: 

   // ---------------------
   // Some private constans
   // ---------------------

   static const char ASCII_BEGIN = '(';
   static const char ASCII_END   = ')';
   static const char ASCII_SEPAR = ' ';
   static const char ASCII_FINAL = '\0';

   // ----------------------
   // Some private typedefs
   // ----------------------

   // Here 'int', (signed int) is our base datatype
   // Every element of this Tuple is an integer.
   typedef int   value_type;
   typedef value_type* pointer;
   typedef const value_type* const_pointer;
   typedef value_type& reference;
   typedef const value_type& const_reference;
   
   typedef ptrdiff_t difference_type;
   typedef size_t size_type;

   typedef pointer iterator;
   typedef const_pointer const_iterator;
   
   typedef std::reverse_iterator<iterator> reverse_iterator;
   typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

   // ------------------------------------------
   // Some useful functions for STL manipulation
   // ------------------------------------------
   iterator begin() { return data; }
   iterator last()  { return data + size(); }
   iterator end()   { return data + size.getCapacity(); }

   const_iterator begin() const { return data; }
   const_iterator last()  const { return data + size(); }
   const_iterator end()   const { return data + size.getCapacity(); }

   reverse_iterator rbegin() { return reverse_iterator(end()); }
   reverse_iterator rend()   { return reverse_iterator(begin()); }

   const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
   const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }


   // ------------------------
   // Private Member Functions
   // ------------------------
   void push_back(value_type v);
   void push_back(iterator i);

   // remove an element
   void remove(value_type x);


public:

   // ------------
   // Constructors
   // ------------

   // Default constructor
   Tuple();

   // Copy constructor
   Tuple(const Tuple& t);

   // Constructor, which creates the tuple (0 1 2 ... k-1).
   Tuple(value_type k);

   // Constructor, which makes a tuple with the k elements from array p.
   Tuple(value_type* p, int k);
   Tuple(char* p, int k);

   // Destructor
   ~Tuple();


   // ----------------
   // Member Functions
   // ----------------

   // add an element
   void addElem(value_type k);

   // delete an element
   bool delElem(value_type k);

   // find a value in a Tuple
   int findElem(value_type x);


   // ---------
   // Operators
   // ---------

   // Overloaded assignment operator =, which makes a deep copy.
   Tuple& assignment(const Tuple& t);
   Tuple& operator = (const Tuple& t);
   Tuple& operator = (value_type v);
   Tuple& operator += (const Tuple& t);
   Tuple& operator += (value_type t);

   // Overloaded comparisson operators
   // A==B
   bool operator == (const Tuple& t) const;
   // A!=B
   bool operator != (const Tuple& t) const;
   // A<B
   bool operator <  (const Tuple& t) const;
   // A>B
   bool operator >  (const Tuple& t) const;
   // A<=B
   bool operator <= (const Tuple& t) const;
   // A>=B
   bool operator >= (const Tuple& t) const;
#if 1
   const Tuple& operator  * (const Tuple& s) const;
#else
   bool operator  * (const Tuple& s) const;
#endif

   // t[k] returns the kth element of the Tuple
   reference operator[](size_type k);


   //---------------------------------------------------------------------------
   // The stream insertion and extraction operators write out and read in 
   // tuples. A tuple is formatted as follows: 
   // - first an open parenthesis: ( followed by one or more spaces, 
   // - then a space-separated list of integers (the number of spaces between 
   //   ints can vary), 
   // - then one or more spaces, then a close parenthesis: ).
   friend ostream& operator<<(ostream& out, const Tuple* t);
   friend ostream& operator<<(ostream& out, const Tuple& t);
   void operator>>(char *buffer);

   // Get sizes
   size_type getSize() { return size(); }
   size_type getCapacity() { return size.getCapacity(); }
   const int getData(int i) const { return (*this).data[i]; }

protected:
   // Size of the tuple
   Size        size;

   // raw data
   value_type* data;


   // flag memory allocated
   bool        memoryIsAllocated;
};

#endif // __TUPLE_H__
