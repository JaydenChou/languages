/******************************************************************************
 *  File: SetTestDriver.cpp
 *  
 *  Comments: Test Driver of class Set
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


bool TestFindTuple()
{
   std::cout << "findTuple" << std::endl;
   Tuple t1(3);
   t1.delElem(0);

   Tuple t2;
   t2.addElem(9);
   t2.addElem(4);
   t2.addElem(3);
   Tuple t3;
   t3 >> "(1 8 6)";
   Tuple t4;
   t4.addElem(15);

   Set r(t1);
   r.addTuple(t2);
   r.addTuple(t3);
   r.addTuple(t4);

   std::cout << "finding: [" << t4 << "] in " << r << std::endl;
   std::cout << "is in position = " << r.findTuple(t4) << std::endl;
   std::cout << std::endl;
   return true;
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
bool TestUnion()
{
   std::cout << "union (r + s)" << std::endl;
   Tuple t1(3);
   t1.delElem(0);

   Tuple t2;
   t2.addElem(9);
   t2.addElem(4);
   t2.addElem(3);
   Set r(t1);
   r.addTuple(t2);
   std::cout << "r = "           << r
             << " - size = "     << r.getSize()
             << " - capacity = " << r.getCapacity() << std::endl;

   Tuple t3;
   t3 >> "(1 8 6)";
   Tuple t4;
   t4.addElem(15);

   Set s(t3);
   s.addTuple(t2);
   s.addTuple(t4);

   std::cout << "s = "           << s
             << " - size = "     << s.getSize()
             << " - capacity = " << s.getCapacity() << std::endl;
   Set x;
   x = s + r;
   std::cout << "s + r = "       << x
             << " - size = "     << x.getSize()
             << " - capacity = " << x.getCapacity() << std::endl;

   return true;
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
bool TestIntersection()
{
   std::cout << "Intersection (r * s)" << std::endl;
   Tuple t1(3);
   t1.delElem(0);

   Tuple t2;
   t2.addElem(9);
   t2.addElem(4);
   t2.addElem(3);
   Set r(t1);
   r.addTuple(t2);
   std::cout << "r = "           << r
             << " - size = "     << r.getSize()
             << " - capacity = " << r.getCapacity() << std::endl;

   Tuple t3;
   t3 >> "(1 8 6)";
   Tuple t4;
   t4.addElem(15);

   Set s(t3);
   s.addTuple(t2);
   s.addTuple(t4);

   std::cout << "s = "           << s
             << " - size = "     << s.getSize()
             << " - capacity = " << s.getCapacity() << std::endl;
   Set x;
   x = s * r;
   std::cout << "s * r = "       << x
             << " - size = "     << x.getSize()
             << " - capacity = " << x.getCapacity() << std::endl;

   return true;
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
bool TestDifference()
{
   std::cout << "Difference (r - s)" << std::endl;
   Tuple t1(3);
   t1.delElem(0);

   Tuple t2;
   t2.addElem(9);
   t2.addElem(4);
   t2.addElem(3);
   Set r(t1);
   r.addTuple(t2);
   std::cout << "r = "           << r
             << " - size = "     << r.getSize()
             << " - capacity = " << r.getCapacity() << std::endl;

   Tuple t3;
   t3 >> "(1 8 6)";
   Tuple t4;
   t4.addElem(15);

   Set s(t3);
   s.addTuple(t2);
   s.addTuple(t4);

   std::cout << "s = "           << s
             << " - size = "     << s.getSize()
             << " - capacity = " << s.getCapacity() << std::endl;
   Set x;
   x = r - s;
   std::cout << "r - s = "       << x
             << " - size = "     << x.getSize()
             << " - capacity = " << x.getCapacity() << std::endl;

   return true;
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
bool TestSymetricDifference()
{
   std::cout << "Difference (r ^ s)" << std::endl;
   Tuple t1(3);
   t1.delElem(0);

   Tuple t2;
   t2.addElem(9);
   t2.addElem(4);
   t2.addElem(3);
   Set r(t1);
   r.addTuple(t2);
   std::cout << "r = "           << r
             << " - size = "     << r.getSize()
             << " - capacity = " << r.getCapacity() << std::endl;

   Tuple t3;
   t3 >> "(1 8 6)";
   Tuple t4;
   t4.addElem(15);

   Set s(t3);
   s.addTuple(t2);
   s.addTuple(t4);

   std::cout << "s = "           << s
             << " - size = "     << s.getSize()
             << " - capacity = " << s.getCapacity() << std::endl;
   Set x;
   x = r ^ s;
   std::cout << "r ^ s = "       << x
             << " - size = "     << x.getSize()
             << " - capacity = " << x.getCapacity() << std::endl;

   return true;
}
bool TestCartesianProductOfSets()
{
   std::cout << "CartesianProduct of Sets (r / s)" << std::endl;
   Tuple t1(4);
   t1.delElem(0);

   Tuple t2;
   t2.addElem(4);
   t2.addElem(5);

   Set r(t1);
   r.addTuple(t2);
   std::cout << "r = "           << r
             << " - size = "     << r.getSize()
             << " - capacity = " << r.getCapacity() << std::endl;

   Tuple t3;
   t3 >> "(8 7 6)";
   Tuple t4;
   t4.addElem(9);
   Tuple t5;
   t5 >> "(11 10)";

   Set s(t3);
   s.addTuple(t4);
   s.addTuple(t5);

   std::cout << "s = "           << s
             << " - size = "     << s.getSize()
             << " - capacity = " << s.getCapacity() << std::endl;
   Set x;
   x = r / s;
   std::cout << "r / s = "       << x
             << " - size = "     << x.getSize()
             << " - capacity = " << x.getCapacity() << std::endl;

   return true;
}

bool TestCartesianProductK()
{
   std::cout << "CartesianProduct of Set r itself k times" << std::endl;
   Tuple t1(4);
   t1.delElem(0);

   Tuple t2;
   t2.addElem(4);
   t2.addElem(5);

   Set r(t1);
   r.addTuple(t2);
   std::cout << "r = "           << r
             << " - size = "     << r.getSize()
             << " - capacity = " << r.getCapacity() << std::endl;

   Set x;
   int k = 3;
   x = r ^ k;
   std::cout << "r / k = "       << x << " where k = " << k
             << " - size = "     << x.getSize()
             << " - capacity = " << x.getCapacity() << std::endl;

   return true;
}
bool TestStreamIn() {
   Tuple *t1 = new Tuple(6);
   Set s1(t1);
   s1 >> "{ ( 1 2 3 ) ( 33 4 5)  (33 3) {33 3}}";
   std::cout << "s1 = "          << s1
             << " - size = "     << s1.getSize()
             << " - capacity = " << s1.getCapacity() << std::endl;
   return true;
}
bool TestDelTuple() {
   Tuple *t1 = new Tuple(6);
   Set s1(t1);
   s1 >> "{ ( 1 2 3 ) ( 33 4 5)  (33 3) {33 3}}";
   std::cout << "s1 = "          << s1
             << " - size = "     << s1.getSize()
             << " - capacity = " << s1.getCapacity() << std::endl;
   Tuple t2(6);
   std::cout << "deleting Tuple t2 = " << t2 << std::endl;
   s1.delTuple(t2);
   std::cout << "s1 = "          << s1
             << " - size = "     << s1.getSize()
             << " - capacity = " << s1.getCapacity() << std::endl;
   return true;
}
int main()
{
   // Creating a Tuple with 6 elements
   Tuple *t1 = new Tuple(6);
   std::cout << "address:  t1 = " << &t1
             << "\tvalues:   t1 = " << t1
             << "\tsize:     t1 = " << t1->getSize()
             << "\tcapacity: t1 = " << t1->getCapacity() << std::endl;
   std::cout << std::endl;

   Set s1(t1);
   std::cout << "s1 = "          << s1
             << " - size = "     << s1.getSize()
             << " - capacity = " << s1.getCapacity() << std::endl;


   std::cout << std::endl;
   Set s2(t1);
   std::cout << "s2 = "          << s2
             << " - size = "     << s2.getSize()
             << " - capacity = " << s2.getCapacity() << std::endl;


   std::cout << std::endl;
   Tuple *t3 = new Tuple(30);
   Set s3(t3);
   std::cout << "s3 = "          << s3
             << " - size = "     << s3.getSize()
             << " - capacity = " << s3.getCapacity() << std::endl;

   s3.addTuple(t1);
   s3.addTuple(t1);
   std::cout << std::endl;
   std::cout << "s3 = "          << s3
             << " - size = "     << s3.getSize()
             << " - capacity = " << s3.getCapacity() << std::endl;

   // std::cout << "s3 [4] = " << s3[4] << std::endl;

   std::cout << std::endl;
   std::cout << "Testing operator=" << std::endl;
   std::cout << "s1 = "          << s1
             << " - size = "     << s1.getSize()
             << " - capacity = " << s1.getCapacity() << std::endl;
   std::cout << "s3 = "          << s3
             << " - size = "     << s3.getSize()
             << " - capacity = " << s3.getCapacity() << std::endl;
   s1 = s3;
   std::cout << " --> s1 = s3 <--" << std::endl;
   std::cout << "s1 = "          << s1
             << " - size = "     << s1.getSize()
             << " - capacity = " << s1.getCapacity() << std::endl;
   std::cout << "s3 = "          << s3
             << " - size = "     << s3.getSize()
             << " - capacity = " << s3.getCapacity() << std::endl;


   s1 = s2;
   std::cout << " --> s1 = s2 <--" << std::endl;
   std::cout << "s1 = "          << s1
             << " - size = "     << s1.getSize()
             << " - capacity = " << s1.getCapacity() << std::endl;
   std::cout << "s2 = "          << s2
             << " - size = "     << s2.getSize()
             << " - capacity = " << s2.getCapacity() << std::endl;


   Set *s10 = new Set(5);
   // s10->addTuple(t1);
   Set s11(5);
   // t1->delElem(1);
   // s11.addTuple(t1);
   std::cout << "s10 = "         << s10
             << " - size = "     << s10->getSize()
             << " - capacity = " << s10->getCapacity() << std::endl;
   std::cout << "s11 = "         << s11
             << " - size = "     << s11.getSize()
             << " - capacity = " << s11.getCapacity() << std::endl;
   *s10 = s11;
   std::cout << " --> s10 = s11 <--" << std::endl;
   std::cout << "s10 = "         << s10
             << " - size = "     << s10->getSize()
             << " - capacity = " << s10->getCapacity() << std::endl;
   std::cout << "s11 = "         << s11
             << " - size = "     << s11.getSize()
             << " - capacity = " << s11.getCapacity() << std::endl;

   Set s12;
   std::cout << "s12 = "         << s12
             << " - size = "     << s12.getSize()
             << " - capacity = " << s12.getCapacity() << std::endl;
   *s10 = s12;
   std::cout << " --> s10 = s12 <--" << std::endl;
   std::cout << "s10 = "         << s10
             << " - size = "     << s10->getSize()
             << " - capacity = " << s10->getCapacity() << std::endl;

   s12 = s11;
   std::cout << " --> s12 = s11 <--" << std::endl;
   std::cout << "s12 = "         << s12
             << " - size = "     << s12.getSize()
             << " - capacity = " << s12.getCapacity() << std::endl;
   // Tuple *t7b = new Tuple(10);
   Tuple t7b(10);
   Set s7b(t7b);
   std::cout << "          s1 = " << s7b << std::endl;
   std::cout << "size:     s1 = " << s7b.getSize() << std::endl;
   std::cout << "capacity: s1 = " << s7b.getCapacity() << std::endl;

   if (TestFindTuple() == true) {
      std::cout << "findTuple......OK" << std::endl;
   }
   if (TestUnion() == true) {
      std::cout << "union (r+s)....OK" << std::endl;
   }
   if (TestIntersection() == true) {
      std::cout << "intersection (r*s)....OK" << std::endl;
   }
   if (TestDifference() == true) {
      std::cout << "Difference (r-s)....OK" << std::endl;
   }
   if (TestSymetricDifference() == true) {
      std::cout << "Difference (r^s)....OK" << std::endl;
   }
   if (TestCartesianProductOfSets() == true) {
      std::cout << "CartesianProductOfSets (r/s)....OK" << std::endl;
   }
   if (TestCartesianProductK() == true) {
      std::cout << "CartesianProductK (r/k)....OK" << std::endl;
   }
   if (TestStreamIn() == true) {
      std::cout << "Strem in << ....OK" << std::endl;
   }
   if (TestDelTuple() == true) {
      std::cout << "delTuple << ....OK" << std::endl;
   }
}
