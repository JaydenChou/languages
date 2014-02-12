/******************************************************************************
 *   File: BbbFile.cpp
 *   
 *   Comments: Implementacion de una clase con metodos static que tienen que ver
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
#include <iostream>
#include <strstream>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#ifndef _ALLOW_STD_TYPES_
#define _ALLOW_STD_TYPES_
#endif
#include "BbbLog.h"
#include "BbbFile.h"


/**
 * Si el archivo existe si/no retorna true/false
 * @param filename
 * 
 * @return bool
 */
bool BbbFile::exist(const char *filename)
{
    dbg << endl;
    bool rta = false;

    do {
		struct stat statVal;

		if (lstat(filename, &statVal) < 0) {
			wrn << "El archivo " << filename << " no existe en el directorio." << endl;
			break;
		}

		if(!(S_ISREG(statVal.st_mode))) {
			wrn << filename << " no es un archivo. " << endl;
			break;
		}

		rta = true;

    } while (false);

    return rta;
}

/**
 * Si el directorio existe si/no retorna true/false
 * @param pathname
 * 
 * @return bool
 */
bool BbbFile::pathexist(const char *pathname)
{
    dbg << endl;
    bool rta = false;

    do {
		struct stat statVal;

		if (lstat(pathname, &statVal) < 0) {
			wrn << "El directorio " << pathname << " no existe." << endl;
			break;
		}

		if(!(S_ISDIR(statVal.st_mode))) {
			wrn << pathname << " no es un directorio. " << endl;
			break;
		}

		rta = true;

    } while (false);

    return rta;
}

/**
 * Obtiene la fecha de creacion/modificacion del archivo.
 * Si pudo obtener la informacion retorna true/false
 * @param filename (input)
 * @param modifTime (output)
 * 
 * @return bool
 */
bool BbbFile::time(const char *filename, time_t& mtime)
{
    dbg << endl;
    bool rta = false;

    do {
		struct stat statVal;

		if (lstat(filename, &statVal) < 0) {
			wrn << "El archivo " << filename << " no existe en el directorio." << endl;
			break;
		}

		mtime = statVal.st_mtim.tv_sec;

		rta = true;

    } while (false);

    return rta;
}
