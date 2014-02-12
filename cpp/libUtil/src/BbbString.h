/******************************************************************************
 *   File: BbbString.h
 *   
 *   Comments: Super rapida clase string, para no usar la std::string de la STL
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
#ifndef __BBBSTRING_H__
#define __BBBSTRING_H__

#include <iostream>
#include <string.h>
#include <assert.h>

#include <bbb_type.h>

#define BBBSTRINGMAX 1024

class BbbString {
public:

  BbbString(void)
  {
    buf[0]=0;
  }


  BbbString(const char* s)
  {
    assert (strlen(s) < BBBSTRINGMAX);
#if defined(_MSC_VER) 
#if (_MSC_VER >= 1400)
    strcpy_s(buf,sizeof(buf),s);
#else
    strcpy(buf,s);
#endif
#else
    strcpy(buf,s);
#endif
  }

  BbbString(const BbbString& s)
  {
      assert (s.length() < BBBSTRINGMAX);
#if defined(_MSC_VER) 
#if (_MSC_VER >= 1400)
    strcpy_s(buf,sizeof(buf),s.buf);
#else
    strcpy(buf,s.buf);
#endif
#else
    strcpy(buf,s.buf);
#endif
  }

  ~BbbString(){}

  BbbString& operator=(const char* s)
  {
    assert (strlen(s) < BBBSTRINGMAX);
#if defined(_MSC_VER) 
#if (_MSC_VER >= 1400)
    strcpy_s(buf,sizeof(buf),s);
#else
    strcpy(buf,s);
#endif
#else
    strcpy(buf,s);
#endif
    return *this;
  }

  void copy(const char* s, int l) {
	  int ls = strlen(s);
	  int len = (ls < l) ? ls : l;
	  strncpy(buf, s, len);
	  buf[len] = 0;
  }

  void copy(const char* s) {
	  copy(s, BBBSTRINGMAX);
  }

  BbbString& operator=(const BbbString& s)
  {
    assert (s.length() < BBBSTRINGMAX);
#if defined(_MSC_VER) 
#if (_MSC_VER >= 1400)
    strcpy_s(buf,sizeof(buf),s.buf);
#else
    strcpy(buf,s.buf);
#endif
#else
    strcpy(buf,s.buf);
#endif
    return *this;
  }


  bool operator == (const char* s) const {
      bool rta = false;
      do {
	  // los 2 vacios
	  if (s == NULL && length() == 0) {
	      rta = true;
	      break;
	  }
	  // el que viene esta vacio
	  if (s == NULL && length() != 0) {
	      rta = false;
	      break;
	  }
	  // el buffer de la clase esta vacio
	  if (strlen(s) == 0 && length() == 0) {
	      rta = true;
	      break;
	  }
	  // el buffer de la clase esta vacio
	  if (s != NULL && length() == 0) {
	      rta = false;
	      break;
	  }
	  // se quiere compara algo de != largo
	  if (strlen(s) != length()) {
	      rta = false;
	      break;
	  }
	  // se quiere compara algo muy largo
	  if (strlen(s) > BBBSTRINGMAX) {
	      rta = false;
	      break;
	  }
	  rta = (!strcmp(buf, s) ? true : false);
      } while (false);

      return rta;
  }


  BbbString& operator+=(const BbbString& s)
  {
      assert ((length()+s.length()) < BBBSTRINGMAX);
#if defined(_MSC_VER) 
#if (_MSC_VER >= 1400)
    strcat_s(buf,sizeof(buf),s.buf);
#else
    strcat(buf,s.buf);
#endif
#else
    strcat(buf,s.buf);
#endif
    return *this;
  }

  const char* c_str(void) const
  {
    return buf;
  }

  void assign(char* temp_buf,int len)
  {
    assert (len < BBBSTRINGMAX);
    for(int i=0;i<len;++i)
      buf[i]=temp_buf[i];
    buf[len]=0;
  }

  char& operator[](int ndx)
  {
    return buf[ndx];
  }

  size_t length(void) const 
  {
    return strlen(buf);
  }

friend std::ostream& operator<<(std::ostream& s,const BbbString& str)
{
  s<<str.c_str();
  return s;
}

protected:
   char buf[BBBSTRINGMAX+1];

};


#endif // __BBBSTRING_H__

