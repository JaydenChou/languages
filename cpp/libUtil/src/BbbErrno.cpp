/******************************************************************************
 *   File: BbbErrno.cpp
 *   
 *   Comments: Implementacion del manejo de errores en tiempo de ejecucion
 *             del servidor SERVBBB
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


#ifndef _ALLOW_STD_TYPES_
#define _ALLOW_STD_TYPES_
#endif
#include "BbbLog.h"
#include "BbbErrno.h"

int BbbErrno::_debug = 1;

void BbbErrno::msgctl(char *aFile, int aLine, int aErrno, char* aInfo)
{
   std::strstream errnoLog;
   errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
   do {
      errnoLog << ", ";
      // On failure, aErrno is set to one of the following values:
      if (aErrno == EACCES) {
         errnoLog << "EACCES";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
               "The  argument  cmd is equal to IPC_STAT or MSG_STAT, but the "
               "calling process does not have read permission on the message "
               "queue msqid, and does not have the CAP_IPC_OWNER capability.";
         }
         break;
      }
      if (aErrno == EFAULT) {
         errnoLog << "EFAULT";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                 "The argument cmd has the value IPC_SET or IPC_STAT, but  the "
                 "address pointed to by buf isn?t accessible.";
         }
         break;
      }
      if (aErrno == EIDRM) {
         errnoLog << "EIDRM";
         if (BbbErrno::getDebug()) {
            errnoLog << ", The message queue was removed.";
         }
         break;
      }
      if (aErrno == EINVAL) {
         errnoLog << "EINVAL";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                     "Invalid  value  for cmd or msqid.  Or: for a MSG_STAT opera"
                     "tion, the index value specified  in  msqid  referred  to  an "
                     "array slot that is currently unused.";
         }
         break;
      }
      if (aErrno == EPERM) {
         errnoLog << "EPERM";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                     "The  argument cmd has the value IPC_SET or IPC_RMID, but the "
                     "effective user ID of the calling process is not the  creator "
                     "(as  found  in  msg_perm.cuid)  or  the  owner  (as found in "
                     "msg_perm.uid) of the message queue, and the process  is  not "
                     "privileged  (Linux: it does not have the CAP_SYS_ADMIN capa"
                     "bility).";
         }
         break;
      }

      // End of While
   } while(false);

   errnoLog << '\0';
   wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}



void BbbErrno::msgget(char *aFile, int aLine, int aErrno, char* aInfo)
{
   std::strstream errnoLog;
   errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
   // dummy switch
   do {
      errnoLog << ", ";
      // On failure, aErrno is set to one of the following values:
      if (aErrno == EACCES) {
         errnoLog << "EACCES";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                     "A message queue exists for key, but the calling process does "
                     "not  have  permission to access the queue, and does not have "
                     "the CAP_IPC_OWNER capability.";
         }
         break;
      }
      if (aErrno == EEXIST) {
         errnoLog << "EEXIST";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                    "A message queue exists for key  and  msgflg  specified  both "
                    "IPC_CREAT and IPC_EXCL.";
         }
         break;
      }
      if (aErrno == ENOENT) {
         errnoLog << "ENOENT";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                    "No  message  queue exists for key and msgflg did not specify "
                    "IPC_CREAT.";
         }
         break;
      }
      if (aErrno == ENOMEM) {
         errnoLog << "ENOMEM";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                    "A message queue has to be created but the  system  does  not "
                    "have enough memory for the new data structure.";
         }
         break;
      }
      if (aErrno == ENOSPC) {
         errnoLog << "ENOSPC";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                    "A  message  queue has to be created but the system limit for "
                    "the maximum number  of  message  queues  (MSGMNI)  would  be "
                    "exceeded.";
         }
         break;
      }

   } while(false);

   errnoLog << '\0';
   wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}


void BbbErrno::msgrcv(char *aFile, int aLine, int aErrno, char* aInfo)
{
   std::strstream errnoLog;
   errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
   // dummy switch
   do {
      errnoLog << ", ";
      // On failure, aErrno is set to one of the following values:
      if (aErrno == E2BIG) {
         errnoLog << "E2BIG";
         if (BbbErrno::getDebug()) {
            errnoLog << ", "
                    "The message text length is greater than msgsz and  MSG_NOER "
                    "ROR isn\'t specified in msgflg.";
		 }
		 break;
	  }
	  if (aErrno == EACCES) {
		  errnoLog << "EACCES";
		  if (BbbErrno::getDebug()) {
			  errnoLog << ", "
                  "The  calling  process  does  not have read permission on the "
                  "message queue, and does not have the CAP_IPC_OWNER  capability.";
		  }
		  break;
	  }
	  if (aErrno == EAGAIN) {
		  errnoLog << "EAGAIN";
		  if (BbbErrno::getDebug()) {
			  errnoLog << ", "
				      "No  message  was  available  in the queue and IPC_NOWAIT was "
					  "specified in msgflg.";
		  }
		  break;
	  }
	  if (aErrno == EFAULT) {
		  errnoLog << "EFAULT";
		  if (BbbErrno::getDebug()) {
			  errnoLog << ", "
				      "The address pointed to by msgp isn\'t accessible.";
		  }
		  break;
	  }
	  if (aErrno == EIDRM) {
		  errnoLog << "EIDRM";
		  if (BbbErrno::getDebug()) {
			  errnoLog << ", "
			          "While the process was sleeping to  receive  a  message,  the "
					  "message queue was removed.";
		  }
		  break;
	  }
	  if (aErrno == EINTR) {
		  errnoLog << "EINTR";
		  if (BbbErrno::getDebug()) {
			  errnoLog << ", "
			          "While  the  process  was  sleeping to receive a message, the "
					  "process caught a signal.";
		  }
		  break;
	  }
	  if (aErrno == EINVAL) {
		  errnoLog << "EINVAL";
		  if (BbbErrno::getDebug()) {
			  errnoLog << ", "
				      "msgqid was invalid, or msgsz was less than 0.";
		  }
		  break;
	  }
	  if (aErrno == ENOMSG) {
		  errnoLog << "ENOMSG";
		  if (BbbErrno::getDebug()) {
			  errnoLog << ", "
			          "IPC_NOWAIT was specified in msgflg and  no  message  of  the "
					  "requested type existed on the message queue.";
		  }
		  break;
	  }

   } while(false);
   errnoLog << '\0';
   wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}

void BbbErrno::msgsnd(char *aFile, int aLine, int aErrno, char* aInfo)
{
    std::strstream errnoLog;
    errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
    // dummy switch
    do {
       errnoLog << ", ";
       // On failure, aErrno is set to one of the following values:
       if (aErrno == EACCES) {
	  errnoLog << "EACCES";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "The calling process does not have write  permission  on  the "
                      "message  queue, and does not have the CAP_IPC_OWNER capability.";
	  }
	  break;
       }
       if (aErrno == EAGAIN) {
	  errnoLog << "EAGAIN";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "The message can\'t be sent due to the  msg_qbytes  limit  for "
                      "the queue and IPC_NOWAIT was specified in msgflg.";
	  }
	  break;
       }
       if (aErrno == EFAULT) {
	  errnoLog << "EFAULT";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "The address pointed to by msgp isn\'t accessible.";
	  }
	  break;
       }
       if (aErrno == EIDRM) {
	  errnoLog << "EIDRM";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "The message queue was removed.";
	  }
	  break;
       }
       if (aErrno == EINTR) {
	  errnoLog << "EINTR";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "Sleeping  on  a  full  message  queue condition, the process "
                      "caught a signal.";
	  }
	  break;
       }
       if (aErrno == EINVAL) {
	  errnoLog << "EINVAL";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                      "Invalid msqid value, or non-positive mtype value, or invalid "
		      "msgsz  value  (less  than 0 or greater than the system value "
		      "MSGMAX).";
	  }
	  break;
       }
       if (aErrno == ENOMEM) {
	  errnoLog << "ENOMEM";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                      "The system does not have enough memory to make a copy of the "
                      "message pointed to by msgp.";

	  }
	  break;
       }

    } while(false);
    errnoLog << '\0';
    wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}

void BbbErrno::rename(char *aFile, int aLine, int aErrno, char* aInfo)
{
    std::strstream errnoLog;
    errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
    // dummy switch
    do {
       errnoLog << ", ";
       // On failure, aErrno is set to one of the following values:
       if (aErrno == EACCES) {
	  errnoLog << "EACCES";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "Write  permission is denied for the directory containing oldpath or newpath, "
		      "or, search permission is denied for one of the directories in the path prefix "
		      "of oldpath or newpath,  or  oldpath  is  a directory  and  does  not  allow  "
		      "write  permission  (needed  to update the ..  entry).";
	  }
	  break;
       }
       if (aErrno == EBUSY) {
	  errnoLog << "EBUSY";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "The rename fails because oldpath or newpath is a directory that is in use by "
		      "some process (perhaps as  current  working directory, or as root directory, "
		      "or because it was open for reading) or is in use by the system (for example "
		      "as mount point), while the system considers this an  error.   (Note that there "
		      "is no requirement to return EBUSY in such cases ? there is nothing wrong with "
		      "doing the rename anyway ? but it is allowed to return EBUSY if the system "
		      "cannot otherwise handle such situations.)";
	  }
	  break;
       }
       if (aErrno == EFAULT) {
	  errnoLog << "EFAULT";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "oldpath or newpath points outside your accessible address space.";
	  }
	  break;
       }
       if (aErrno == EINVAL) {
	  errnoLog << "EINVAL";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "The  new  pathname  contained a path prefix of the old, or, more generally, "
		  "an attempt was made to make a directory a subdirectory of itself.";
	  }
	  break;
       }
       if (aErrno == EISDIR) {
	  errnoLog << "EISDIR";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "newpath is an existing directory, but oldpath is not a directory.";
	  }
	  break;
       }
       if (aErrno == ELOOP) {
	  errnoLog << "ELOOP";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "Too many symbolic links were encountered in resolving oldpath or newpath.";
	  }
	  break;
       }
       if (aErrno == EMLINK) {
	  errnoLog << "EMLINK";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "oldpath already has the maximum number of links to it, or it was a  directory  and "
		  "the  directory containing newpath has the maximum number of links.";
	  }
	  break;
       }
       if (aErrno == ENAMETOOLONG) {
	  errnoLog << "ENAMETOOLONG";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "oldpath or newpath was too long.";
	  }
	  break;
       }
       if (aErrno == ENOENT) {
	  errnoLog << "ENOENT";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "A directory component in oldpath  or  newpath does not exist or is a dangling "
		  "symbolic link.";
	  }
	  break;
       }
       if (aErrno == ENOMEM) {
	  errnoLog << "ENOMEM";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                      "Insufficient kernel memory was available.";
	  }
	  break;
       }
       if (aErrno == ENOSPC) {
	  errnoLog << "ENOSPC";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                      "The device containing the file has no room for the new directory entry.";
	  }
	  break;
       }
       if (aErrno == ENOTDIR) {
	  errnoLog << "ENOTDIR";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "A component used as a directory in oldpath or newpath is not, in fact, a "
		  "directory.  Or, oldpath is a directory, and newpath exists but is not a "
		  "directory.";
	  }
	  break;
       }
       if (aErrno == ENOTEMPTY) errnoLog << "ENOTEMPTY";
       if (aErrno == EEXIST) errnoLog << "EEXIST";
       if (aErrno == ENOTEMPTY || aErrno == EEXIST) {
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "newpath is a non-empty directory, i.e., contains entries other than \".\" "
		  "and \"..\". ";
	  }
	  break;
       }
       if (aErrno == EPERM) errnoLog << "EPERM";
       if (aErrno == EACCES) errnoLog << "EACCES";
       if (aErrno == EPERM || aErrno == EACCES) {
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "The directory containing oldpath has the sticky bit (S_ISVTX) set and the process's"
		  " effective user ID  is  neither the user ID of the file to be deleted nor that of "
		  "the directory containing it, and the process is not privileged (Linux: does not "
		  "have the CAP_FOWNER capability); or newpath  is  an existing  file  and the "
		  "directory containing it has the sticky bit set and the process?s effective user "
		  "ID is neither the user ID of the file to be replaced nor that of the directory "
		  "containing it, and  the  process  is  not  privileged  (Linux:  does  not have "
		  "the CAP_FOWNER capability); or the filesystem containing pathname does not support "
		  "renaming of the type requested.";
	  }
	  break;
       }
       if (aErrno == EROFS) {
	  errnoLog << "EROFS";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "The file is on a read-only filesystem.";
	  }
	  break;
       }
       if (aErrno == EXDEV) {
	  errnoLog << "EXDEV";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "oldpath and newpath are not on the same mounted filesystem.  (Linux permits  a  "
		  "filesystem  to  be mounted at multiple points, but rename(2) does not work across "
		  "different mount points, even if the same filesystem is mounted on both.)";
	  }
	  break;
       }

    } while(false);
    errnoLog << '\0';
    wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}

void BbbErrno::remove(char *aFile, int aLine, int aErrno, char* aInfo)
{
    std::strstream errnoLog;
    errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
    // dummy switch
    do {
       errnoLog << ", ";
       // On failure, aErrno is set to one of the following values:
       if (aErrno == EACCES) {
	  errnoLog << "EACCES";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
              "Write access to the directory containing pathname is not allowed for the process's effective UID, or one of the"
              "directories in pathname did not allow search permission.  (See also path_resolution(2).)";
	  }
	  break;
       }
       if (aErrno == EBUSY) {
	  errnoLog << "EBUSY";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
              "The  file  pathname cannot be unlinked because it is being used by the system or another process and the "
              "implementation considers this an error.";
	  }
	  break;
       }
       if (aErrno == EFAULT) {
	  errnoLog << "EFAULT";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "pathname points outside your accessible address space.";
	  }
	  break;
       }
       if (aErrno == EIO) {
	  errnoLog << "EIO";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		      "An I/O error occurred.";
	  }
	  break;
       }
       if (aErrno == EISDIR) {
	  errnoLog << "EISDIR";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "pathname refers to a directory.  (This is the non-POSIX value returned by Linux since 2.1.132.)";
	  }
	  break;
       }
       if (aErrno == ELOOP) {
	  errnoLog << "ELOOP";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "Too many symbolic links were encountered in translating pathname.";
	  }
	  break;
       }
       if (aErrno == ENAMETOOLONG) {
	  errnoLog << "ENAMETOOLONG";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "pathname was too long.";
	  }
	  break;
       }
       if (aErrno == ENOENT) {
	  errnoLog << "ENOENT";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "A component in pathname does not exist or is a dangling symbolic link, or pathname is empty.";
	  }
	  break;
       }
       if (aErrno == ENOMEM) {
	  errnoLog << "ENOMEM";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                      "Insufficient kernel memory was available.";
	  }
	  break;
       }
       if (aErrno == ENOTDIR) {
	  errnoLog << "ENOTDIR";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "A component used as a directory in pathname is not, in fact, a directory.";
	  }
	  break;
       }
       if (aErrno == EPERM) errnoLog << "EPERM";
       if (aErrno == EACCES) errnoLog << "EACCES";
       if (aErrno == EPERM || aErrno == EACCES) {
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "The directory containing pathname has the sticky bit (S_ISVTX) set and the process's"
		  " effective UID  is  neither the UID of the file to be deleted nor that of "
		  "the directory containing it, and the process is not privileged (Linux: does not "
		  "have the CAP_FOWNER capability).";
	  }
	  break;
       }
       if (aErrno == EROFS) {
	  errnoLog << "EROFS";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "pathname refers to a file on a read-only filesystem.";
	  }
	  break;
       }
    } while(false);
    errnoLog << '\0';
    wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}

void BbbErrno::mkdir(char *aFile, int aLine, int aErrno, char* aInfo)
{
    std::strstream errnoLog;
    errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
    // dummy switch
    do {
       errnoLog << ", ";
       // On failure, aErrno is set to one of the following values:
       if (aErrno == EACCES) {
	  errnoLog << "EACCES";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "The  parent directory does not allow write permission to the process, or one of the directories in pathname did "
		  "not allow search permission.  (See also path_resolution(2).)";
	  }
	  break;
       }
       if (aErrno == EEXIST) {
	  errnoLog << "EEXIST";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "pathname already exists (not necessarily as a directory).  This includes the case where pathname is a  symbolic "
		  "link, dangling or not.";
	  }
	  break;
       }
       if (aErrno == EFAULT) {
	  errnoLog << "EFAULT";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "pathname points outside your accessible address space.";
	  }
	  break;
       }
       if (aErrno == ELOOP) {
	  errnoLog << "ELOOP";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "Too many symbolic links were encountered in resolving pathname.";
	  }
	  break;
       }
       if (aErrno == ENAMETOOLONG) {
	  errnoLog << "ENAMETOOLONG";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "pathname was too long.";
	  }
	  break;
       }
       if (aErrno == ENOENT) {
	  errnoLog << "ENOENT";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "A directory component in pathname does not exist or is a dangling symbolic link.";
	  }
	  break;
       }
       if (aErrno == ENOMEM) {
	  errnoLog << "ENOMEM";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                      "Insufficient kernel memory was available.";
	  }
	  break;
       }
       if (aErrno == ENOSPC) {
	  errnoLog << "ENOSPC";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "The device containing pathname has no room for the new directory. Or "
		  "The new directory cannot be created because the user's disk quota is exhausted.";
	  }
	  break;
       }
       if (aErrno == ENOTDIR) {
	  errnoLog << "ENOTDIR";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "A component used as a directory in pathname is not, in fact, a directory.";
	  }
	  break;
       }
       if (aErrno == EPERM) {
	   errnoLog << "EPERM";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "The filesystem containing pathname does not support the creation of directories.";
	  }
	  break;
       }
       if (aErrno == EROFS) {
	  errnoLog << "EROFS";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "pathname refers to a file on a read-only filesystem.";
	  }
	  break;
       }
    } while(false);
    errnoLog << '\0';
    wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}

void BbbErrno::opendir(char *aFile, int aLine, int aErrno, char* aInfo)
{
    std::strstream errnoLog;
    errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
    // dummy switch
    do {
       errnoLog << ", ";
       // On failure, aErrno is set to one of the following values:
       if (aErrno == EACCES) {
	  errnoLog << "EACCES";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "Permission denied.";
	  }
	  break;
       }
       if (aErrno == EMFILE) {
	  errnoLog << "EMFILE";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "Too many file descriptors in use by process. Or "
		  "Too many files are currently open in the system.";
	  }
	  break;
       }
       if (aErrno == ENOENT) {
	  errnoLog << "ENOENT";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                  "Directory does not exist, or name is an empty string.";
	  }
	  break;
       }
       if (aErrno == ENOMEM) {
	  errnoLog << "ENOMEM";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
                      "Insufficient memory to complete the operation.";
	  }
	  break;
       }
       if (aErrno == ENOTDIR) {
	  errnoLog << "ENOTDIR";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "name is not a directory.";
	  }
	  break;
       }
    } while(false);
    errnoLog << '\0';
    wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}


void BbbErrno::fork(char *aFile, int aLine, int aErrno, char* aInfo)
{
    std::strstream errnoLog;
    errnoLog << aFile << ':' << aLine << " errno = " << aErrno;
    // dummy switch
    do {
       errnoLog << ", ";
       // On failure, aErrno is set to one of the following values:
       if (aErrno == EAGAIN) {
	  errnoLog << "EAGAIN";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "fork()  cannot  allocate  sufficient memory to copy the parent's "
		  "page tables and allocate a task structure for the child. Or..."
		  "It was not possible to create a new process because the caller's "
		  "RLIMIT_NPROC  resource  limit  was  encountered.  To exceed this "
		  "limit, the process must have either  the  CAP_SYS_ADMIN  or  the "
		  "CAP_SYS_RESOURCE capability.";
	  }
	  break;
       }
       if (aErrno == ENOMEM) {
	  errnoLog << "ENOMEM";
	  if (BbbErrno::getDebug()) {
	      errnoLog << ", "
		  "fork()  failed  to  allocate  the  necessary  kernel  structures "
		  "because memory is tight.";
	  }
	  break;
       }
    } while(false);
    errnoLog << '\0';
    wrn << errnoLog.str() << ", " << aInfo << ", " << strerror(aErrno) << endl;
}

