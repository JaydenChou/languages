#**************************************************************************
# File: Makefile.am
#
# Compiler: 
#    __linux__: gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 - kernel 2.4
#		 gcc/g++ 4.0.1 20050727 - Fedora Core 4 - kernel 2.6.12
#    _WIN32:    Visual Studio 6.0 (MS Windows NT 2000 XP)
#
# History:
#   Feb/2005: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    
#
#**************************************************************************

bin_PROGRAMS = buildBooksNovels buildBooksTechnical

buildBooksNovels_SOURCES = buildBooksNovels.cpp buildBooks.cpp buildBooks.h

buildBooksTechnical_SOURCES = buildBooksTechnical.cpp buildBooks.cpp buildBooks.h

AM_CXXFLAGS=-D_VERSION=$(VERSION) -Wno-deprecated -g

INCLUDES=-I/usr/include

