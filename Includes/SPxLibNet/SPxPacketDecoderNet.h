/*********************************************************************
*
* (c) Copyright 2008 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPacketDecoderNet class.
*
*	This is a generic class that may be used to decode an SPx
*	packet stream received on the network.
*	It parses the messages given to it and invokes callback functions
*	according to the packet types found.
*
*
* Revision Control:
*   12/05/22 v1.33   AGC	Support optional firewall checks.
*
* Previous Changes:
*   06/04/22 1.32   AGC	Add parameter support.
*   11/11/21 1.31   AGC	Improve packet sender/decoder socket sharing.
*   07/01/21 1.30   AGC	Make GetPort() virtual.
*   27/11/20 1.29   SP 	Fix N/C #809 - Throttle error reporting and 
*			avoid busy wait on socket error.
*   19/11/19 1.28   AGC	Revert last change.
*   18/11/19 1.27   AGC	Add option for external mutex.
*   18/10/18 1.26   AGC	Changes for improved testing.
*   15/10/18 1.25   SP 	Make StartDecodeThread() and friends virtual.
*   15/06/18 1.24   AGC	Set thread name.
*   17/05/18 1.23   AGC	Overload PrepareForRestart().
*   06/03/18 1.22   REW	Add SendData().
*   08/02/18 1.21   AGC	Add CloseSocket().
*   31/10/17 1.20   AGC	Add CheckSocket().
*   08/03/17 1.19   AGC	Use atomic variables.
*   11/01/17 1.18   AGC	Improve thread start/shutdown logic.
*   04/01/17 1.17   AGC	Add GetSocket().
*   23/09/16 1.16   AGC	Improve mutex behaviour.
*   02/09/16 1.15   AGC	Use atomic variable for change pending.
*   23/08/16 1.14   AGC	Make PollSocket() virtual.
*   23/06/15 1.13   AGC	Support TCP receipt.
*   26/01/15 1.12   SP 	Derive from SPxNetAddr.
*                       Make network Get...() functions const.
*   26/11/13 1.11   AGC	Remove unused m_invalidSocket.
*   04/10/13 1.10   AGC	Simplify headers.
*   24/05/13 1.9    AGC	Add IsDecodeThreadStarted().
*   14/11/11 1.8    AGC	Return error from updateAddress().
*			Use const char * for address params.
*   25/10/11 1.7    AGC	Add GetIfAddressString() function.
*   07/09/11 1.6    AGC	Add GetInitialised() function.
*   09/08/11 1.5    AGC Report sender address correctly. 
*   09/06/11 1.4    AGC	Add GetAddressString() function.
*   06/04/11 1.3    AGC	Rename class/file to SPxPacketDecoderNet.
*			Refactor to use new SPxPacketDecoder class.
*   04/10/10 1.2    AGC	Support running decoder in a thread.
*   16/09/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PACKET_DECODER_NET_H
#define _SPX_PACKET_DECODER_NET_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need SPxPacketDecoder base classes. */
#include "SPxLibData/SPxPacketDecoder.h"
#include "SPxLibNet/SPxNetAddr.h"
#include "SPxLibNet/SPxSocketChangeIface.h"


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

/* Forward declarations. */
class SPxThread;

/*
 * Define our class.
 */
class SPxPacketDecoderNet :public SPxPacketDecoder, public SPxNetAddr, public SPxSocketChangeIface
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxPacketDecoderNet(void);
    virtual ~SPxPacketDecoderNet(void);

    /* Address/port control. */
    SPxErrorCode SetAddress(const char *address, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetAddress(UINT32 address, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetPort(int port) { return SetPortNumber(port); }
    /* The SetPort function calls the SetPortNumber because the winspool
     * header defines SetPort to be either SetPortW or SetPortA which causes
     * issues when linking to a function called SetPort. This inline function
     * does not suffer from this problem.
     */
    SPxErrorCode SetPortNumber(int port);
    UINT32 GetAddress(void) const;
    const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    UINT32 GetIfAddress(void) const;
    const char *GetIfAddressString(void) const;
    virtual int IsTCPSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetTCP(int isTCP);
    virtual int IsTCP(void) const { return m_isTCP; }

    /* Decode thread functions. */
    virtual SPxErrorCode StartDecodeThread(const char *name=NULL);
    virtual SPxErrorCode StopDecodeThread(void);
    virtual int IsDecodeThreadStarted(void) const;
    virtual SPxErrorCode PrepareForRestart(void);

    virtual SPxErrorCode CheckSocket(void);
    SPxErrorCode CloseSocket(void);

    SPxErrorCode EnableFirewallCheck(int enable);
    int IsFirewallCheckEnabled(void) const { return m_isFirewallCheckEnabled; }

    /* Poll the socket for packets. */
    virtual SPxErrorCode PollSocket(void);

    /* Decoding functions. */
    SPxErrorCode DecodePackets(unsigned char *buf, unsigned int numBytes,
				struct sockaddr_in *fromAddr = NULL);

    SPxErrorCode GetInitialised(void) const { return m_isBoundOrConnected ? SPX_NO_ERROR : SPX_ERR_UNKNOWN; }

    /* Access to underlying socket if needed. */
    virtual SOCKET_TYPE GetSocket(void) { return m_socket; }

    /* Function to allow dta to be sent out on socket if in TCP mode.
     * Returns the number of bytes actually sent (like send() does).
     */
    virtual int SendData(const void *buf, unsigned int numBytes);

    /* Add/remove callback for socket change. */
    virtual SPxErrorCode AddSocketChangeCallback(SPxSocketChangeCallback fn,
						 void *userArg);
    virtual SPxErrorCode RemoveSocketChangeCallback(SPxSocketChangeCallback fn,
						    void *userArg);

    /* Debug/log reporting. */
    void SetLogFile(FILE *logFile) { m_logFile = logFile; }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;
    /* Critical section. */
    mutable SPxCriticalSection m_mutex;
    SPxAtomic<int> m_changePending;

    /* Decode thread. */
    SPxThread *m_thread;
    SPxAtomic<int> m_threadRunning;

    unsigned char *m_buf;		/* Buffer for reading data */
    unsigned int m_bufSize;		/* Size of readBuf in bytes */
    unsigned int m_timeoutMsecs;	/* Time out for reading from socket. */

    /* Socket for receiving radar images etc. */
    int m_isTCP;			/* TCP or UDP? */
    SOCKET_TYPE m_socket;		/* Socket to receive via */
    struct sockaddr_in m_socketAddress;	/* Address structure */
    SPxAtomic<UINT32> m_addressCache;
    SPxAtomic<int> m_portCache;
    SPxAtomic<UINT32> m_ifAddressCache;
    SPxAtomic<char*> m_addressString;	/* String with dotted-IP address */
    SPxAtomic<char*> m_ifAddrString;		/* Interface address for multicast, or NULL */
    struct sockaddr_in m_interfaceAddr;	/* Used if ifAddrString not NULL */
    int m_port;				/* Port to receive from */ 
    SPxAtomic<int> m_isBoundOrConnected;
    UINT32 m_lastCheckSocketErrTime;	/* Time last error reported by checkSocket(). */

    /* Firewall check. */
    UINT32 m_lastDataTime;
    int m_isFirewallCheckEnabled;

    /* Debug/log reporting. */
    FILE *m_logFile;			/* NULL or destination for debug */

    /*
     * Private functions.
     */
    SPxErrorCode checkSocket(void);
    SPxErrorCode closeSocket(void);
    static void* decodeThreadWrapper(SPxThread *thread);
    void* decodeThreadFn(SPxThread *thread);
    SPxErrorCode updateAddress(void);
    int threadReadDataFn(unsigned char *buf,
			 unsigned int bufSizeBytes,
			 unsigned int timeoutMsecs,
			 sockaddr_in *from);

}; /* SPxPacketDecoderNet */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_PACKET_DECODER_NET_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
