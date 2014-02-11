/*******************************************************************************
 *  File: Tuple.cpp
 *  
 *  Comments: Implementation of class Tuple.
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


// ------------
// Includes
// ------------

#include <stdlib.h>
#include <assert.h>
#include "Tuple.h"


/*******************************************************************************
 *  Default constructor
 *  Comments: creates an empty tuple.
 ******************************************************************************/

Tuple::Tuple()
{
   memoryIsAllocated = false;
}


/*******************************************************************************
 *  Copy constructor
 *  Comments: makes a deep copy.
 ******************************************************************************/

Tuple::Tuple(const Tuple& t)
{
   if (t.size() == 0)
      return;
   assignment(t);
}


/*******************************************************************************
 *  Constructor
 *  Comments: creates the tuple (0 1 2 ... k-1).
 ******************************************************************************/

Tuple::Tuple(value_type k)
{
   for (value_type i = 0; i < k; i++) {
      addElem(i);
   }
}


/*******************************************************************************
 *  Constructor 
 *  Comments: which makes a tuple with the k elements from array p.
 *  Arguments: 2
 *             p (value_type*) is an array of integers
 *             kth elements of array p
 ******************************************************************************/

Tuple::Tuple(value_type* p, int k)
{
   for (int i = 0; i < k; i++) {
      addElem(p[i]);
   }
}


/*******************************************************************************
 *  Constructor 
 *  Comments: which makes a tuple with the k elements from array p.
 *  Arguments: 2
 *             p (char*) is a string like: "123459"
 *             kth elements of array p
 ******************************************************************************/

Tuple::Tuple(char* p, int k)
{
   for (int i = 0; i < k; i++) {
      addElem((value_type)p[i]);
   }
}




/*******************************************************************************
 *  Destructor
 *  Comments: free your mind
 ******************************************************************************/

Tuple::~Tuple()
{
   if (memoryIsAllocated == true)
      free(data);
}


/*******************************************************************************
 *  Member Function: addElem
 *  Comments: add an element, which adds integer k to the end of the current
 *            tuple.
 *  Arguments: 1
 *  Return: -
 ******************************************************************************/

void Tuple::addElem(value_type k)
{
   push_back(k);
}

/*******************************************************************************
 *  Member Function: delElem
 *  Comments: Deletes the first occurrence of integer k from the current tuple, 
 *            if it exists. 
 *  Arguments: 1
 *  Return: If it exists, then return true, else return false.
 ******************************************************************************/

bool Tuple::delElem(value_type k)
{
   value_type position;

   if ((position = findElem(k)) == (-1))
      return false;

   // delete the element
   remove(k);

   // reallocate memory and array space
   --size;
   if (size() < (size.getCapacity()/4)) {
      data = (value_type *) realloc(data, sizeof(value_type)*size.getCapacity());
   }
   return true;
}


/*******************************************************************************
 *  Member Function: findElem
 *  Comments: Find the first occurrence of an element.
 *  Arguments: 1
 *  Return: If it exists, then return the position of the element in the 
 *          tuple, between [0-nth].
 *          If not exists, then return (-1)
 ******************************************************************************/

int Tuple::findElem(value_type x)
{
   value_type *p = std::lower_bound(begin(), last(), x);
   return( (p == last()) ? (-1) : (p - begin()) );
}

/*******************************************************************************
 *  Member Function: push_back
 *  Comments: add elements
 *  Arguments: 1
 *  Return: -
 ******************************************************************************/

void Tuple::push_back(value_type v)
{
   if (!size()) {
      data = (value_type *) malloc (sizeof(value_type)*size.getCapacity());
      memoryIsAllocated = true;
   } else if (size() >= size.getCapacity()) {
      ++size; 
      --size;
      data = (value_type *) realloc(data, sizeof(value_type)*size.getCapacity());
   }
   data[size()] = v;
   ++size;
}


/*******************************************************************************
 *  Member Function: push_back
 *  Comments: add elements
 *  Arguments: 1
 *  Return: -
 ******************************************************************************/

void Tuple::push_back(iterator i) {
   push_back(*i);
}



/*******************************************************************************
 *  Member Function: remove
 *  Comments: remove an element
 *  Arguments: an element
 *  Return: -
 ******************************************************************************/
void Tuple::remove(value_type x) {
   value_type *newend = std::remove(begin(), last(), x);
}


/*******************************************************************************
 *  Operator: =
 *  Comments: Overloaded assignment operator =, which makes a deep copy.
 *  Return: this instance
 ******************************************************************************/
Tuple& Tuple::assignment(const Tuple& t) 
{
   // A = B (assignment)
   // -----

   // ------------------------------
   // A.size is less than B.size
   if (size() < t.size()) {
      int i = 0;

      while (i < size()) {
         data[i] = t.data[i];    // replace every value from B into A
         ++i;
      }

      // nothing to be done with the memory
      // because addElem (into push_back()) already do it
      while (i < t.size()) {
         addElem(t.data[i]);   // add new values from B
         ++i;
      }

   // ------------------------------
   // A.size is greather than B.size
   } else if (size() > t.size()) {
         int i = 0;
         while (i < t.size())  // replace every value from B into A
         {
            data[i] = t.data[i];
            i++;
         }
         // reallocate memory and array space
         size = t.size;
         if (size() < (size.getCapacity()/4)) {
            data = (value_type *) realloc(data, sizeof(value_type)*size.getCapacity());
         }

   // ------------------------------
   // the same size
   } else { 
      // replace every value from B into A
      // nothing to be done with the memory
      for (int i = 0; i < t.size(); i++)
         data[i] = t.data[i];
   }
   return (*this);
}

Tuple& Tuple::operator = (const Tuple& t) 
{
   return (assignment(t));
}
Tuple& Tuple::operator += (const Tuple& t) 
{
   std::copy(t.begin(), t.last(), back_inserter(*this));
   return (*this);
}
Tuple& Tuple::operator += (value_type v) 
{
   addElem(v);
   return (*this);
}

// Overloaded assignment operator =, which makes a deep copy.
Tuple& Tuple::operator = (value_type v) 
{
   addElem(v);
   return (*this);
}


/*******************************************************************************
 *  Operator: ==
 *  Comments: compare 2 tuples (A==B)
 *  Return: true if the tuples are equal
 ******************************************************************************/

// (A == B)
bool Tuple::operator == (const Tuple& t) const
{
   // if size is diferent, nothing we can do
   if (size != t.size)
      return false;
   // compare every element
   for (size_t n = 0; n < t.size(); n++)
      if (data[n] != t.data[n])  // this is diferent
         return false;
   // all elements are the equivalents
   return true;
}

// (A != B)  <==> !(A == B)
bool Tuple::operator != (const Tuple& t) const {
   return (!((*this) == t));
}

/*******************************************************************************
 *  Operator: <
 *  Comments: compare 2 tuples (A<B). Usefull if we will sort between tuples
 *  Return: true if 'this' is less than t
 ******************************************************************************/

// (A < B)
bool Tuple::operator < (const Tuple& t) const
{
   // if size is diferent, nothing we can do
   if (size < t.size)
      return true;
   else if (t.size < size)
      return false;

   // The same size, then the comparisson is made between elements of a tuple
   for (size_t n = 0; n < t.size(); n++)
      if (data[n] < t.data[n])   
         return true;
      else if (t.data[n] < data[n])
         return false;
   return false;
}
// (A > B)  <==> (B < A)
bool Tuple::operator > (const Tuple& t) const
{
   return (t < (*this));
}
// (A <= B) <==> (A < B || A == B)
bool Tuple::operator <= (const Tuple& t) const
{
   return ((*this) < t) || ((*this) == t);
}
// (A >= B) <==> (A > B || A == B)
bool Tuple::operator >= (const Tuple& t) const
{
   return ((*this) > t) || ((*this) == t);
}
#if 1
const Tuple& Tuple::operator * (const Tuple& t) const
{
   Tuple tmp;
   std::copy(t.begin(), t.last(), back_inserter(tmp));
   if ((*this) == tmp)
      return (*this);
   else
      return (t);
}
#else
bool Tuple::operator  * (const Tuple& s) const
{
   return ((*this) == s);
}
#endif




/*******************************************************************************
 *  Operator: []
 *  Comments: t[k] returns the kth element of the Tuple if k is at least 0 and 
 *            strictly smaller than the size of the tuple (so the entries in 
 *            tuple are numbered starting from 0). If k is out of bounds, then 
 *            report an error message and abort the program.
 *  Return: returns the kth element 
 ******************************************************************************/

Tuple::reference Tuple::operator[](Tuple::size_type k)
{
   bool outOfBounds = !(!k || k >= size());
   if (outOfBounds)
   {
      // report the error message
      std::cerr << "k = " << k << " is out of bounds." 
                << "Need to be between 0 and " << (size() - 1) << std::endl;
      // out of bounds
      assert(outOfBounds);
   }

   return (*this).data[k]; 
}

/*******************************************************************************
 *  Operator: <<
 *  Comments: write out a tuple
 * -----------------------------------------------------------------------------
 *  The stream insertion and extraction operators write out and read in 
 *  tuples. A tuple is formatted as follows: 
 *  - first an open parenthesis: ( followed by one or more spaces, 
 *  - then a space-separated list of integers (the number of spaces between 
 *    ints can vary), 
 *  - then one or more spaces, then a close parenthesis: ).
 ******************************************************************************/

ostream& operator<<(ostream& out, const Tuple* t)
{
   out << (*t);
   return out;
}

ostream& operator<<(ostream& out, const Tuple& t)
{
   out << Tuple::ASCII_BEGIN;
   out << Tuple::ASCII_SEPAR;
   std::copy(t.begin(), t.last(), ostream_iterator<Tuple::value_type>(out," "));
   out << Tuple::ASCII_END;
   return out;
}



/*******************************************************************************
 *  Operator: >>
 *  Comments: read in a tuple. See above about the format of a tuple.
 ******************************************************************************/

void Tuple::operator>>(char *buffer)
{
   assert (buffer != NULL);        // null is not permited 
   int length = strlen(buffer);
   assert (!(length < 3));         // minimum format "(i)"

   // Find Begin of Tuple
   char *first = std::find(buffer, buffer+length,
         (char)Tuple::ASCII_BEGIN);
   assert (*first == Tuple::ASCII_BEGIN);    // is not there, bad tuple

   // Find End of Tuple
   char *last = std::find(buffer, buffer+length,
         (char)Tuple::ASCII_END);
   assert (*last == Tuple::ASCII_END);    // is not there, bad tuple

   // something imposible, but, who knows
   assert (first != last);

   // descending position into while
   // ------------------------------
   Tuple * tmp = new Tuple();
   while (first < (--last)) {
      assert ((*last >= '0' && *last <= '9') ||
              *last == Tuple::ASCII_SEPAR);

      // imposible, but who knows
      assert (first <= last);

      if (*last == Tuple::ASCII_SEPAR)
         continue;

      Tuple::value_type power = 0;         // every time whe found a digit between 0-9
      Tuple::value_type number = 0;        // number to have in a tuple
      Tuple::value_type digit  = 0;        // every digit
      do
      {
         // convert ascii ('0'-'9') into integers
         digit = (*last) - 48;

         // multiply the digit by the nth power of 10
         // example: 120 = 0*10^1 + 2*10^2 + 1*10^3
         // trying a number with unsigned int
         unsigned int result = number + (digit * (intPower(10,power)));
         // Number is too big, be carefull
         assert(result <= INT_MAX);
         // convert the number into signed int
         number = ((Tuple::value_type) result);

         // increasing power, descending position
         // -------------------------------------
         power++;
         last--;

         // imposible, but who knows
         assert (first <= last);

         // only this is permited into this cicle
         assert ((*last >= '0' && *last <= '9') ||
                 *last == Tuple::ASCII_SEPAR ||
                 *last == Tuple::ASCII_BEGIN ||
                 *last == Tuple::ASCII_END);

      } while (*last >= '0' && *last <= '9' && 
               first != last);
      tmp->addElem(number);
   }
   std::reverse(tmp->begin(), tmp->last());
   (*this) += (*tmp);
}

