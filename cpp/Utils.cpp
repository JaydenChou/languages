//**************************************************************************
// File: Utils.cpp
//
// Description: Simple Functions
//
// Compiler: 
//    __linux__: gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 - kernel 2.4
//		 gcc/g++ 4.0.1 20050727 - Fedora Core 4 - kernel 2.6.12
//    _WIN32:    Visual Studio 6.0 (MS Windows NT 2000 XP)
//
// History:
//    Feb/2005: Javier Fuchs                                    |\_/|   
//              Phone (+54 2944) 525173                        /     \  
//              http://southfox.servehttp.com                 /_.~ ~,_\ 
//                                                               \@/    
//
// **************************************************************************

#ifdef _WIN32
#  pragma message( "Compiling " __FILE__ ) 
#  include "stdafx.h"
#  include <stdio.h>
#  include <rpc.h>
#  include "WbemDump.h"
#elif __linux__
#  include <sys/time.h>
#  include <time.h>
#endif

#include <strstream>

#include <assert.h>

#ifdef _WIN32
//*****************************************************************************
// Function:   DllMain
// Purpose:    Compatibility with Win32 shared libraries
//*****************************************************************************
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif

#ifdef _WIN32
DWORD DumpWMI(WCHAR* p_pwcsQuery, LPTSTR hInfo, int iLenInfo);
#endif

//*****************************************************************************
// Function:   fncGetBiosRelease
// Purpose:    Obtain Bios Release (only in Windows)
//*****************************************************************************

OFFICETOOL_API bool fncGetBiosRelease(LPTSTR hInfo, int iLenInfo) {
   if (iLenInfo <= 0)
      return false;

#ifdef _WIN32
   DumpWMI(L"SELECT ReleaseDate FROM Win32_BIOS", hInfo, iLenInfo);
#elif __linux__
   sprintf(hInfo, "fncGetBiosRelease(): Not supported in linux");
#endif
#ifdef _DEBUG_INFO
   std::strstream strDebugInfo;
   strDebugInfo << "Before. [" << hInfo << "]" << std::endl << '\0';
   MessageBox(NULL, strDebugInfo.str(), "mypackage.dll@fncGetBiosRelease()", MB_ICONSTOP);
#endif
   return true;
}


//*****************************************************************************
// Function:   fncGetBiosSerialNumber
// Purpose:    Obtain Bios Serial Number (only windows)
//*****************************************************************************

OFFICETOOL_API bool fncGetBiosSerialNumber(LPTSTR hInfo, int iLenInfo) {
   if (iLenInfo <= 0)
      return false;
#ifdef _WIN32
    DumpWMI(L"SELECT SerialNumber FROM Win32_BIOS", hInfo, iLenInfo);
#elif __linux__
    sprintf(hInfo, "fncGetBiosSerialNumber(): Not supported in linux");
#endif
#ifdef _DEBUG_INFO
    std::strstream strDebugInfo;
    strDebugInfo << "Before. [" << hInfo << "]" << std::endl << '\0';

    MessageBox(NULL, strDebugInfo.str(), "mypackage.dll@fncGetBiosSerialNumber()", MB_ICONSTOP);
#endif
    return true;
}


//*****************************************************************************
// Function:   fncGetMotherSerialNumber
// Purpose:    Obtain Motherboard Serial Number (only windows)
//*****************************************************************************

OFFICETOOL_API bool fncGetMotherSerialNumber(LPTSTR hInfo, int iLenInfo) {
   if (iLenInfo <= 0)
      return false;
#ifdef _WIN32
    DumpWMI(L"SELECT SerialNumber FROM Win32_Baseboard", hInfo, iLenInfo);
#elif __linux__
    sprintf(hInfo, "fncGetMotherSerialNumber(): Not supported in linux");
#endif
#ifdef _DEBUG_INFO
    std::strstream strDebugInfo;
    strDebugInfo << "Before. [" << hInfo << "]" << std::endl << '\0';

    MessageBox(NULL, strDebugInfo.str(), "mypackage.dll@fncGetBiosSerialNumber()", MB_ICONSTOP);
#endif
    return true;
}


//*****************************************************************************
// Function:   fncGetProcessorID
// Purpose:    Obtain CPU's ID (only Windows)
//*****************************************************************************

OFFICETOOL_API bool fncGetProcessorID(LPTSTR hInfo, int iLenInfo) {
   if (iLenInfo <= 0)
      return false;

#ifdef _WIN32
    DumpWMI(L"SELECT ProcessorID FROM Win32_Processor", hInfo, iLenInfo);
#elif __linux__
    sprintf(hInfo, "fncGetProcessorID(): Not supported in linux");
#endif
#ifdef _DEBUG_INFO
    std::strstream strDebugInfo;
    strDebugInfo << "Before. [" << hInfo << "]" << std::endl << '\0';

    MessageBox(NULL, strDebugInfo.str(), "mypackage.dll@fncGetBiosInfo()", MB_ICONSTOP);
#endif
    return true;
}


//*****************************************************************************
// Function:   fncGetSystemInfo
// Purpose:    Obtain System Information (Only Windows)
//             The SYSTEM_INFO structure contains information about the 
//             current computer system. This includes the architecture 
//             and type of the processor, the number of processors in the 
//             system, the page size, and other such information. 
//*****************************************************************************

OFFICETOOL_API bool fncGetSystemInfo(LPTSTR hSystemInfo, int iLenInfo) {
   if (iLenInfo <= 0)
      return false;
    
#ifdef _DEBUG_INFO
   std::strstream strDebugInfo;
#endif

#ifdef _WIN32
   SYSTEM_INFO si; 

   // The GetSystemInfo function returns information about the current system. 
   GetSystemInfo(&si);


   int i = 0;
   if (i < iLenInfo) {
      i += sprintf(hSystemInfo + i, "%d", si.wProcessorArchitecture);
#     ifdef _DEBUG_INFO
      strDebugInfo << "wProcessorArchitecture = [" << si.wProcessorArchitecture << "]" << std::endl;
#     endif
   }
   if (i < iLenInfo) {
      i += sprintf(hSystemInfo + i, "%d", si.wProcessorLevel);
#     ifdef _DEBUG_INFO
      strDebugInfo << "wProcessorLevel = [" << si.wProcessorLevel << "]" << std::endl;
#     endif
   }
   if (i < iLenInfo) {
      i += sprintf(hSystemInfo + i, "%d", si.dwNumberOfProcessors);
#     ifdef _DEBUG_INFO
      strDebugInfo << "dwNumberOfProcessors = [" << si.dwNumberOfProcessors << "]" << std::endl;
#     endif
   }
   if (i < iLenInfo) {
      i += sprintf(hSystemInfo + i, "%d", si.dwProcessorType);
#     ifdef _DEBUG_INFO
      strDebugInfo << "dwProcessorType = [" << si.dwProcessorType << "]" << std::endl;
#     endif
   }
   if (i < iLenInfo) {
      i += sprintf(hSystemInfo + i, "%d", si.wProcessorRevision);
#     ifdef _DEBUG_INFO
      strDebugInfo << "wProcessorRevision = [" << si.wProcessorRevision << "]" << std::endl;
#     endif
   }

   // The GetVolumeInformation function returns information about a 
   // file system and volume whose root directory is specified. 
   DWORD nVolumeNameSize = 128;
   LPTSTR lpVolumeNameBuffer = new CHAR[nVolumeNameSize];
   LPDWORD lpMaximumComponentLength = new DWORD;
   LPDWORD lpFileSystemFlags = new DWORD;
   DWORD nFileSystemNameSize = 128;
   CHAR lpFileSystemNameBuffer[128];
   BOOL bRet = GetVolumeInformation(
         NULL,        // address of root directory of the file system
         lpVolumeNameBuffer,     // address of name of the volume
         nVolumeNameSize,         // length of lpVolumeNameBuffer
         NULL,  // address of volume serial number
         lpMaximumComponentLength,     // address of system's maximum filename length
         lpFileSystemFlags,     // address of file system flags
         lpFileSystemNameBuffer, // address of name of file system
         nFileSystemNameSize      // length of lpFileSystemNameBuffer
       );
   if (i < iLenInfo) {
      i += sprintf(hSystemInfo + i, "%s", lpVolumeNameBuffer);
#     ifdef _DEBUG_INFO
      strDebugInfo << "lpFileSystemNameBuffer = [" << lpFileSystemNameBuffer << "]" << std::endl << '\0';
      strDebugInfo << "lpVolumeNameBuffer = [" << lpVolumeNameBuffer << "]" << std::endl;
#     endif
   }
   if (i < iLenInfo) {
      i += sprintf(hSystemInfo + i, "%s", lpFileSystemNameBuffer);
   }
   delete lpVolumeNameBuffer;
   delete lpMaximumComponentLength;
   delete lpFileSystemFlags;
#elif __linux__
#  ifdef _DEBUG_INFO
   strDebugInfo << "fncGetSystemInfo(): Not supported in linux" << '\0';
#  endif
   sprintf(hSystemInfo, "fncGetSystemInfo(): Not supported in linux");
#endif
#ifdef _DEBUG_INFO
   MessageBox(NULL, strDebugInfo.str(), "mypackage.dll@fncGetSystemInfo()", MB_ICONSTOP);
#endif

   return true;
}


//*****************************************************************************
// Function:   fncGetMacAddress
// Purpose:    Obtain Mac-Address (only Windows)
//*****************************************************************************

OFFICETOOL_API bool fncGetMacAddress(LPTSTR hMacAddress, int iLenInfo) {
   bool bRet = true;
   if (iLenInfo <= 0)
      return false;

    std::strstream strMacAddress;

#ifdef _WIN32
    // Windows NT: Yes
    // Windows CE: Unsupported.
    // Header: Declared in rpcdce.h.
    // Import Library: Link with rpcrt4.lib.
    UUID uuid;
    UuidCreate(&uuid);

    for (int i = 2; i < 8; ++i) {
        strMacAddress << std::hex;
        strMacAddress.fill('0');
        strMacAddress.width(2);
        strMacAddress << int (uuid.Data4[i]);
        if (i < 7) {
            strMacAddress << ":";
        }
    }
    strMacAddress << '\0';
#  ifdef _DEBUG_INFO
    MessageBox(NULL, strMacAddress.str(), "mypackage.dll@fncGetMacAddress()", MB_ICONSTOP);
#  endif
#elif __linux__
    strMacAddress << "fncGetMacAddress(): Not supported in linux" << '\0';
#endif
    if (strlen(strMacAddress.str()) < iLenInfo)
       sprintf(hMacAddress, "%s", strMacAddress.str());
    else {
       sprintf(hMacAddress, "%s", "Null");
       bRet = false;
    }

    return bRet;
}



//*****************************************************************************
// Function:   fncGetSystemTime
// Purpose:    Obtain Local Date and Time from the system
// OS:         Windows and Linux
//*****************************************************************************

OFFICETOOL_API bool fncGetSystemTime(LPTSTR hSystemTime, int iLenInfo) {
   bool bRet = true;
   if (iLenInfo <= 0)
      return false;

    char szAux[20];
#ifdef _WIN32
    SYSTEMTIME lSystemTyme;
    // The GetSystemTime function retrieves the current system date and time.
    // The system time is expressed in Coordinated Universal Time (UTC).
    //   Windows NT: Requires version 3.1 or later.
    //   Windows: Requires Windows 95 or later.
    //   Windows CE: Requires version 1.0 or later.
    
    GetLocalTime(&lSystemTyme);
    sprintf(szAux, "%02d%02d%04d%02d%02d",
        lSystemTyme.wDay,
        lSystemTyme.wMonth,
        lSystemTyme.wYear,
        lSystemTyme.wHour,
        lSystemTyme.wMinute);
#elif __linux__
    struct timeval tvDate;
    gettimeofday(&tvDate, NULL);
    strftime(szAux, 15, "%d%m%Y%H%M", localtime(&(tvDate.tv_sec)));
#endif
    if (strlen(szAux) < iLenInfo)
       sprintf(hSystemTime, "%s", szAux);
    else {
       memcpy(hSystemTime, szAux, iLenInfo);
       hSystemTime[iLenInfo] = 0;
       bRet = false;
    }
#ifdef _DEBUG_INFO
    MessageBox(NULL, hSystemTime, __FILE__, MB_ICONSTOP);
#endif

    return bRet;
}


