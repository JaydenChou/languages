/******************************************************************************
 *   File: BbbLog.cpp
 *   
 *   Comments: Implementacion de clase para log o trace del servbbb
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

#include <strstream>

#ifdef _BBBLOG_TEST_
#define S_LOG
#endif
#ifndef _ALLOW_STD_TYPES_
#define _ALLOW_STD_TYPES_
#endif
#include "BbbLog.h"

/**
 * Constructor por default
 */
BbbLog::BbbLog()
{
   vater = 0;
   setpid(::getpid());
   stdOutput = false;
   logLevel = thisLine = oldLine = LOGQUIET;
   open(getName());
}

char* BbbLog::getName() {
   std::strstream Nombre;
   if (getenv("BBBLOGDIR") == NULL) {
       std::cerr << "variable de ambiente BBBLOGDIR no existe" << nl;
       Nombre << "/tmp";
   } else {
       Nombre << getenv("BBBLOGDIR");
   }
   Nombre << "/";
   if (getenv("BBBLOGFILE") == NULL) {
       std::cerr << "variable de ambiente BBBLOGFILE no existe" << nl;
       BbbDate d;
       Nombre << "BBB_" 
	      << getenv("USERNAME") << '_'	// obtiene usuario linux que hizo login al equipo
	      << d << ".log";
   } else {
       Nombre << getenv("BBBLOGFILE");
   }
   Nombre << '\0';
   return Nombre.str();
}

/**
 * Constructor
 * @param name
 */
BbbLog::BbbLog(const char* name)
{
   vater = 0;
   setpid(::getpid());
   logLevel = thisLine = oldLine = LOGQUIET;
   stdOutput = false;
   open(name);
}

/**
 * Desctructor
 */
BbbLog::~BbbLog()
{
	BbbDate dtLog((char *)"%c");
	if (vater == 0) {
		fs << nl << nl << nl
		   << "--- " << getpid() 
		   << ' ' << __FILE__ << ' ' << __FUNCTION__ << "()" << ':' << __LINE__ << ' ' << nl
		   << setfill('=') << setiosflags(ios::left) << setw(80) << '=' << nl
		   << "PID: " << getpid() << " - FIN Sesion: " << dtLog << nl
		   << ' ' << __FILE__ << ' ' << __FUNCTION__ << "()" << ':' << __LINE__ << ' ' << nl
		   << setfill('=') << setiosflags(ios::left) << setw(80) << '=' << nl;
	} else {
		fs << nl << nl << nl
		   << "--- " << getpid() << "FIN Sesion del hijo " << getpid() << "." << nl;
	}
	// WARNING!!
	// El hijo cierra el log cuando se muere. Seria mejor que no lo hiciera. 
	close();
}

/**
 * Abrir archivo
 */
void BbbLog::open() {
	fs << nl << nl << nl
	   << "--- " << getpid() << "OPEN del log" << nl;
	open(getName());
}

void BbbLog::close() {
	fs << nl << nl << nl
	   << "--- " << getpid() << "CLOSE del log" << nl;
	fs.close();
}

/**
 * Abrir archivo
 * @param name
 */
void BbbLog::open(const char* name)
{
   // reopening a stream fails  
   fs.clear(); // este clear es para este bug en la libstdc++
   // Ver: http://gcc.gnu.org/onlinedocs/libstdc++/faq/index.html#4_0

   if (name == NULL) {
       cerr << "no esta seteado el nombre del archivo" << nl;
       setStdOutput();
       return;
   }
   fs.open(name, ios::out | ios::app);

   if (!fs) {
       cerr << "cannot open: " << name << nl;
   }
   BbbDate dtLog((char *)"%c");
   fs << "--- " << getpid() 
      << ' ' << __FILE__ << ' ' << __FUNCTION__ << "()" << ':' << __LINE__ << ' ' << nl
      << setfill('=') << setiosflags(ios::left) << setw(80) << '=' << nl
      << "PID: " << getpid() << " - INICIO Sesion: " << dtLog << nl
      << setfill('=') << setiosflags(ios::left) << setw(80) << '=' << nl;
   fs.flush();
}

#ifdef _BBBLOG_TEST_
/**
 * Test
 * 
 * @return int
 */

// g++  -Wno-deprecated -m32 -I./ -I../../shared/h -DLINUX -D_BBBLOG_TEST_ BbbLog.cpp -o BbbLogTest

int f() {
    dbg << endl; 
    dbg << "HI" << endl; 
    for (long i = 0; i < 2000000; i++);
    return 99;
}
int main()
{
    slog.setStdOutput();
    slog.setOn();
    BbbDate d;
    inf << "Test de la clase BbbLog()" << nl
        << "Fecha: " << d << endl;
    inf << "int ";
    slog << 10;   


    slog << "float ";
    slog << 10.0f;
    slog << endl;
    slog << getpid() << ": log creado." << endl;
    cout << getpid() << ": log creado." << endl;


    inf << " ---> INF0 <--- " << f() << endl;
    dbg << " ---> DBG <--- " << f() << endl;
    inf << " ---> INF1 <--- " << f() << endl;
    err << " ---> ERR <--- " << f() << endl;
    inf << " ---> INF2 <--- " << f() << endl;
    wrn << " ---> WRN <--- " << f() << endl;
    inf << " ---> INF3 <--- " << f() << endl;

	slog.close();
	inf << "Cerro" << endl;
	slog.open();
    slog.setOn();
	inf << "Abrio" << endl;
	inf << "FIN TEST" << endl;

    return 0;
}



#endif
