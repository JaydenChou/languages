/*******************************************************************************
 *  File: Set.cpp
 *  
 *  Comments: Implementation of class Set.
 *  
 *  Compiler: tested in linux only
 *     gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 (i386) - kernel 2.4
 *     gcc/g++ 3.3.3 20040412 - Fedora Core 2 (i686) - kernel 2.6.10
 *     gcc/g++ 3.4.3 20050227 - Fedora Core 3 (i386) - kernel 2.6.12
 *     gcc/g++ 4.0.0 20050519 - Fedora Core 4 (i686) - kernel 2.6.11
 *     gcc/g++ 4.0.1 20050727 - Fedora Core 4 (x86_64) - kernel 2.6.12
 *  
 *  History:
 *       Oct-20-2005: Javier Fuchs                                   |\_/|   
 *                    Phone (+54 2944) 525173                       /     \  
 *                    http://southfox.servehttp.com                /_.~ ~,_\ 
 *                                                                    \@/    
 ******************************************************************************/


// ------------
// Includes
// ------------

#include <stdlib.h>
#include <assert.h>
#include "Set.h"

/*******************************************************************************
 *  Default constructor
 *  Comments: creates an empty set.
 ******************************************************************************/

Set::Set()
{
   memoryIsAllocated = false;
}


/*******************************************************************************
 *  Copy constructor
 *  Comments: makes a deep copy.
 ******************************************************************************/

Set::Set(const Set& s)
{
   if (s.size() == 0)
      return;
   std::copy(s.begin(), s.last(), back_inserter(*this));
}


/*******************************************************************************
 *  Constructor
 *  Comments: creates a set containing a single tuple t 
 ******************************************************************************/

Set::Set(value_type& t)
{
   addTuple(&t);
}
// overloading 
Set::Set(value_type* t)
{
   addTuple(t);
}

/*******************************************************************************
 *  Constructor
 *  Comments: creates a set containing a single tuple t with values 
 *            between 0 and k-1
 *            e.g. r = { (0 1 2 ... k-1) }
 ******************************************************************************/

Set::Set(int k)
{
   addTuple(new Tuple(k));
}


/*******************************************************************************
 *  Constructor
 *  Comments: which makes a tuple with the k elements from array p.
 ******************************************************************************/

Set::Set(value_type* p, int k)
{
   for (int i = 0; i < k; i++) {
      addTuple(p[i]);
   }
}





/*******************************************************************************
 *  Destructor
 *  Comments: free your mind
 ******************************************************************************/

Set::~Set()
{
   if (memoryIsAllocated == true) {
      if (data != NULL)
         free(data);
   }
}


/*******************************************************************************
 *  Member Function: addTuple
 *  Comments: add a tuple, which adds tuple t to the current set.
 *  Arguments: 1
 *  Return: -
 ******************************************************************************/

void Set::addTuple(value_type* t)
{
   Tuple tmp;
   std::copy(t->begin(), t->last(), back_inserter(tmp));
   if (!((*this)%tmp))
      push_back(t);
}
// Overloading
void Set::addTuple(value_type& t)
{
   if (!((*this)%t))
      push_back(t);
}

/*******************************************************************************
 *  Member Function: delTuple
 *  Comments: Deletes tuple t from the current set, if it exists. 
 *  Arguments: 1
 *  Return: If it exists, then return true, else return false.
 ******************************************************************************/

bool Set::delTuple(value_type& t)
{
   value_type position;

   if ((position = findTuple(t)) == (-1))
      return false;

   // delete the element
   remove(t);

   --size;
   return true;
}


/*******************************************************************************
 *  Member Function: findTuple
 *  Comments: Find the first occurrence of a tuple in a Set
 *  Arguments: 1
 *  Return: If it exists, then return the position of the tuple in the 
 *          set, between [0-nth].
 *          If not exists, then return (-1)
 ******************************************************************************/

Set::value_type* Set::myFind(value_type& t) 
{
   value_type *p = std::find(begin(), last(), t);
   return p;
}
int Set::findTuple(const value_type& t) const
{
   Set tmpS;
   std::copy(t.begin(), t.last(), back_inserter(tmpS));
   value_type myT(t);
   value_type *p = tmpS.myFind(myT);
   return( (p == tmpS.last()) ? (-1) : (p - tmpS.begin()) );
}
int Set::findTuple(value_type& t)
{
   value_type *p = std::find(begin(), last(), t);
   return( (p == last()) ? (-1) : (p - begin()) );
}

/*******************************************************************************
 *  Member Function: getMemory
 *  Comments: obtains memory of the OS
 *  Arguments: 1
 *  Return: -
 ******************************************************************************/

void Set::getMemory(void)
{
   if (!size()) {
      data = (Set::value_type *) malloc (sizeof(Set::value_type)*size.getCapacity());
      memoryIsAllocated = true;
   } else if (size() >= size.getCapacity()) {
      ++size; 
      --size;
      data = (Set::value_type *) realloc(data, sizeof(Set::value_type)*size.getCapacity());
   }
}
/*******************************************************************************
 *  Member Function: push_back
 *  Comments: add tuples, asking for memory
 *  Arguments: 1
 *  Return: -
 ******************************************************************************/

void Set::push_back(Set::iterator t) 
{
   getMemory();
   memcpy(&(data[size()]), t, sizeof(Tuple));
   ++size;
}
// overloading
void Set::push_back(Set::value_type& t)
{
   getMemory();
   memcpy(&(data[size()]), &t, sizeof(Tuple));
   ++size;
}
// overloading
void Set::push_back(const Set::value_type& t)
{
   getMemory();
   memcpy(&(data[size()]), &t, sizeof(Tuple));
   ++size;
}

/*******************************************************************************
 *  Member Function: sort
 *  Comments: sort tuples in a set
 *  Arguments: -
 *  Return: -
 ******************************************************************************/
void Set::sort() { 
   std::stable_sort(begin(), last());
}



/*******************************************************************************
 *  Member Function: remove
 *  Comments: remove a tuple
 *  Arguments: a tuple
 *  Return: -
 ******************************************************************************/
void Set::remove(value_type t) {
   value_type *newend = std::remove(begin(), last(), t);
}



/*******************************************************************************
 *  Operator: =
 *  Comments: Overloaded assignment operator =, which makes a deep copy 
 *            of a Set
 *  Return: this instance
 ******************************************************************************/
Set& Set::operator = (const Set& s)
{
   // A = B (assignment)
   // -----

   // ------------------------------
   // A.size is less than B.size
   if (size() < s.size()) {
      int i = 0;

      while (i < size()) {
         // replace every value from B into A
         memcpy(&(data[i]), &s.data[i], sizeof(Tuple));
         i++;
      }
      // nothing to be done with the memory
      // because addTuple (into push_back()) already do it
      while (i < s.size()) {
         addTuple(&s.data[i]);   // add new values from B
         i++;
      }

   // ------------------------------
   // A.size is greather than B.size
   } else if (size() > s.size()) {
         int i = 0;
         while (i < s.size())  // replace every value from B into A
         {
            memcpy(&(data[i]), &s.data[i], sizeof(Tuple));
            i++;
         }
         // reallocate memory and array space
         size = s.size;
         if (size() < (size.getCapacity()/4)) {
            data = (Set::value_type *) realloc(data, sizeof(Set::value_type)*size.getCapacity());
         }

   // ------------------------------
   // the same size
   } else { 
      // replace every value from B into A
      // nothing to be done with the memory
      for (int i = 0; i < s.size(); i++)  // replace every value from B into A
         memcpy(&(data[i]), &s.data[i], sizeof(Tuple));
   }
   return (*this);
}


/*******************************************************************************
 *  Member Function: equalSet
 *  Comments: call push for add a tuple in a set
 *  Return: this instance
 ******************************************************************************/
Set& Set::equalSet(Set::value_type& t)
{
   push_back(&t);
   return (*this);
}
/*******************************************************************************
 *  Operator: =
 *  Comments: Overloaded assignment operator =, which makes a deep copy 
 *            of a single Tuple in a Set
 *  Return: this instance
 ******************************************************************************/
Set& Set::operator = (Set::value_type& t)
{
   return equalSet(t);
}
// overloading
Set& Set::operator = (const Set::value_type& t)
{
   Tuple i(t);
   return equalSet(i);
}

/*******************************************************************************
 *  Operator: =
 *  Comments: Overloaded assignment operator =, which makes a deep copy 
 *            of a single Tuple in a Set
 *  Return: this instance
 ******************************************************************************/
Set& Set::operator = (int i) 
{
   Tuple t(i);
   t.addElem(i);
   addTuple(t);
   return (*this);
}


/*******************************************************************************
 *  Operator: +, union (r + s)
 *  Comments: r + s returns the union of sets r and s, which is the set of 
 *            tuples that appear in r or s. Note that due to the definition of 
 *            a set, you need to remove any duplicate tuples before returning 
 *            the union. 
 *            E.g. if r = { ( 1 2 ) ( 9 4 3 ) } and 
 *                 s = { ( 6 8 1 ) ( 9 4 3 ) ( 15 ) }, 
 *                 then r + s = { ( 1 2 ) ( 9 4 3 ) ( 15 ) }.
 *  Return: returns the union of sets r and s
 ******************************************************************************/

Set Set::operator + (const Set& s) const
{
   Set result(*this);
   std::copy(s.begin(), s.last(), back_inserter(result));
   return (result);
} 


/*******************************************************************************
 *  Operator: *, intersection (r * s)
 *  Comments: r * s returns the intersection of sets r and s, which is the set 
 *            of tuples that appear in r and s. 
 *            E.g. if r = { ( 1 2 ) ( 9 4 3 ) } and 
 *                 s = { ( 6 8 1 ) ( 9 4 3 ) ( 15 ) }, 
 *                 then r * s = { ( 9 4 3 ) }.
 *  Return: returns the intersection of sets r and s
 ******************************************************************************/

Set Set::operator * (const Set& s) const {
   Set result;
   for (int i = 0; i < s.size(); i++)  
      if ((*this) % s.data[i]) {
         result.addTuple(&s.data[i]);
      }

   return (result);
}



/*******************************************************************************
 *  Operator: -, difference (r - s)
 *  Comments: r - s returns the set difference of sets r and s, which is the 
 *            set of tuples that appear in r but not in s. 
 *            E.g. if r = { ( 1 2 ) ( 9 4 3 ) } and 
 *                    s = { ( 6 8 1 ) ( 9 4 3 ) ( 15 ) }, 
 *                    then r - s = { ( 1 2 ) }.
 *  Return: returns the difference of sets r and s
 ******************************************************************************/

Set  Set::operator - (const Set& s) const {
   Set result;
   for (int i = 0; i < size(); i++)  
      if (!(s % data[i])) {
         result.addTuple(data[i]);
      }

   return (result);
}



/*******************************************************************************
 *  Operator: ^, symetric difference (r ^ s)
 *  Comments: r^s returns the symmetric difference of sets r and s, which is 
 *            the set of tuples that appear in r or s, but not both. 
 *            I.e. it's the same as (r - s) + (s - r). 
 *            It's also the same as (r + s) - (r * s). 
 *            E.g. if r = { ( 1 2 ) ( 9 4 3 ) } and 
 *                    s = { ( 6 8 1 ) ( 9 4 3 ) ( 15 ) }, 
 *                    then r^s = { ( 1 2 ) ( 6 8 1 ) ( 15 ) }.
 *  Return: returns the symetric difference of sets r and s
 ******************************************************************************/

Set Set::operator ^ (const Set& s) const {
   return (((*this) - s) + (s - (*this)));
}



/*******************************************************************************
 *  Operator: /, Cartesian product of sets
 *  Comments: r / s returns the Cartesian product of sets r and s, which means 
 *            that you combine each tuple in r with each tuple in s. 
 *            For example, if r = {(1 2 3) (4 5)} and 
 *                            s = {(6 7 8) (9) (10 11)}, 
 *            then r / s = {(1 2 3 6 7 8) (1 2 3 9) (1 2 3 10 11) (4 5 6 7 8) 
 *                          (4 5 9) (4 5 10 11)}
 *  Return: returns the Cartesian product with other Set
 ******************************************************************************/

Set Set::operator / (const Set& s) const {
   Set result;
   Set R = (*this);
   Set S = (s);
   for (int kr = 0; kr < R.size(); kr++) {
      for (int ks = 0; ks < S.size(); ks++) {
         Tuple *T = new Tuple();
         (*T) = R[kr];
         (*T) += S[ks];
         result.addTuple(T);
      }
   }
   return (result);
}




/*******************************************************************************
 *  Operator: ^, Cartesian product of r with itself k times.
 *  Comments: r^k returns the Cartesian product of r with itself k times. 
 *            For example, if r = {(1 2 3) (4 5)} then
 *            r^3 = (r / r) / r 
 *                = {(1 2 3 1 2 3) (1 2 3 4 5) (4 5 1 2 3) (4 5 4 5)} / r
 *                = {(1 2 3 1 2 3 1 2 3) (1 2 3 1 2 3 4 5) (1 2 3 4 5 1 2 3) 
 *                   (1 2 3 4 5 4 5) (4 5 1 2 3 1 2 3) (4 5 1 2 3 4 5) 
 *                   (4 5 4 5 1 2 3) (4 5 4 5 4 5)}
 *  Return: returns the Cartesian product of r with itself k times.
 ******************************************************************************/

Set Set::operator ^ (const int k) const {
   Set tmp = (*this) / (*this);
   Set result = tmp / (*this);
   return (result);
}



/*******************************************************************************
 *  Operator: %, inclusion
 *  Comments: t % r returns true if tuple t is in set r and false otherwise.
 *  Return: true or false
 ******************************************************************************/

bool Set::operator % (const Tuple& t) const {
   Tuple tmp;
   std::copy(t.begin(), t.last(), back_inserter(tmp));
   Set s(*this);
   return ((s.findTuple(tmp) == (-1)) ? false : true);
}




/*******************************************************************************
 *  Operator: !, empty or not
 *  Comments: ! r returns true if set r is empty and false otherwise.
 *  Return: true or false
 ******************************************************************************/

bool Set::operator ! (void) const {
   return ((size==0) ? true : false);
}




/*******************************************************************************
 *  Operator: ==
 *  Comments: compare 2 sets (A==B)
 *  Return: true if the sets are equal
 ******************************************************************************/

bool Set::operator == (const Set& s) const
{
   // if size is diferent, nothing we can do
   if (size != s.size)
      return false;
   // compare every element
   for (size_t n = 0; n < s.size(); n++)
      if (data[n] != s.data[n])  // this is diferent
         return false;
   // all elements are the equivalents
   return true;
}
bool Set::operator != (const Set& s) const
{
   return (!((*this) == s));
}


/*******************************************************************************
 *  Operator: <
 *  Comments: compare 2 Sets (A<B). Usefull if we will sort between sets
 *  Return: true if 'this' is less than r
 ******************************************************************************/

bool Set::operator < (const Set& s) const
{
   // if size is diferent, nothing we can do
   if (size < s.size)
      return true;
   else if (s.size < size)
      return false;

   // The same size, then the comparisson is made between tuples of a set
   for (size_t n = 0; n < s.size(); n++)
      if (data[n] < s.data[n])   
         return true;
      else if (s.data[n] < data[n])
         return false;
   return false;
}
// (A > B)  <==> (B ↔ A)
bool Set::operator > (const Set& s) const
{
   return (s < (*this));
}
// (A <= B) <==> (A < B || A == B)
bool Set::operator <= (const Set& s) const
{
   return ((*this) < s) || ((*this) == s);
}
// (A >= B) <==> (A > B || A == B)
bool Set::operator >= (const Set& s) const
{
   return ((*this) > s) || ((*this) == s);
}



/*******************************************************************************
 *  Operator: []
 *  Comments: r[k] returns the kth tuple (assuming the tuples are sorted) of r 
 *            if k is at least 0 and strictly smaller than the size of the set 
 *            (so the entries in t are numbered starting from 0). If k is out 
 *            of bounds, then report an error message and abort the program.
 *  Return: returns the kth tuple
 ******************************************************************************/

Set::const_reference Set::operator[](int k) const 
{
   bool outOfBounds = (k >= 0 && k < size());
   if (!outOfBounds)
   {
      // report the error message
      std::cerr << "k = " << k << " is out of bounds." 
                << "Need to be between 0 and " << (size() - 1) << std::endl;
      // abort the program
      std::cerr << (*this) << std::endl;
      std::cerr << size() << std::endl;
      std::cerr << getCapacity() << std::endl;
      assert(outOfBounds);
   }

   return (*this).data[k]; 
}
Set::reference Set::operator[](int k)
{
   bool outOfBounds = (k >= 0 && k < size());
   if (!outOfBounds)
   {
      // report the error message
      std::cerr << "k = " << k << " is out of bounds." 
                << "Need to be between 0 and " << (size() - 1) << std::endl;
      // abort the program
      std::cerr << (*this) << std::endl;
      std::cerr << size() << std::endl;
      std::cerr << getCapacity() << std::endl;
      assert(outOfBounds);
   }

   return (*this).data[k]; 
}

/*******************************************************************************
 *  Operator: <<
 *  Comments: write out Sets 
 * -----------------------------------------------------------------------------
 *  The stream insertion and extraction operators write out and read in sets. 
 *  A set is formatted as follows: 
 *  - first an open curly brace: { followed by one or more spaces, 
 *  - then a space-separated list of tuples formatted as above 
 *    (the number of spaces between tuples can vary), 
 *  - then one or more spaces, 
 *  - then a close curly brace: }, e.g.:
 *              { ( 2 5 7 ) ( 1 7 ) ( 7 1 ) }
 ******************************************************************************/

ostream& operator<<(ostream& out, const Set* s)
{
   out << (*s);
   return out;
}

ostream& operator<<(ostream& out, const Set& s)
{
   out << Set::ASCII_BEGIN;
   out << Set::ASCII_SEPAR;
   std::copy(s.begin(), s.last(), ostream_iterator<Set::value_type>(out," "));
   out << Set::ASCII_END;
   return out;
}



/*******************************************************************************
 *  Operator: >>
 *  Comments: read in a set and tuples. See above about the format of a set.
 ******************************************************************************/

void Set::operator>>(char *buffer)
{
   assert (buffer != NULL);        // null is not permited 
   int length = strlen(buffer);
   assert (!(length < 3));         // minimum format "(i)"

   // Find Begin of Set
   char *first = std::find(buffer, buffer+length, (char)Set::ASCII_BEGIN);
   assert (*first == Set::ASCII_BEGIN);   // is not there, bad Set

   // Find End of Set
   char *last = std::find(buffer, buffer+length, (char)Set::ASCII_END);
   assert (*last == Set::ASCII_END);   // is not there, bad Set

   // something imposible, but, who knows
   assert (first != last);

   char *position = first + 1;

   // descending position into while
   // ------------------------------
   while (position < last) {
      assert ((*position >= '0' && *position <= '9') ||
              *position == Tuple::ASCII_BEGIN ||
              *position == Tuple::ASCII_END ||
              *position == Set::ASCII_SEPAR);

      // Find Begin of Tuple
      char *firstTuple = std::find(position, last, (char)Tuple::ASCII_BEGIN);
      if (*firstTuple != Tuple::ASCII_BEGIN)     // no tuple
         break;
      // Find End of Tuple
      char *lastTuple = std::find(position, last, (char)Tuple::ASCII_END);
      assert (*lastTuple == Tuple::ASCII_END);   // no end tuple (imposible)

      // imposible, but who knows
      if (first > last)
         break;

      int length = lastTuple - firstTuple + 1;
      // ( 1 2 3 ) 
      // 012345678
      char *newbuffer = new char(length+1);
      for (int i = 0; i <= length; i++) {
         newbuffer[i] = *(firstTuple++);
      }
      newbuffer[length] = 0;
      Tuple *T = new Tuple();
      (*T) >> newbuffer;

      addTuple(T);
      position = lastTuple +1;
   }
}


