/*******************************************************************************
 *  File: Set.h
 *  
 *  Comments: Class set, is a collection of tuples. A set is defined as an 
 *            unordered collection, so the sets {(1 2) (2 4 7)} and 
 *            {(2 4 7) (1 2)} are identical. No duplicates are allowed, 
 *            so {(1 2) (1 2) (6 4 9)} is not a valid set; instead it should 
 *            be represented as {(1 2) (6 4 9)}.
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

#ifndef __SET_H__
#define __SET_H__

#include <algo.h>
#include "Tuple.h"

class Set {

public: 
   // ---------------------
   // Some private constans
   // ---------------------

   static const char ASCII_BEGIN = '{';
   static const char ASCII_END   = '}';
   static const char ASCII_SEPAR = ' ';
   static const char ASCII_FINAL = '\0';

   // ----------------------
   // Some private typedefs
   // ----------------------

   // Here the class 'Tuple', is our base datatype
   // Every element of this Set is a Tuple.
   typedef Tuple  value_type;
   typedef value_type* pointer;
   typedef const value_type* const_pointer;
   typedef value_type& reference;
   typedef const value_type& const_reference;
   
   typedef ptrdiff_t difference_type;
   typedef size_t size_type;

   typedef pointer iterator;
   typedef pointer iterator_category;
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
   void getMemory(void);
   void push_back(iterator t);
   void push_back(value_type& t);
   void push_back(const value_type& t);

      template <class kind>
   void TecBlock_iterator(kind b)
   {
      std::copy(b.begin(), b.last(), back_inserter(*this));
   }

   // sort 
   void sort();

   // remove an element
   void remove(value_type t);


public:
   // ------------
   // Constructors
   // ------------

   // Default constructor
   Set();

   // Copy constructor
   Set(const Set& s);

   // Constructor, which creates a set containing a single tuple t.
   Set(value_type& t);
   Set(value_type* t);

   // Constructor, which creates a set with a single tuple with (0 1 2 ... k-1).
   Set(int k);

   // Constructor, which makes a set with the k tuples from array p.
   Set(value_type* p, int k);

   // Destructor
   ~Set();


   // ----------------
   // Member Functions
   // ----------------

   // add a tuple
   void addTuple(value_type* t);
   void addTuple(value_type& t);

   // delete a tuple
   bool delTuple(value_type& t);

   // find a value in a Tuple
   value_type* myFind(value_type& t);
   int findTuple(value_type& t);
   int findTuple(const value_type& t) const;


   // ---------
   // Operators
   // ---------

   // Overloaded assignment operator =, which makes a deep copy.
   Set& equalSet(value_type& t);
   Set& operator = (const Set& s);
   Set& operator = (value_type& t);
   Set& operator = (const value_type& t);
   Set& operator = (int i);

   // Overloaded operator +, union
   Set operator + (const Set& s) const;
   // Overloaded operator *, intersection
   Set operator * (const Set& s) const;
   // Overloaded operator -, difference
   Set operator - (const Set& s) const;
   // Overloaded operator ^, symetric difference
   Set operator ^ (const Set& s) const;
   // Overloaded operator /, Cartesian product of sets
   Set operator / (const Set& s) const;
   // Overloaded operator ^, Cartesian product of r with itself k times.
   Set operator ^ (const int k) const;
   // Overloaded operator %, inclusion
   bool operator % (const Tuple& t) const;
   // bool operator % (Tuple* t);
   // Overloaded operator !, empty or not
   bool operator ! (void) const;

   // Overloaded comparisson operators
   // A==B
   bool operator == (const Set& s) const;
   // A!=B
   bool operator != (const Set& s) const;
   // A<B
   bool operator <  (const Set& s) const;
   // A>B
   bool operator >  (const Set& s) const;
   // A<=B
   bool operator <= (const Set& s) const;
   // A>=B
   bool operator >= (const Set& s) const;

   // r[k] returns the kth tuple 
   reference operator[](int k);
   const_reference operator[](int k) const;


   //---------------------------------------------------------------------------
   // The stream insertion and extraction operators write out and read in sets. 
   // A set is formatted as follows: 
   // - first an open curly brace: { followed by one or more spaces, 
   // - then a space-separated list of tuples formatted as above 
   //   (the number of spaces between tuples can vary), 
   // - then one or more spaces, 
   // - then a close curly brace: }, e.g.:
   //             { ( 2 5 7 ) ( 1 7 ) ( 7 1 ) }
   friend ostream& operator<<(ostream& out, const Set* s);
   friend ostream& operator<<(ostream& out, const Set& s);
   void operator>>(char *buffer);

   size_type getSize() { return size(); }
   const size_type getSize() const { return size(); }
   size_type getCapacity() { return size.getCapacity(); }
   const size_type getCapacity() const { return size.getCapacity(); }

protected:
   // Size of the Set
   Size        size;

   // raw data
   value_type* data;

   // flag memory allocated
   bool        memoryIsAllocated;
};

#endif // __SET_H__
