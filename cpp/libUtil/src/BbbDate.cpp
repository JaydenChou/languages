/******************************************************************************
 *   File: BbbDate.cpp
 *   
 *   Comments: Uso de BbbDate.h, un simple main test para poder compilarlo
 *   
 *   Compilers: 
 *    1) Linux Argon 2.4.20-8smp #1 SMP Thu Mar 13 17:45:54 EST 2003 
 *       i686 i686 i386 GNU/Linux
 *       gcc version 3.2.2 20030222 (Red Hat Linux 3.2.2-5)
 *    2) Linux acer 2.6.18-foxcastex.2798.fc6 #2 SMP Sat Feb 24 22:25:44 ART 2007 
 *       x86_64 x86_65 x86_64 GNU/Linux
 *       gcc version 5.1.1 20061011 (Red Hat 4.1.1-30)
 *   
 ******************************************************************************/
#ifdef _BBBDATE_TEST_
#include <strstream>

#ifndef _ALLOW_STD_TYPES_
#define _ALLOW_STD_TYPES_
#endif
#include "BbbDate.h"

/**
 * Test
 * 
 * @return int
 */
// How to test it:
// $ g++  -Wno-deprecated -m32 -I./ -I../../shared/h -DLINUX -D_BBBDATE_TEST_ BbbDate.cpp -o BbbDateTest
// $ ./BbbDateTest

int main()
{
    TypeBbbDate bbbdate;
    // 05-09-2007 17:05:00
    // 2007092116181940
    std::cout << "year: "   ; std::cin >> bbbdate.year;
    std::cout << "month: "  ; std::cin >> bbbdate.month;
    std::cout << "day: "    ; std::cin >> bbbdate.day;
    std::cout << "hour: "   ; std::cin >> bbbdate.hour;
    std::cout << "minute: " ; std::cin >> bbbdate.minute;
    std::cout << "second: " ; std::cin >> bbbdate.second;
    bbbdate.setNoNull();

    std::cout << "BbbDate = " << bbbdate << " - ";
    time_t ltimet = bbbdate.toTime();
    time_t gmtimet = bbbdate.toGmtTime();
    std::cout << "local = " << ltimet << " - ";
    std::cout << "gmt = " << gmtimet << std::endl;

    TypeBbbDate local;
    local= ltimet;
    TypeBbbDate gmt;
    gmt = gmtimet;

    std::cout << "local BbbDate = " << local << " - ";
    std::cout << "gmt BbbDate = " << gmt << std::endl;

    TypeBbbDate paracompararsysdate;
    TypeBbbDate fVigencia;
    fVigencia = bbbdate;
    paracompararsysdate = fVigencia.toGmtTime();
    std::cout << "paracompararsysdate from gmt = " << paracompararsysdate 
	<< " (time_t) L: " << paracompararsysdate.toTime() << " - "
	<< " (time_t) GMT: " << paracompararsysdate.toGmtTime() << std::endl;

    BbbDate sysdate;
    std::cout << "sysdate = " << sysdate << " (time_t) " << sysdate.toTime() << std::endl;
    if (fVigencia < sysdate)  { std::cout << "fv < sysdate" << std::endl; }
    if (fVigencia <= sysdate) { std::cout << "fv <= sysdate" << std::endl; }
    if (fVigencia == sysdate) { std::cout << "fv == sysdate" << std::endl; }
    if (fVigencia >= sysdate) { std::cout << "fv >= sysdate" << std::endl; }
    if (fVigencia > sysdate)  { std::cout << "fv > sysdate" << std::endl; }
    std::cout << endl;
    if (paracompararsysdate < sysdate)  { std::cout << "fvGmt < sysdate" << std::endl; }
    if (paracompararsysdate <= sysdate) { std::cout << "fvGmt <= sysdate" << std::endl; }
    if (paracompararsysdate == sysdate) { std::cout << "fvGmt == sysdate" << std::endl; }
    if (paracompararsysdate >= sysdate) { std::cout << "fvGmt >= sysdate" << std::endl; }
    if (paracompararsysdate > sysdate)  { std::cout << "fvGmt > sysdate" << std::endl; }

    long timet;
    std::cout << "ingresar time_t: " ; std::cin >> timet;
    TypeBbbDate d1;
    d1 = timet;
    std::cout << "d1 = " << d1 << std::endl;
    
    return 0;
}



#endif // _BBBDATE_TEST_
