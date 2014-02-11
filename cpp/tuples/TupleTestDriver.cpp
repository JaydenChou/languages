/******************************************************************************
 *  File: TupleTestDriver.cpp
 *  
 *  Comments: Test Driver of class Tuple
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
#include "Tuple.h"
#include "Set.h"


int main()
{
   // Creating a Tuple with 6 elements
   Tuple *t1 = new Tuple(6);
   std::cout << "address:  t1 = " << &t1 << std::endl;
   std::cout << "values:   t1 = " << t1 << std::endl;
   std::cout << "size:     t1 = " << t1->getSize() << std::endl;
   std::cout << "capacity: t1 = " << t1->getCapacity() << std::endl;
   std::cout << std::endl;

   // Adding an element
   t1->addElem(14);
   std::cout << "address:  t1 = " << &t1 << std::endl;
   std::cout << "values:   t1 = " << t1 << std::endl;
   std::cout << "size:     t1 = " << t1->getSize() << std::endl;
   std::cout << "capacity: t1 = " << t1->getCapacity() << std::endl;
   std::cout << std::endl;


   // Adding another element
   t1->addElem(3);
   std::cout << "address:  t1 = " << &t1 << std::endl;
   std::cout << "values:   t1 = " << t1 << std::endl;
   std::cout << "size:     t1 = " << t1->getSize() << std::endl;
   std::cout << "capacity: t1 = " << t1->getCapacity() << std::endl;
   std::cout << std::endl;

   // Creating Tuple 2
   Tuple *t2 = new Tuple(56);
   std::cout << "address:  t2 = " << &t2 << std::endl;
   std::cout << "values:   t2 = " << t2 << std::endl;
   std::cout << "size:     t2 = " << t2->getSize() << std::endl;
   std::cout << "capacity: t2 = " << t2->getCapacity() << std::endl;
   std::cout << std::endl;


   t2->addElem(35);
   t2->addElem(33);
   t1->addElem(30);
   t1->addElem(35);
   t1->addElem(33);
   t2->addElem(30);

   std::cout << "address:  t1 = " << &t1 << std::endl;
   std::cout << "values:   t1 = " << t1 << std::endl;
   std::cout << "size:     t1 = " << t1->getSize() << std::endl;
   std::cout << "capacity: t1 = " << t1->getCapacity() << std::endl;
   std::cout << std::endl;

   std::cout << "address:  t2 = " << &t2 << std::endl;
   std::cout << "values:   t2 = " << t2 << std::endl;
   std::cout << "size:     t2 = " << t2->getSize() << std::endl;
   std::cout << "capacity: t2 = " << t2->getCapacity() << std::endl;
   std::cout << std::endl;


   int x = 33;
   std::cout << "finding: [" << x << "] in " << t1 << std::endl;
   std::cout << "is in position = " << t1->findElem(x) << std::endl;
   std::cout << std::endl;
   std::cout << "finding: [" << x << "] in " << t2 << std::endl;
   std::cout << "is in position = " << t2->findElem(x) << std::endl;
   std::cout << std::endl;

   x = 99;
   std::cout << "finding: [" << x << "] in " << t2 << std::endl;
   std::cout << "is in position = " << t2->findElem(x) << std::endl;
   std::cout << std::endl;

   x = 33;
   std::cout << "deleting: [" << x << "]" << std::endl;
   std::cout << "in  t1 = " << t1->findElem(x) << std::endl;
   std::cout << "t1 = " << t1 << std::endl;
   t1->delElem(x);
   std::cout << "t1 = " << t1 << std::endl;
   std::cout << std::endl;

   std::cout << "deleting: [" << x << "]" << std::endl;
   std::cout << "in  t2 = " << t2->findElem(x) << std::endl;
   std::cout << "t2 = " << t2 << std::endl;
   t2->delElem(x);
   std::cout << "t2 = " << t2 << std::endl;
   std::cout << std::endl;


   Tuple *t3 = new Tuple(3);
   std::cout << "address:  t3 = " << &t3 << std::endl;
   std::cout << "values:   t3 = " << t3 << std::endl;
   std::cout << "size:     t3 = " << t3->getSize() << std::endl;
   std::cout << "capacity: t3 = " << t3->getCapacity() << std::endl;
   std::cout << std::endl;

   Tuple *t4 = new Tuple(4);
   std::cout << "address:  t4 = " << &t4 << std::endl;
   std::cout << "values:   t4 = " << t4 << std::endl;
   std::cout << "size:     t4 = " << t4->getSize() << std::endl;
   std::cout << "capacity: t4 = " << t4->getCapacity() << std::endl;
   std::cout << std::endl;

   t3 = t4;
   std::cout << "address:  t3 = " << &t3 << std::endl;
   std::cout << "values:   t3 = " << t3 << std::endl;
   std::cout << "size:     t3 = " << t3->getSize() << std::endl;
   std::cout << "capacity: t3 = " << t3->getCapacity() << std::endl;
   std::cout << std::endl;


   Tuple *t5 = new Tuple(5);
   std::cout << "address:  t5 = " << &t5 << std::endl;
   std::cout << "values:   t5 = " << t5 << std::endl;
   std::cout << "size:     t5 = " << t5->getSize() << std::endl;
   std::cout << "capacity: t5 = " << t5->getCapacity() << std::endl;
   std::cout << std::endl;

   t5 = t4;
   std::cout << "address:  t5 = " << &t5 << std::endl;
   std::cout << "values:   t5 = " << t5 << std::endl;
   std::cout << "size:     t5 = " << t5->getSize() << std::endl;
   std::cout << "capacity: t5 = " << t5->getCapacity() << std::endl;
   // std::cout << "t5[2] = " << t5[2] << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;



   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   
   Tuple t6(6);
   t6 >> "( 1234 2384 312 1 094572 39 )";
   t6 >> "(99)";
   t6 >> "( 7 8 9 10 11 55 64 )";
   t6 >> "( 7 8 9 10 11 55 64 )";
   t6 >> "( 7 8 9 10 11 55 64 )";
   std::cout << "t6 = " << t6 << std::endl;
   std::cout << "size:     t6 = " << t6.getSize() << std::endl;
   std::cout << "capacity: t6 = " << t6.getCapacity() << std::endl;
   std::cout << std::endl;
   std::cout << "t6[3] = " << t6[3] << std::endl;
   t6.delElem(1234);
   t6.delElem(2384);
   t6.delElem(312);
   t6.delElem(6);
   t6.delElem(5);
   t6.delElem(1);
   t6.delElem(39);
   t6.delElem(7);
   t6.delElem(8);
   t6.delElem(9);
   t6.delElem(10);
   t6.delElem(7); 
   t6.delElem(8); 
   t6.delElem(9); 
   t6.delElem(10); 
   t6.delElem(11); 
   t6.delElem(55); 
   t6.delElem(64); 
   t6.delElem(7); 
   t6.delElem(8); 
   t6.delElem(9); 
   t6.delElem(10); 
  t6.delElem(11); 
   std::cout << "t6 = " << t6 << std::endl;
   std::cout << "size:     t6 = " << t6.getSize() << std::endl;
   std::cout << "capacity: t6 = " << t6.getCapacity() << std::endl;
  t6.delElem(9); 
  t6.delElem(9); 
  t6.delElem(9); 
  t6.delElem(9); 
  t6.delElem(9); 
  t6.delElem(99); 
  t6.delElem(4); 
  t6.delElem(39); 
   std::cout << "t6 = " << t6 << std::endl;
   std::cout << "size:     t6 = " << t6.getSize() << std::endl;
   std::cout << "capacity: t6 = " << t6.getCapacity() << std::endl;
#if 0
   std::cout << "t6[8] = " << t6[8] << std::endl;
#endif

   Tuple t7a(10);
   Tuple t7b(10);
   Tuple t7c(11);
   std::cout << "t7a   = " << t7a << std::endl;
   std::cout << "t7b   = " << t7b << std::endl;
   std::cout << "t7c   = " << t7c << std::endl;
   if (t7a == t7b)
      std::cout << "t7a EQUAL t7b" << std::endl;
   else
      std::cout << "t7a NOT EQUAL t7b" << std::endl;
   if (t7a == t7c)
      std::cout << "t7a EQUAL t7c" << std::endl;
   else
      std::cout << "t7a NOT EQUAL t7c" << std::endl;
   if (t7a < t7c)
      std::cout << "t7a IS LESS THAN t7c" << std::endl;
   if (t7a > t7c)
      std::cout << "t7a IS GREATHER THAN t7c" << std::endl;
   if (t7c < t7b)
      std::cout << "t7c IS LESS THAN t7b" << std::endl;
   if (t7c > t7b)
      std::cout << "t7c IS GREATHER THAN t7b" << std::endl;


   if (t7a <= t7c)
      std::cout << "t7a IS LESS OR EQUAL THAN t7c" << std::endl;
   if (t7a >= t7c)
      std::cout << "t7a IS GREATHER OR EQUAL THAN t7c" << std::endl;
   if (t7c <= t7b)
      std::cout << "t7c IS LESS OR EQUAL THAN t7b" << std::endl;
   if (t7c >= t7b)
      std::cout << "t7c IS GREATHER OR EQUAL THAN t7b" << std::endl;


   // delete some elements
   t7c.delElem(10);
   t7c.delElem(9);
   t7c >> "(10)";
   std::cout << "t7a   = " << t7a << std::endl;
   std::cout << "t7b   = " << t7b << std::endl;
   std::cout << "t7c   = " << t7c << std::endl;
   if (t7a == t7b)
      std::cout << "t7a EQUAL t7b" << std::endl;
   else
      std::cout << "t7a NOT EQUAL t7b" << std::endl;
   if (t7a == t7c)
      std::cout << "t7a EQUAL t7c" << std::endl;
   else
      std::cout << "t7a NOT EQUAL t7c" << std::endl;
   if (t7a < t7c)
      std::cout << "t7a IS LESS THAN t7c" << std::endl;
   if (t7a > t7c)
      std::cout << "t7a IS GREATHER THAN t7c" << std::endl;
   if (t7c < t7b)
      std::cout << "t7c IS LESS THAN t7b" << std::endl;
   if (t7c > t7b)
      std::cout << "t7c IS GREATHER THAN t7b" << std::endl;


   if (t7a <= t7c)
      std::cout << "t7a IS LESS OR EQUAL THAN t7c" << std::endl;
   if (t7a >= t7c)
      std::cout << "t7a IS GREATHER OR EQUAL THAN t7c" << std::endl;
   if (t7c <= t7b)
      std::cout << "t7c IS LESS OR EQUAL THAN t7b" << std::endl;
   if (t7c >= t7b)
      std::cout << "t7c IS GREATHER OR EQUAL THAN t7b" << std::endl;


   std::cout << "---------------------------------------------------" <<
      std::endl;

   Set s1(t7b);
   std::cout << "          s1 = " << s1 << std::endl;
   std::cout << "size:     s1 = " << s1.getSize() << std::endl;
   std::cout << "capacity: s1 = " << s1.getCapacity() << std::endl;
}
