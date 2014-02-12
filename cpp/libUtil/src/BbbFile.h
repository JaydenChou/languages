/******************************************************************************
 *   File: BbbFile.h
 *   
 *   Comments: Declaracion de una clase con metodos static que tienen que ver
 *             con archivos en disco.
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
#ifndef __BBBFILE_H__
#define __BBBFILE_H__


class BbbFile {
protected:

private:

public:
    static bool exist(const char *filename);
    static bool pathexist(const char *pathname);
	static bool time(const char *filename, time_t& mtime);

};


#endif // __BBBFILE_H__
