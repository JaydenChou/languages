/******************************************************************************
 *   File: BbbQueue.h
 *   
 *   Comments: Declaracion de una clase para manejo de message queues
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
#ifndef __BBBQUEUE_H__
#define __BBBQUEUE_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "tmsg.h"

/**
 *  Defines
 */
#define QUEUE_PERMS          0666

class BbbQueue {

protected:
    key_t _key;                 // Message queue key
    int   _msqid;               // Message queue id
    struct msqid_ds _msqds;     // Message queue data structure
    int    _msgtyp;				// Tipo de Message queue (usado por msgrcv)
    int    _msgflg;				// Message flag para posix (usado por msgrcv)
    int    _errno;		// ultimo errno
    int    _timeout;		// timeout 
    bool   _destroyable;

    static const int PERMS = QUEUE_PERMS;

private:
    void setMsqid(signed int value);
    void close();

public:
    BbbQueue();
    BbbQueue(key_t akey);
    ~BbbQueue();

    void setDestroyable();

    signed int getMsqid() const;
    signed int getKey() const;
    signed int getErrno() const;

    unsigned short getNumberOfMessages();
    size_t receiveMessage(tMsg& msgp, int msgtyp = TIP_CUALQU);
    bool sendMessage(tMsg& msgp, size_t msgsz);

};

#endif // __BBBQUEUE_H__
