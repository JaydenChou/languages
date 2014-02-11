//**************************************************************************
// File: buildBooks.cpp
//
// Description: Functions for create a html file
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
#include "buildBooks.h"

#ifdef _WIN32
   // In windows: the name of the struct is: _stat
#  define structStat _stat
   // In windows: the name of the function is: _stat()
#  define fncStat _stat
   // In windows: the macro S_ISDIR() not exist.
#  define S_ISDIR(mode) ((statVal.st_mode & S_IFMT) & S_IFDIR)
#elif __linux__
   // In linux: the name of the struct is: stat
#  define structStat stat
   // In linux: the name of the function is: lstat
#  define fncStat lstat
#endif

//**************************************************************************
// Function:   replaceName
// Scope:      global
// Purpose:    replace ' ' with "%20"
// Return:     -
//**************************************************************************
void replaceName(std::string &sFile, std::string &sNewFileName)
{
   for (int k = 0; k < sFile.length(); k++) {
      if (sFile[k] == ' ') {
         sNewFileName += "%20";
      } else {
         sNewFileName += sFile[k];
      }
   }
}


//***************************************************************************
// Function:   byBook
// Scope:      global
// Purpose:    comparisson functions
// Return:     true or false
//***************************************************************************
bool byBook(stBooks b1, stBooks b2)
{
   return (b1.sBook < b2.sBook);
}


//***************************************************************************
// Function:   initLanguage
// Purpose:    comparisson functions
// Return:     static number with the language
//***************************************************************************
int initLanguage(std::string& sLanguage) 
{
   if (sLanguage == "sp")
      return LANGUAGE_SP;
   if (sLanguage == "en")
      return LANGUAGE_EN;
   if (sLanguage == "de")
      return LANGUAGE_DE;
   if (sLanguage == "pt")
      return LANGUAGE_PT;
   return LANGUAGE_SP;
}


//***************************************************************************
// Function:   isDir
// Scope:      global
// Purpose:    receives a name
// Return:     true if the name is an existing directory in the filesystem
//***************************************************************************
bool isDir(std::string& sDir)
{
   struct structStat statVal;
   if (fncStat(sDir.c_str(), &statVal) < 0) {
      std::cerr << __FILE__ << ":" << __LINE__ << " "
           << "Error in stat() [" << sDir << "] "
           << "not exist - "
           << "errno (" << errno << ")" << std::endl;
      return false;
   }

   // Not a Directory
   if (!(S_ISDIR(statVal.st_mode))) {
      std::cerr << __FILE__ << ":" << __LINE__ << " "
           << "Error [" << sDir << "] " 
           << "is not a directory - "
           << "errno (" << errno << ")" << std::endl;
      return false;
   }

   return true;
}


//***************************************************************************
// Function:   processEntryDirectoryName
// Scope:      global
// Purpose:    receives an entry of a path (a file), save the FileType and
//             return the size of the file
// Return:     the size of the file
//***************************************************************************
off_t processEntryDirectoryName(std::string sDir, char *pEntryName, std::string& sFileType)
{
   if (!strcmp(pEntryName, ".") ||
       !strcmp(pEntryName, ".."))
       return -1;

   std::cerr << std::endl
             << "processing [" << pEntryName << "]" << std::endl;
   std::string sFullPath(sDir);
   sFullPath += "/";
   sFullPath += pEntryName;
   off_t st_size = 0;     // total size, in bytes
   {
      struct structStat statVal;
      if (fncStat(sFullPath.c_str(), &statVal) < 0) {
         std::cerr << __FILE__ << ":" << __LINE__ << " "
              << "Error in lstat() there is a problem with [" << sFullPath << "] "
              << "errno (" << errno << ")" << std::endl;
         return -1;
      }

      // Is a Directory
      if (S_ISDIR(statVal.st_mode)) {
         std::cerr << "Dir -> " << sFullPath << std::endl;
         return -1;
      }

      // Get size of file in bytes
#ifdef __linux__
      if (S_ISLNK(statVal.st_mode)) {
         std::cerr << "ln: ";
         char *buf = new char[256];
         int bufsiz = 256;
         int iResult = readlink(sFullPath.c_str(), buf, bufsiz); 
         std::cerr << "[" << buf << "] sz=" << bufsiz << " - ";

         std::string sFullPathLink = sDir;
         sFullPathLink += '/';
         sFullPathLink += buf;

         if (lstat(sFullPathLink.c_str(), &statVal) < 0) {
            std::cerr << __FILE__ << ":" << __LINE__ << " "
                 << "Error in lstat() there is a problem with [" << sFullPathLink << "] "
                 << "errno (" << errno << ")" << std::endl;
            return -1;
         }
      }
#endif
      st_size = statVal.st_size;

   }

   // Certains file names are not permited
   {
      char *pattern = "index.html";
      const int N = strlen(pEntryName);
      const int M = strlen(pattern);
      char *pszX = std::search(pEntryName,
                               pEntryName + N,
                               pattern, pattern + M);

      if (pszX != (pEntryName + N)) {
         std::cerr << "This is not a book" << std::endl;
         return -1;
      }
   }

   // File type
   {
      const int N = strlen(pEntryName);
      const int M = 1;
      char *token = ".";
      char *pszExt = std::find_end(pEntryName,
                                   pEntryName + N,
                                   token, token + M);
      sFileType = (++pszExt);
   }
   std::cerr << "FileType [" << sFileType << "]" << std::endl;

   // Only certains file types are permited
   if (!(sFileType == "pdf" ||
         sFileType == "zip" ||
         sFileType == "txt" ||
         sFileType == "lit" ||
         sFileType == "rtf" ||
         sFileType == "pdb" ||
         sFileType == "doc" ||
         sFileType == "rar" ||
         sFileType == "ppt" ||
         sFileType == "html" ||
         sFileType == "chm")) {
      std::cerr << "This is not a book" << std::endl;
      return -1;
   }
   return st_size;
}



//***************************************************************************
// Function:   getBookShortName
// Scope:      global
// Purpose:    save in sBook the short name of the file
// Return:     -
//***************************************************************************
void getBookShortName(char *pEntryName, int iLenghtFileType, std::string& sBook)
{
   const int L = strlen(pEntryName);
   for (int c = 0; c < (L - iLenghtFileType); c++) {

      // "[ - ]"
      if (pEntryName[c] == '-')
      if (c &&
          pEntryName[c-1] == ' ' &&
          pEntryName[c+1] == ' ')
         break;

      sBook += pEntryName[c];
   }
}


//***************************************************************************
// Function:   getBookLongName
// Scope:      global
// Purpose:    save in sBook the short name of the file
// Return:     -
//***************************************************************************
void getBookLongName(char *pEntryName, int iLenghtFileType, std::string& sBook)
{
   const int L = strlen(pEntryName);
   for (int c = 0; c < (L - (iLenghtFileType+1)); c++) {
      sBook += pEntryName[c];
   }
}


//***************************************************************************
// Function:   printHead       
// Scope:      global
// Purpose:    print <head>
// Return:     -
//***************************************************************************
void printHead(int iLanguage) 
{
   std::cout << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">" << std::endl
             << "<HTML>" << std::endl
             << "<HEAD>" << std::endl
	     << "<BASE HREF=\"http://localhost\">" << std::endl
             << "<META NAME=\"keywords\" content=\"books\">" << std::endl
	     << "<META HTTP-EQUIV=\"Content-Language\" content=\"us\">" << std::endl
             << "<META NAME=AUTHOR CONTENT=\"Javier Fuchs\">" << std::endl
             << "<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">" << std::endl
             << "<META NAME=\"CHARSET\" content=\"ISO-8859-1\">" << std::endl
             << std::endl
             << "<meta name=\"DESCRIPTION\"" << std::endl
      	     << "      content=\"Javier Fuchs: Books\">" << std::endl
             << "<TITLE>Javier Fuchs: ";
   switch (iLanguage) {
   case LANGUAGE_SP:
      std::cout << "Libros"; break;
   case LANGUAGE_EN:
      std::cout << "Books"; break;
   default:
      std::cout << "Libros";
   }
   std::cout << "</TITLE>" << std::endl
             << "</HEAD>" << std::endl
             << "<BODY>" << std::endl;
}



//***************************************************************************
// Function:   printFoot
// Scope:      global
// Purpose:    print <head>
// Return:     -
//***************************************************************************
void printFoot(void) 
{
   std::cout << "<p>" << std::endl
             << "<table width=100"
#ifdef _WIN32
             << '%'
#elif __linux__
             << '\%'
#endif
             << " cellpadding=0 cellspacing=0><tr><td width=100"
#ifdef _WIN32
             << '%'
#elif __linux__
             << '\%'
#endif
             << "valign=top id=HEADLINE align=right>" 
             << "<font size=-1>" 
             << "Copyright &copy; 2005 Javier Fuchs</font>" << std::endl
             << "</td>" << std::endl
             << "</tr>" << std::endl
             << "</TABLE>" << std::endl
             << "</BODY>" << std::endl
             << "</HTML>" << std::endl;
}

//***************************************************************************
// Function:   printTitleHeader
// Scope:      local static
// Purpose:    print in the standard output the header of the html
// Return:     -
//***************************************************************************
static void printTitleHeader(std::string& sDir, int iLanguage) 
{
   std::cout << "<center>"
             << "<H1>";

   switch (iLanguage) {
   case LANGUAGE_SP:
      std::cout << "Libros de "; break;
   case LANGUAGE_EN:
      std::cout << "Books from "; break;
   default:
      std::cout << "Books from ";
   }
   std::cout << sDir
             << "</H1>"
             << "</center>"
             << std::endl
             << "<p>"
             << "<p>"
             << "<p>"
             << "<p>";
}


//***************************************************************************
// Function:   printTitleHeader
// Scope:      global
// Purpose:    print in the standard output the column title and the header 
// Return:     -
//***************************************************************************
void printTitle(std::string& sDir, int iLanguage)
{
   printTitleHeader(sDir, iLanguage);
   std::cout << "<table border=1 cellspacing=0 cellpadding=0 >" << std::endl
             << "<tr>" << std::endl;

   std::cout << "  <th width=5%>";
   switch (iLanguage) {
   case LANGUAGE_SP:
      std::cout << " Tipo "; break;
   case LANGUAGE_EN:
      std::cout << " Type "; break;
   default:
      std::cout << " Type ";
   }
   std::cout << "</th>" << std::endl;

   std::cout << "  <th width=5%>";
   switch (iLanguage) {
   case LANGUAGE_SP:
      std::cout << " Tama&ntilde;o (KB) "; break;
   case LANGUAGE_EN:
      std::cout << " Size (KB) "; break;
   default:
      std::cout << " Size (KB) ";
   }
   std::cout << "</th>" << std::endl;

   std::cout << "  <th width=65%>";
   switch (iLanguage) {
   case LANGUAGE_SP:
      std::cout << " Libro "; break;
   case LANGUAGE_EN:
      std::cout << " Book "; break;
   default:
      std::cout << " Book ";
   }
   std::cout << "</th>" << std::endl;

}


//***************************************************************************
// Function:   printTitleAuthor
// Scope:      global
// Purpose:    print in the standard output the column for the Author
// Return:     -
//***************************************************************************
void printTitleAuthor(int iLanguage)
{
   std::cout << "  <th width=30%>";
   switch (iLanguage) {
   case LANGUAGE_SP:
      std::cout << " Autores "; break;
   case LANGUAGE_EN:
      std::cout << " Authors "; break;
   default:
      std::cout << " Authors ";
   }
   std::cout << "</th>" << std::endl;
}
