//**************************************************************************
// File: buildBooks.h
//
// Description: Header of functions for create a html file
//
// Compiler: 
//    __linux__: gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 - kernel 2.4
//		 gcc/g++ 4.0.1 20050727 - Fedora Core 4 - kernel 2.6.12
//    _WIN32:    Visual Studio 6.0 (MS Windows NT 2000 XP)
//
// History:
//   Feb/2005: Javier Fuchs                                    |\_/|   
//             Phone (+54 2944) 525173                        /     \  
//             http://southfox.servehttp.com                 /_.~ ~,_\ 
//                                                              \@/    
//
//**************************************************************************
#ifndef __BUILD_BOOKS_H__
#define __BUILD_BOOKS_H__


#ifdef _WIN32
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <algorithm>
#include <functional>
#elif __linux__
#undef __DEPRECATED
#include <iostream.h>
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#ifdef __linux__
#include <dirent.h>
#endif

#include <errno.h>
#include <string>
#include <vector>
#include <iomanip.h>


//**************************************************************************
// Name:       stBooks
// Object:     struct
// Scope:      public
// Purpose:    books structure
//**************************************************************************
typedef struct {
      std::string sFile;
      std::string sFileType;
      std::string sBook;
      std::string sAuthors;
      off_t st_size;
} stBooks;


//**************************************************************************
// Name:       enum_LANGUAGE
// Object:     enum
// Scope:      public
// Purpose:    language enumeration
//**************************************************************************
static enum {
   LANGUAGE_SP,
   LANGUAGE_EN,
   LANGUAGE_DE,
   LANGUAGE_PT
} enum_LANGUAGE;



//**************************************************************************
// Exported functions
//**************************************************************************
void replaceName(std::string &, std::string &);
bool byBook(stBooks, stBooks);
int initLanguage(std::string&);
bool isDir(std::string&);
off_t processEntryDirectoryName(std::string, char*, std::string&);
void getBookShortName(char*, int, std::string&);
void getBookLongName(char*, int, std::string&);
void printHead(int);
void printFoot(void);
void printTitle(std::string&, int);
void printTitleAuthor(int);


#endif // __BUILD_BOOKS_H__
