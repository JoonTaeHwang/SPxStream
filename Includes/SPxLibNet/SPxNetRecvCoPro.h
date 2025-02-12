/*********************************************************************
*
* (c) Copyright 2008, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetRecvCoPro.h,v $
* ID: $Id: SPxNetRecvCoPro.h,v 1.3 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*	Header for network receiving class that can use a
*	co-processor server to assist with decompression.
*
*
* Revision Control:
*   04/10/13 v1.3    AGC	Simplify headers.
*
* Previous Changes:
*   14/11/11 1.2    SP 	Add data arg to ThreadDecodeReturnFn().
*   25/09/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECVCOPRO_H
#define _SPX_NETRECVCOPRO_H

/*
 * Other headers required.
 */
/* We need the base class header (which includes several others as well). */
#include "SPxLibNet/SPxNetRecv.h"


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

/*
 * Define our class, derived from the standard one.
 */
class SPxNetworkReceiveCoPro :public SPxNetworkReceive
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxNetworkReceiveCoPro(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveCoPro();

    /* Specification of what co-processor server to use. */
    SPxErrorCode SetCoProServer(char *address, int port=0);
    SPxErrorCode SetCoProServer(UINT32 adresss, int port=0);

    /* Information retrieval functions. */
    UINT32 GetCoProAddress(void);
    int GetCoProPort(void);
    int IsConnected()	{ return(m_cpConnected); }

    /* Overwrite the base class functions we want to handle ourselves. */
    virtual int ThreadDecodeReturnFn(SPxReturnHeader_tag *hdr, 
                                     unsigned char *data);
    virtual int ThreadBackgroundFn(void);

private:
    /*
     * Private fields.
     */
    /* Details of the co-processor server we're using. */
    SOCKET_TYPE m_cpSocket;		/* The socket we talk via */
    struct sockaddr_in m_cpSocketAddress; /* The address of the server */
    char *m_cpAddressString;		/* Possibly including :port */
    int m_cpPort;			/* May be overridden by address */
    int m_cpConnected;			/* Are we connected to the server? */
    UINT32 m_cpLastConnectAttempt;	/* Time of last connection attempt */

    /* Receipt of decoded returns from the server. */
    unsigned char *m_cpDecodeBuffer;	/* Buffer for decoded hdr/data */
    unsigned int m_cpDecodeBufferSize;	/* Size of m_cpDecodeBuffer */
    unsigned int m_cpDecodeBytesGot;	/* How many bytes do we have? */

    /*
     * Private functions.
     */
    SPxErrorCode tryToConnectToServer(void);
    SPxErrorCode disconnectFromServer(void);
    SPxErrorCode checkForDecodedReturns(void);
}; /* SPxNetworkReceiveCoPro class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVCOPRO_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
