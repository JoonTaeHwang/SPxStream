/*********************************************************************
*
* (c) Copyright 2008 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPacketSender class.
*
*	This is a generic class that may be used to send any SPxPacket
*	packets across the network.  Alternatively, it may be derived
*	from in order to make specific packet sending objects.
*
*
* Revision Control:
*   21/06/22 v1.34   AGC	Add silent option to GetSenderPort().
*
* Previous Changes:
*   08/04/22 1.33   SP 	Add SetMTU().
*   11/11/21 1.32   AGC	Improve packet sender/decoder socket sharing.
*   12/03/21 1.31   SP 	Exclude inhibit CAT247 param by default.
*   25/11/20 1.30   AGC	Support excluding inhibit CAT247 param.
*   02/10/20 1.29   REW	Support inhibiting CAT247.
*   27/08/20 1.28   AGC	Add EnableSendErrors().
*   17/07/20 1.27   SP 	Revert last change.
*   15/07/20 1.26   SP 	Add SendPacketBJSON().
*   24/02/20 1.25   SP 	Lock mutex in GetSenderPort().
*   16/01/20 1.24   REW	SendRawBuffer() can optionally send CAT247 too.
*   17/09/19 1.23   SP 	Further changes to support output manager.
*   23/08/19 1.22   REW	Add RecvRawBuffer().
*   17/05/18 1.21   AGC	Overload PrepareForRestart().
*   04/01/17 1.20   AGC	Support use with pre-existing socket.
*   23/09/16 1.19   AGC	Add GetSocket().
*   26/11/15 1.18   AGC	Improve error messages for TCP connections.
*   14/10/15 1.17   AGC	GetSenderPort() now const.
*   23/06/15 1.16   AGC	Allow dynamic TCP/UDP change.
*   17/03/15 1.15   AGC	Make some protected variables private.
*   06/03/15 1.14   SP 	Support sending on a TCP socket.
*   10/04/14 1.13   AGC	Make Send function virtual.
*   22/11/13 1.12   AGC	Use const void* for payload params.
*			Make get functions const.
*   04/10/13 1.11   AGC	Simplify headers.
*   31/08/12 1.10   AGC	Derive from SPxNetAddr interface.
*   29/05/12 1.9    AGC	Add TTL support.
*   13/01/12 1.8    AGC	Use const for payload parameters.
*   14/11/11 1.7    AGC	Use const char * for address params.
*   22/09/11 1.6    REW	Add GetIfAddressString().
*   24/05/11 1.5    AGC	Add GetSenderPort() function.
*   05/01/11 1.4    AGC	Add GetAddressString() function.
*   04/10/10 1.3    AGC	Support interface address and blocking socket.
*   13/11/08 1.2    REW	GetParameter() returns value in a string.
*   16/09/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PACKET_SEND_H
#define _SPX_PACKET_SEND_H

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
class SPxAsterixEncoder;
class SPxSocketChangeIface;

/*
 * Define our class.
 */
class SPxPacketSender :public SPxObj, public SPxNetAddr
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxPacketSender(int isTCP=FALSE, SOCKET_TYPE sock=SPX_SOCK_INVALID,
			     int includeInhibitCat247Param=FALSE,
			     SPxSocketChangeIface *sockChangeObj=NULL);
    virtual ~SPxPacketSender(void);

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
    virtual int IsTCPSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetTCP(int isTCP);
    virtual int IsTCP(void) const { return m_isTCP; }
    virtual SPxErrorCode SetTTL(int ttl);
    virtual int GetTTL(void) const { return m_ttl; }
    virtual SPxErrorCode SetMTU(unsigned int mtu);
    virtual unsigned int GetMTU(void) { return m_mtu; }
    virtual int GetSenderPort(int silent=FALSE) const;

    /* Use to accept and query connections on a TCP socket. */
    virtual SPxErrorCode TryAccept(void);
    virtual int IsClientConnected(void);
    virtual UINT32 GetClientAddr(void);

    /* Sending functions. */
    virtual SPxErrorCode SendRawBuffer(const void *buf,
				unsigned int bufSizeBytes,
				int mutexAlreadyLocked=FALSE,
				SPxAsterixEncoder *asterixEncoder=NULL);
    virtual SPxErrorCode SendPacketA(UINT8 packetType,
				const void *payload,
				unsigned int payloadSizeBytes,
				int mutexAlreadyLocked=FALSE);
    virtual SPxErrorCode SendPacketB(UINT16 packetType,
				UINT32 timeSecs, UINT32 timeUsecs,
				const void *payload,
				unsigned int payloadSizeBytes,
				int mutexAlreadyLocked=FALSE);
    virtual SPxErrorCode SendPacketB(UINT16 packetType,
				SPxTime_t ts,
				const void *payload,
				unsigned int payloadSizeBytes,
				int mutexAlreadyLocked=FALSE)
    {
	return( SendPacketB(packetType, ts.secs, ts.usecs,
			    payload, payloadSizeBytes, mutexAlreadyLocked) );
    }

    /* Receiving functions. */
    virtual int RecvRawBuffer(unsigned char *buf, unsigned int bufSizeBytes,
				unsigned int timeoutMsecs, sockaddr_in *from);

    /* Socket blocking. */
    virtual SPxErrorCode SetSocketBlocking(int block);

    /* Send errors. */
    void EnableSendErrors(int enable);

    SOCKET_TYPE GetSocket(void) { return m_socket; }

    virtual SPxErrorCode PrepareForRestart(void);

    /* CAT247 control. */
    void SetInhibitCat247(int inhibit)	{ m_inhibitCat247 = inhibit; }
    int GetInhibitCat247(void) const	{ return(m_inhibitCat247); }

    /* Overrides function in the base class. */
    virtual void SetActive(int active);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Debug/log reporting. */
    void SetLogFile(FILE *logFile) { m_logFile = logFile; }


protected:
    /*
     * Protected variables.
     */
    /* Critical section. */
    mutable SPxCriticalSection m_mutex;

    /* Debug/log reporting. */
    FILE *m_logFile;			/* NULL or destination for debug */

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
    int m_isTCP;                        /* Is socket TCP or UDP? */
    int m_isSockFromUser;
    SOCKET_TYPE m_socket;		/* Socket to send via */
    SPxSocketChangeIface *m_socketChangeObj;	/* Socket change interface. */
    struct sockaddr_in m_socketAddress;	/* Address structure */
    struct sockaddr_in m_interfaceAddr; /* Used if ifAddrString not NULL */
    char *m_addressString;		/* String with dotted-IP address */
    char *m_ifAddrString;		/* Interface string with dotted-IP address */
    int m_port;				/* Port to send to */ 
    int m_ttl;				/* Time to live. */
    unsigned int m_mtu;			/* Maximum transmission unit size. */

    /* TCP socket created when connected. */
    SOCKET_TYPE m_clientSocket;                 /* Connected TCP socket. */
    struct sockaddr_in m_clientSocketAddress;   /* TCP socket address */

    /* Buffer for building messages in. */
    unsigned char *m_buffer;		/* Pointer to buffer */
    unsigned int m_bufferSizeBytes;	/* Size of buffer */

    /* Info regarding version sending. */
    int m_inhibitCat247;		/* Do we inhibit them? */
    UINT32 m_lastCat247VersionMsecs;	/* Last time we sent version info */

    /* Flags. */
    int m_dataSent;			/* Has any data been sent to the current address/port. */

    /* Send errors. */
    int m_enableSendErrors;

    /*
     * Private functions.
     */
    SPxErrorCode setupSocket(int isTCP);
    SPxErrorCode ensureBufferBigEnough(unsigned int sizeBytes);
    SPxErrorCode sendData(const unsigned char *buf, int numBytes);
    SPxErrorCode tryAccept(void);
    static void socketChanged(SOCKET_TYPE sock, void *userArg);

}; /* SPxPacketSender */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_PACKET_SEND_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
