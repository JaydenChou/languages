/******************************************************************************
 *   File: BbbDate.h
 *   
 *   Comments: Declaracion de clase para manejo de fechas
 *   
 *   Compilers: 
 *    1) Linux Argon 2.4.20-8smp #1 SMP Thu Mar 13 17:45:54 EST 2003 
 *       i686 i686 i386 GNU/Linux
 *       gcc version 3.2.2 20030222 (Red Hat Linux 3.2.2-5)
 *    2) Linux acer 2.6.18-foxcastex.2798.fc6 #2 SMP Sat Feb 24 22:25:44 ART 2007 
 *       x86_64 x86_64 x86_64 GNU/Linux
 *       gcc version 4.1.1 20061011 (Red Hat 4.1.1-30)
 *   
 ******************************************************************************/

#ifndef __BBB_DATE_H__
#define __BBB_DATE_H__

#include <iostream>
#include <iomanip.h>
#include <strstream>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include "BbbString.h"


#ifdef OTL_ORA8
#include <iostream>
#define OTL_VALUE_TEMPLATE_ON // Turn on otl_value<T>
#define OTL_USER_DEFINED_STRING_CLASS_ON
#include "BbbString.h"
#define OTL_ADD_NULL_TERMINATOR_TO_STRING_SIZE
#define USER_DEFINED_STRING_CLASS BbbString
#include "otlv4.h" // include the OTL 4 header file
#endif



/** BbbDate
 * Obtiene la fecha/hora/centesimas del SO en diferentes formatos
 * @author jfuchs 
 */

class BbbDate {
   // Atributos privados
   /// First Weeday of YearMin
   enum { Sunday = 0, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday } enumWeekDay;

   static const int firstWeekDay = 8;
   /// Min Year
   static const int YearMin = 1900;
   /// Max Year
   static const int YearMax = 2099;
   /// Estructura de Fecha
   struct timeval    tvDate;
   /// Centesimas: CC
   BbbString          C;       
   /// Date: YYYYMMDD
   BbbString          D;       
   /// Time: HHMISS
   BbbString          T;       
   /// DateTime: YYYYMMDDHHMISS
   BbbString          DT;      
   /// DateTimeCent: YYYYMMDDHHMISSCC
   BbbString          DTC;     
   
   /**
    * Construir una fecha
    * @param pFmt
    * 
    * @return BbbString
    */
   BbbString  Build(char *pFmt)
   {
      if (!gettimeofday(&tvDate, NULL))
         setOk();

      char szBuf[128];
      strftime(szBuf, 128, pFmt, localtime(&(tvDate.tv_sec)));
      assert(strlen(szBuf));

      // Microsegundos / 10000 = Centesimas
      std::strstream sAux;
      sAux << tvDate.tv_usec/10000 << '\0';
      C = sAux.str();

      BbbString  sBuf;
      sBuf = szBuf;
      return sBuf;
   }
public:
   /// string para salidas
   BbbString    Str;

   bool status;

   void setOk() { status = true; }

   /**@name constructors */
   //@{
   /// Default, obtiene la fecha/hora/Centesimas con formatos predefinidos
   BbbDate()
   {
      status = false;
      // Dia: YYYYMMDD
      D = Build((char*) "%Y%m%d");
      // Hora: HHMISS
      T = Build((char*) "%H%M%S");
      // Dia+Hora: YYYYMMDDHHMISS
      DT = D;
      DT += T;

      // Microsegundos / 10000 = Centesimas
      std::strstream sAux;
      sAux << setfill('0') << setiosflags(ios::left) << setw(2) << tvDate.tv_usec/10000 << '\0';
      C = sAux.str();

      // Dia+Hora+Centesimas: YYYYMMDDHHMISSCC
      DTC = DT;
	  DTC += ".";
      DTC += C;

      Str = DTC.c_str();
      assert(Str.length());
   }

   time_t toTime() { return tvDate.tv_sec; }

   /// Constructor con formato especificado

   /**
    * Construir una fecha en base a un formato f
    * @param f formato
    */
   BbbDate(char * f)
   {
      // Si el formato es Nulo, obtiene una fecha con formato generico
	  if (f != NULL) {
		  Str = Build(f).c_str();
	  } else {
		  Str = Build((char *)"%Y-%m-%d %H:%M:%S").c_str();
		  Str += ".";
		  Str += C.c_str();
	  }
      // Nunca deberia suceder que el size es 0
      assert(Str.length());
   }
   

   /**
    * Constructor de fecha
    * @param year
    * @param month
    * @param day
    */
   BbbDate(int year, int month, int day)
   {
      if (validar(year, month, day) == true)
      {
         setOk();
         /// Centesimas: CC
         C = "00";       
         /// Date: YYYYMMDD
         {
            std::strstream sAux;
            sAux << year << month << day << '\0';
            D = sAux.str();       
         }
         /// Time: HHMISS
         T = "000000";
         /// DateTime: YYYYMMDDHHMISS
         DT = D;
         DT += T;
         /// DateTimeCent: YYYYMMDDHHMISSCC
         DTC = DT;
         DTC += C;
      }
   }
   //@}

   // Metodos publicos
   /**@name public methods */
   //@{
   /// Valida que la fecha se encuentre entre los rangos permitidos de Year,
   // y si es una fecha valida, segun Calendario Gregoriano
   inline bool validar(int year, int month, int day)
   {
      bool r = false;
      do {
         // Valido el year, estos son los limites
         if (year < YearMin || year > YearMax)
            break;

         // Valido el Mes
         if (month < 1 || month > 12)
            break; // ingreso un mes, invalido!

         // Valido el dia
         if (day < 1 || day > 31)
            break; // Ingreso un dia, invalido!

         // Valido los Dias de meses cortos
         if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
            break; // Ese mes no tiene esa cant. de dias

         // Bisiestos con 28 dias en febrero
         if (month == 2 && ((!(year % 4) && day > 29) || (year % 4) && day > 28))
            break; // Aqui en la tierra este year, cuando es febrero no tiene esa cant. de dias

         r = true;
         break;
      } while(1);

      return r;
   }

   /**
    * Retorna el dia de la semana.
    * 0: Sunday - 1: Monday - 2: Tuesday - 3: Wednesday - 4: Thursday - 5: Friday - 6: Saturday
    * @param year
    * @param month
    * @param day
    * 
    * @return int
    */
   inline int weekDay(int year, int month, int day)
   {
      int d, m, y, limitemonth;
      // El (primer dia de la semana + 1) en 'YearMin'
      d = firstWeekDay;
      for (y = YearMin; y <= year; y++)
      {
         if (y == year)
         {
            // Tengo que saber cuantos dias transcurrieron hasta el mes
            // anterior, del año que me piden
            limitemonth = (month - 1);
         } else {
            // Los demas años con 12 meses
            limitemonth = 12;
         }
         // Ahora el mes
         for (m = 1; m <= limitemonth; m++)
         {
            // En abril, Junio, Septiembre y Noviembre siempre tengo
            // 30 dias
            if (m == 4 || m == 6 || m == 9 || m == 11)
            {
               d += 30;
            } else if (m == 2) // Si es bisiesto
            {
               // el año es divisible por 4: febrero tiene 29 dias
               if (!(y % 4))
                  d += 29;
               else
                  d += 28; // Febrero con 28 dias
            } else {
               // es un mes con 31 dias
               d += 31;
            }
         }
      }
      // ahora tengo a 'd' con tantos dias antes de llegar al
      // corriente mes/año, entonces le sumo los dias 
      // del corriente mes
      d += day;

      // Esto me da el resto que es igual al dia de la semana
      // el numero de la semana entre 0 y 6
      if (!(d % 7))
         return 6;      
      else
         return ((d % 7) - 1);
   }

   /// return Date
   BbbString D2Str()    { return D;   }
   /// return Time
   BbbString T2Str()    { return T;   }
   /// return Date-Time
   BbbString DT2Str()   { return DT;  }
   /// return Date-Time-Cent.
   BbbString DTC2Str()  { return DTC; }
   /// return Cent
   int Cent()  { return atoi(C.c_str()); }
   //@}

   // Operador publico friend
   friend std::ostream& operator << ( std::ostream& os, BbbDate d ) { os << d.Str; return os; }
};

class TypeBbbDate {
private:
  bool _null;
  bool _sysdate;
public:

  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
  unsigned int second;

  TypeBbbDate() { setNull(); }

  const bool isNotNull() const { return (_null == false); }
  const bool isNull() const { return (_null == true); }
  const bool isSysdate() const { return (_sysdate == true); }
  void setNull() { 
      _null = true; 
      _sysdate = false; 
      year = month = day = hour = minute = second = 0;
  }
  void setNoNull() { _null = false; }
  void setSysdate() { setNoNull(); _sysdate = true; }

  friend std::ostream& operator << ( std::ostream& os, const TypeBbbDate& d ) { 
      if (d.isNull()) {
	  os << "NULL";
      } else if (d.isSysdate()) {
	  os << "SYSDATE";
      } else {
	  os << d.year
	     << setfill('0') << setiosflags(ios::right) << setw(2) 
	     << d.month
	     << setfill('0') << setiosflags(ios::right) << setw(2) 
	     << d.day
	     << setfill('0') << setiosflags(ios::right) << setw(2) 
	     << d.hour
	     << setfill('0') << setiosflags(ios::right) << setw(2) 
	     << d.minute
	     << setfill('0') << setiosflags(ios::right) << setw(2) 
	     << d.second
	     << setfill(' ');
      }
      return os; 
  }

  TypeBbbDate& operator=(const time_t& rt) {
      if (rt <= 0) {
	  setNull();
      }
      setNoNull();
      struct tm *ptm = gmtime(&rt);
      year = ptm->tm_year + 1900;
      month = ptm->tm_mon + 1;
      day = ptm->tm_mday;
      hour = ptm->tm_hour;
      minute = ptm->tm_min;
      second = ptm->tm_sec;
      return (*this);
   }
  TypeBbbDate& operator=(const TIME_ST& lt) {
      if (lt.gmt <= 0) {
	  setNull();
      }
      setNoNull();
      time_t gmt = (time_t)lt.gmt + ((int)lt.timezone)*3600;
      struct tm *ptm = gmtime(&gmt);
      year = ptm->tm_year + 1900;
      month = ptm->tm_mon + 1;
      day = ptm->tm_mday;
      hour = ptm->tm_hour;
      minute = ptm->tm_min;
      second = ptm->tm_sec;
      return (*this);
   }

   TypeBbbDate& operator=(BbbDate& sd) {
       setNoNull();
       time_t gmt = sd.toTime();
       struct tm *ptm = gmtime(&gmt);
       year = ptm->tm_year + 1900;
       month = ptm->tm_mon + 1;
       day = ptm->tm_mday;
       hour = ptm->tm_hour;
       minute = ptm->tm_min;
       second = ptm->tm_sec;
       return (*this);
  }


  bool operator == (const TypeBbbDate& that) const {
      return (toTime() == that.toTime());
  }

  bool operator == (BbbDate& sd) const {
      return (toTime() == sd.toTime());
  }

  bool operator == (time_t t) const {
      return (toTime() == t);
  }

  bool operator != (time_t t) const {
      return (toTime() != t);
  }

  bool operator < (BbbDate& sd) const {
      return (toTime() < sd.toTime());
  }

  bool operator <= (BbbDate& sd) const {
      return (toTime() <= sd.toTime());
  }

  bool operator >= (BbbDate& sd) const {
      return (toTime() >= sd.toTime());
  }

  bool operator > (BbbDate& sd) const {
      return (toTime() > sd.toTime());
  }

  time_t toTime() const 
  {
     if (isNull()) {
	 return ((time_t)-1);
     } 
     struct tm tm0;
     tm0.tm_year = year - 1900;
     tm0.tm_mon = month - 1;
     tm0.tm_mday = day;
     tm0.tm_hour = hour;
     tm0.tm_min = minute;
     tm0.tm_sec = second;
     tm0.tm_isdst = -1;
     return (mktime(&tm0));
  }

  time_t toGmtTime(int timezone) const 
  {
     if (isNull()) {
	 return ((time_t)-1);
     } 
     struct tm tm0;
     tm0.tm_year = year - 1900;
     tm0.tm_mon = month - 1;
     tm0.tm_mday = day;
     tm0.tm_hour = hour;
     tm0.tm_min = minute;
     tm0.tm_sec = second;
     tm0.tm_isdst = -1;
     return (mktime(&tm0) + (timezone*3600));
  }

#ifdef OTL_ORA8

  void toOtl(otl_value<otl_datetime>& otlDate) const {
      if (isNull()) {
	  otlDate.set_null();
      } else {
	  otlDate.set_non_null();
	  otlDate.v.year = year;
	  otlDate.v.month = month;
	  otlDate.v.day = day;
	  otlDate.v.hour = hour;
	  otlDate.v.minute = minute;
	  otlDate.v.second = second;
      }
  }

  TypeBbbDate& operator=(const otl_value<otl_datetime>& otlDate) {

      if (otlDate.is_null()) {
	  setNull();
      } else {
	  setNoNull();
	  year = otlDate.v.year;
	  month = otlDate.v.month;
	  day = otlDate.v.day;
	  hour = otlDate.v.hour;
	  minute = otlDate.v.minute;
	  second = otlDate.v.second;
      }
      return (*this);
  }
#endif

};


#endif // __BBB_DATE_H__
