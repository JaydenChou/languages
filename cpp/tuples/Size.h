/*******************************************************************************
 *  File: Size.h
 *  
 *  Comments: Declaration and inner implementation of class Size
 *            The lengths of your arrays are dynamically adjusted. There are 
 *            two different notions of array size. 
 *            - The first is the number of items stored:
 *                size_t Size::_;
 *              Every time this value will change. 
 *            - The second notion of size is *related* to the amount of 
 *              memory is allocated for the array. 
 *                size_t Size::capacity;
 *              This value (we'll refer to it as *Capacity*) should be 
 *              initialized to 4, and doubled each time you run out of room. 
 *            If the number of entries stored in the array drops below 
 *            Capacity/4, 
 *            then you should cut Capacity in half.
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

#ifndef __SIZE_H__
#define __SIZE_H__
#include <algo.h>

// Inner class (Size)
class Size {
private:
   // Number of items stored
   size_t _;
   // Number of items reserved
   size_t capacity;
   // Factor of increment in capacity
   static const size_t INCREMENT = 2;
   static const size_t DECREMENT = 2;

public:
   Size(int SIZE = 4) { _ = 0; capacity = SIZE; }

   // operator Default function
   const size_t operator()(void) const  { return(_); }

   // Prefix: s++
   Size operator ++(int) {
      Size result(*this);
      if (_ >= capacity) 
         capacity *= INCREMENT;
      _++;
      return(result);
   }
   // Prefix: ++s
   Size& operator ++(void) {
      if (_ >= capacity) 
         capacity *= INCREMENT;
      _++;
      return(*this);
   }
   // Prefix: s--
   Size operator --(int) {
      Size result(*this);
      if (_ > 0) _--;
      if (_ < (capacity/4))
         capacity /= DECREMENT;
      return(result);
   }
   // Prefix: --s
   Size& operator --(void) {
      // TODO: what happens if drops bellow 0 --> assert()
      //       for the momment, only decrement if (_>0)
      if (_ > 0) _--;
      if (_ < (capacity/4))
         capacity /= DECREMENT;
      return(*this);
   }

   Size& operator += (const Size& t) {
      if (_ >= capacity) 
         capacity *= INCREMENT;
      _ += t._;
      return(*this);
   }

   Size& operator -= (const Size& t) {
      // TODO: what happens if drops bellow 0 --> assert()
      //       for the momment, only decrement if (_>0)
      if (_ > t._) _ -= t._;
      if (_ < (capacity/4))
         capacity /= INCREMENT;
      return(*this);
   }

   Size& operator = (const Size& t) {
      _ = t._;
      capacity = t.capacity;
      return(*this);
   }

   Size& operator = (size_t p) {
      while (p >= capacity) 
         capacity *= INCREMENT;
      while (p < (capacity/4))
         capacity /= DECREMENT;
      _ = p;
      return(*this);
   }
   Size& operator = (int p) {
      size_t pi = p;
      while (pi >= capacity) 
         capacity *= INCREMENT;
      while (pi < (capacity/4))
         capacity /= DECREMENT;
      _ = pi;
      return(*this);
   }
   bool operator == (const Size& s) const {
      return (_ == s._);
   }
   bool operator != (const Size& s) const {
      return (_ != s._);
   }
   bool operator < (const Size& s) const {
      return (_ < s._);
   }

   const size_t getCapacity() const { return capacity; }
   const size_t getIncrement() const { return INCREMENT; }

};

#endif // __SIZE_H__
