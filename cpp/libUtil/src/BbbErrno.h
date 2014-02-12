/******************************************************************************
 *   File: BbbErrno.h
 *   
 *   Comments: Declaracion del manejo de errores en tiempo de ejecucion
 *             del servidor BBB
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
#ifndef __BBBERRNO_H__
#define __BBBERRNO_H__

#include <errno.h> 

class BbbErrno {
protected:

private:

public:
    static int getDebug() { return _debug; }
    static void msgctl(char *aFile, int aLine, int aErrno, char* aInfo);
    static void msgget(char *aFile, int aLine, int aErrno, char* aInfo);
    static void msgrcv(char *aFile, int aLine, int aErrno, char* aInfo);
    static void msgsnd(char *aFile, int aLine, int aErrno, char* aInfo);
    static void rename(char *aFile, int aLine, int aErrno, char* aInfo);
    static void remove(char *aFile, int aLine, int aErrno, char* aInfo);
    static void mkdir(char *aFile, int aLine, int aErrno, char* aInfo);
    static void opendir(char *aFile, int aLine, int aErrno, char* aInfo);
    static void fork(char *aFile, int aLine, int aErrno, char* aInfo);
    static int _debug;
};


#endif // __BBBERRNO_H__
