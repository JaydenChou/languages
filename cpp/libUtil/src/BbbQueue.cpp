/******************************************************************************
 *   File: BbbQueue.cpp
 *   
 *   Comments: Implementacion de una clase para manejo de message queues
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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#ifndef _ALLOW_STD_TYPES_
#define _ALLOW_STD_TYPES_
#endif
#include "BbbLog.h"
#include "BbbQueue.h"
#include "BbbErrno.h"
#include "BbbLog.h"

/**
 * Constructor por default
 */
BbbQueue::BbbQueue() 
{
    dbg << endl;
    _msgtyp = TIP_CUALQU;
    _msgflg = 0;
    _errno = errno;
    _destroyable = false;
}


/**
 * Constructor con una key
 * 
 * @param akey
 */
BbbQueue::BbbQueue(key_t akey)
{
    dbg << endl;
    _errno = errno;
    _key = akey;
    _destroyable = false;
    _msgtyp = TIP_CUALQU;
    _msgflg = 0;
    assert(_key > 0);     // la key no puede ser 0 o menor
    int queue = msgget(_key, BbbQueue::PERMS | IPC_CREAT);
    if (queue < 0) {
        err << "Intentando abrir o crear queue 0x" << uppercase << hex << getKey() << dec << endl;
        BbbErrno::msgget(__FILE__, __LINE__, errno, "Intentando abrir o crear queue");
        assert(queue > 0);   // Inicio: falla msgget, no se pudo crear o abrir la cola
    }
    setMsqid(queue);
}


/**
 * Destructor, solo de la queue con cierta propiedad
 * destructible
 */
BbbQueue::~BbbQueue()
{
    dbg << endl;
    if (_destroyable == true) {
       close();
    }
}

void BbbQueue::setDestroyable() {
    dbg << endl;
    _destroyable = true;
}

/**
 * Cierra la cola usando IPC_RMID
 */
void BbbQueue::close()
{
    dbg << endl;
    inf << "Cerrando queue 0x" << uppercase << hex << getKey() << dec << endl;
    _errno = errno;

    assert(getMsqid() != -1);   // ya estaba cerrado

    if (msgctl(getMsqid(), IPC_RMID, 0) == -1) {
        BbbErrno::msgctl(__FILE__, __LINE__, errno, "Intentando cerrar queue");
	_errno = errno;
	wrn << "falla al cerrar la queue 0x" << uppercase << hex << getKey() << dec << endl;
    }
    setMsqid(-1);
}



/**
 * Obtiene el actual numero de mensajes de la cola
 * 
 * @return unsigned short
 */
unsigned short BbbQueue::getNumberOfMessages()
{
    dbg << endl;
    _errno = errno;
    if (msgctl(getMsqid(), IPC_STAT, &_msqds) != 0) {
        BbbErrno::msgctl(__FILE__, __LINE__, errno, "Obteniendo cantidad de mensajes");
	err << "falla al obtener cantidad de mensajes la queue " 
	    << "0x" << uppercase << hex << getKey() << dec << endl;
	_errno = errno;
        assert(0);
    }
    return _msqds.msg_qnum;
}


/**
 * 
 * Recuperar un elemento de la cola
 * 
 * @param msgp
 * @param msgtyp
 * 
 * @return size_t
 */
size_t BbbQueue::receiveMessage(tMsg& msgp, int msgtyp)
{
    dbg << endl;
    size_t msgsz = sizeof(struct tMsg);

    inf << "Esperando que venga un msg en "
        << "0x" << uppercase << hex << getKey() << dec
        << endl;
    ssize_t size = msgrcv(getMsqid(), (void*)&msgp, msgsz, msgtyp, _msgflg);
    _errno = errno;
    if (size == -1) {
        BbbErrno::msgrcv(__FILE__, __LINE__, _errno, "Al obtener mensajes de la cola");
    }
    return size;
}


/**
 * Recuperar un elemento de la cola
 * 
 * @param msgp
 * 
 * @return bool
 */
bool BbbQueue::sendMessage(tMsg& msgp, size_t msgsz)
{
    dbg << endl;
    bool rta = false;
    inf << "Enviando msg a "
        << "0x" << uppercase << hex << getKey() << dec
        << endl;
    int response = msgsnd(getMsqid(), (void*)&msgp, msgsz, IPC_NOWAIT);
    _errno = errno;
    if (response == -1) {
	rta = false; 
	wrn << "falla al enviar mensaje por queue: " 
	    << "0x" << uppercase << hex << getKey() << dec << endl;
        BbbErrno::msgsnd(__FILE__, __LINE__, _errno, "Al enviar mensajes de la cola");
    } else {
	rta = true;
    }
    return rta;
}

/**
 * Obtiene la _msgqid (es protegida)
 * 
 */

int BbbQueue::getMsqid() const{
    return (*this)._msqid;
}

/**
 * Setea la _msgqid (es protegida)
 * @param value
 */
void BbbQueue::setMsqid(int value){
    (*this)._msqid = value;
}

/**
 * Obtiene la _key (es protegida)
 */
int BbbQueue::getKey() const{
    return (*this)._key;
}

/**
 * Obtiene la _errno (es protegida)
 */
int BbbQueue::getErrno() const{
    return (*this)._errno;
}
