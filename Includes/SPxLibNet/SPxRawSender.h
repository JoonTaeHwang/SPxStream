/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRawSender class.
 *
 *   This is a generic class that is used to send data
 *   using a SOCK_RAW socket.
 *
 * Revision Control:
 *   12/05/22 v1.1    SP 	Initial version.
 *
 * Previous Changes:
 *
 **********************************************************************/

#ifndef _SPX_RAW_SENDER_H
#define _SPX_RAW_SENDER_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"

/* For SPxNetAddr interface. */
#include "SPxLibNet/SPxNetAddr.h"

/* For SPX_SOCK_INVALID. */
#include "SPxLibSc/SPxScNet.h"

/*********************************************************************
 *
 *   Constants
 *
 **********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 **********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Forward-declare classes we may use. */

/*
 * Define our class.
 */
class SPxRawSender :public SPxObj, public SPxNetAddr
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxRawSender(int protocol);
    virtual ~SPxRawSender(void);

    /* Address/port control. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0,
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0,
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetPort(int port) { return SetPortNumber(port); }
    /* The SetPort function calls the SetPortNumber because the winspool
     * header defines SetPort to be either SetPortW or SetPortA which causes
     * issues when linking to a function called SetPort. This inline function
     * does not suffer from this problem.
     */
    SPxErrorCode SetPortNumber(int port);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const
    {
        return(m_ifAddrString ? m_ifAddrString : "");
    }
    virtual SPxErrorCode SetTTL(int ttl);
    virtual int GetTTL(void) const { return m_ttl; }
    virtual SPxErrorCode SetMTU(unsigned int mtu);
    virtual unsigned int GetMTU(void) { return m_mtu; }

    /* Sending functions. */
    virtual SPxErrorCode SendBuffer(const void *buf,
                                    unsigned int bufSizeBytes,
                                    int mutexAlreadyLocked=FALSE);

    /* Receiving functions. */
    virtual int RecvBuffer(unsigned char *buf, 
                           unsigned int bufSizeBytes,
                           unsigned int timeoutMsecs, 
                           sockaddr_in *from,
                           int mutexAlreadyLocked=FALSE);

    /* Socket blocking. */
    virtual SPxErrorCode SetSocketBlocking(int block);

    /* Send errors. */
    void EnableSendErrors(int enable);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /* Debug/log reporting. */
    void SetLogFile(FILE *logFile) { m_logFile = logFile; }

protected:
    /*
     * Protected variables.
     */
    /* Critical section. */
    mutable SPxCriticalSection m_mutex;

    /* Debug/log reporting. */
    FILE *m_logFile;  /* NULL or destination for debug */

    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

    /* Convenient copy of an invalid socket handle. */
    SOCKET_TYPE m_invalidSocket;

    /* Socket for sending out the radar images etc. */
    int m_protocol;                         /* Socket protocol. */
    SOCKET_TYPE m_socket;                   /* Socket to send via */
    struct sockaddr_in m_socketAddress;     /* Address structure */
    struct sockaddr_in m_interfaceAddr;     /* Used if ifAddrString not NULL */
    char *m_addressString;                  /* String with dotted-IP address */
    char *m_ifAddrString;                   /* Interface string with dotted-IP address */
    int m_port;                             /* Port to send to */
    int m_ttl;                              /* Time to live. */
    unsigned int m_mtu;                     /* Maximum transmission unit size. */

    /* Buffer for building messages in. */
    unsigned char *m_buffer;                /* Pointer to buffer */
    unsigned int m_bufferSizeBytes;         /* Size of buffer */

    /* Send errors. */
    int m_enableSendErrors;

    /*
     * Private functions.
     */
    SPxErrorCode setupSocket(void);
    SPxErrorCode ensureBufferBigEnough(unsigned int sizeBytes);
    SPxErrorCode sendData(const unsigned char *buf, int numBytes);

}; /* SPxRawSender */

/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_RAW_SENDER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
