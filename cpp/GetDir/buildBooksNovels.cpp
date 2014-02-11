//**************************************************************************
// File: buildBooksNovels.cpp
//
// Description: Build an html with books
//
// Compiler: 
//    __linux__: gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 - kernel 2.4
//		 gcc/g++ 4.0.1 20050727 - Fedora Core 4 - kernel 2.6.12
//    _WIN32:    Visual Studio 6.0 (MS Windows NT 2000 XP)
//
// History:
//  Feb/2005: Javier Fuchs                                    |\_/|   
//            Phone (+54 2944) 525173                        /     \  
//            http://southfox.servehttp.com                 /_.~ ~,_\ 
//                                                             \@/    
//
//**************************************************************************
#include "buildBooks.h"


// Vector of books
std::vector<stBooks> vBooks;


//**************************************************************************
// Function:   printhtml
// Scope:      global
// Purpose:    prints in standard output an html
// Return:     -
//**************************************************************************
void printhtml(std::string& sDir, int iLanguage) {
   printHead(iLanguage);
   printTitle(sDir, iLanguage);

   std::cout << "</tr>" << std::endl;

   for (int n = 0; n < vBooks.size(); n++) {
      // printing 
      std::string sNewFileName="";
      replaceName(vBooks[n].sFile, sNewFileName);
      std::cout << "<tr>" << std::endl
                << "<td>" 
                << "<A HREF=\"" << "books" << '/' << sDir << '/' << sNewFileName << "\">"
                << vBooks[n].sFileType
                << "</A>" 
                << "</td>" 
                << "<td align=" << '"' << "right" << '"' << '>';
      std::cout << vBooks[n].st_size/1024;
      std::cout << "</td>"
                << "<td>" << vBooks[n].sBook << "</td>" 
                << std::endl
                << "</tr>" << std::endl;
   }

   std::cout << "</table>" << std::endl;
   printFoot();
}

//**************************************************************************
// Function:   main
// Scope:      global
// Purpose:    prints in standard output an html
// Return:     -
//**************************************************************************
#ifdef _WIN32
void
#elif  __linux__
int 
#endif
main(int argc, char **argv)
{
   // Directory
   std::string sDir = argv[1];
#ifdef _WIN32
   HANDLE          hList;
   WIN32_FIND_DATA FileData;
   std::string sDirPattern;
#elif  __linux__
   // Directory pointer
   DIR *pDir;
   // pointer to the directory entry
   struct dirent *pEntry;
#endif

   // Language
   std::string sLanguage = argv[2];
   int iLanguage = initLanguage(sLanguage);

   // Verifying if the directory exist
   if (!isDir(sDir)) {
      exit(1);
   }

#ifdef __linux__
   // Open the directory
   if ((pDir = opendir(sDir.c_str())) == NULL) {
      std::cerr << __FILE__ << ":" << __LINE__ << " "
           << "Error in opendir() [" << sDir << "] " << std::endl
           << "errno (" << errno << ")" << std::endl;
      exit(1);
   }
#endif

#ifdef _WIN32
   sDirPattern = sDir;
   sDirPattern += "\\*";
   // Get the first file
   if ((hList = FindFirstFile(sDirPattern.c_str(), &FileData)) == INVALID_HANDLE_VALUE)
#elif  __linux__
   if ((pEntry = readdir(pDir)) == NULL)
#endif
   {
     std::cerr << __FILE__ << ":" << __LINE__ << " "
          << "No files found" << std::endl
          << "errno (" << errno << ")" << std::endl;
     exit(1);
   }
   // Directory: read entries
   do 
   {
#ifdef _WIN32
      char *pFileName = FileData.cFileName;
#elif  __linux__
      char *pFileName = pEntry->d_name;
#endif

      std::string sFileType;
      off_t st_size = 0;     // total size, in bytes
      // process the file entry
      if ((st_size = processEntryDirectoryName(sDir, pFileName, sFileType)) == -1) {
         continue;
      }

      // Obtaining the Long name of the book
      std::string sBook;
      getBookLongName(pFileName, sFileType.length(), sBook);
      std::cerr << "Book [" << sBook << "]" << std::endl;

      // create an instance of the book structure, and save it in the vector
      {
      stBooks b;
      std::string sFile(pFileName);
      b.sFile = sFile;
      b.sFileType = sFileType;
      b.sBook = sBook;
      b.st_size = st_size;

      vBooks.push_back(b);
      }
   }
#ifdef _WIN32
   while (FindNextFile(hList, &FileData));
#elif  __linux__
   while ((pEntry = readdir(pDir)) != NULL);
#endif

   if (vBooks.size()) {
      // Sort the vector
      std::stable_sort(vBooks.begin(), vBooks.end(), std::ptr_fun(byBook));
      // Print all the books
      printhtml(sDir, iLanguage);
   }
   exit(0);
}




