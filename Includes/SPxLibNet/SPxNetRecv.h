/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for base class of network receiving objects.
*
*
* Revision Control:
*   26/09/22 v1.50   AGC	Generate error for bind issues.
*
* Previous Changes:
*   18/06/20 1.49   REW	Support m_derivedClassProjCode.
*   24/04/19 1.48   SP 	Support receipt of compressed 1, 2 and 4-bit samples.
*   10/04/19 1.47   SP 	Support multiple RIBs in a derived class.
*   03/05/18 1.46   REW	Re-factor common code into closeConnection().
*   09/05/17 1.45   AGC	Add CheckForChanges() function for testing.
*   20/04/17 1.44   AGC	Simplify firewall check.
*   18/04/17 1.43   AGC	Back-off when installing data function.
*   12/04/17 1.42   AGC	Add firewall check.
*   21/02/17 1.41   AGC	Lock mutex while installing data function.
*   29/11/16 1.40   REW	Add SetPaused().
*   23/09/16 1.39   AGC	Improve mutex behaviour.
*   29/07/15 1.38   SP 	Support TCP keep-alive.
*   15/05/15 1.37   AGC	Make mutex mutable.
*   25/03/15 1.36   REW	Allow derived classes to stop socket creation.
*   10/04/14 1.35   SP 	Add GetTimeOfLastDataMsecs().
*   22/11/13 1.34   AGC	Make get functions const.
*   04/11/13 1.33   SP 	Support non-blocking TCP socket.
*   04/10/13 1.32   AGC	Simplify headers.
*   02/10/13 1.31   REW	Add sendDataLocked().
*   15/05/13 1.30   AGC	Report bad message errors every 10 seconds.
*   25/04/13 1.29   AGC	Use new SPxCompress class.
*   13/02/13 1.28   REW	Support control over rcvbuf size.
*   04/02/13 1.27   REW	Support dropping multicast membership.
*   25/01/13 1.26   REW	Support dropout reporting.
*   13/11/12 1.25   AGC	Make SPxNetAddr derivation virtual.
*   24/10/12 1.24   REW	Support server-based client licensing.
*   31/08/12 1.23   AGC	Derive from SPxNetAddr interface.
*			Avoid nameclash with SetPort in winspool.
*   29/05/12 1.22   AGC	Add interface address param to SetAddress().
*			Add GetDebug() function.
*   09/02/12 1.21   REW	Add SendData() function.
*   14/11/11 1.20   AGC	Use const char * for address params.
* 			Add GetAddressString() function.
*   02/11/11 1.19   SP 	Allow ThreadDecodeReturnFn() to handle
*                       radar data in a separate buffer to header.
*   26/09/11 1.18   REW	Only force log file if debug is non zero.
*   02/03/10 1.17   REW	Add InstallDataFn().
*   10/07/09 1.16   REW	Add optional interface address to Create().
*   03/06/09 1.15   REW	Add SPX_NET_RECV_DEBUG_PROJ_A to D.
*   08/05/09 1.14   REW	Ensure m_logFile set if m_debug is.
*   08/04/09 1.13   REW	Support watchdogs for TCP connections.
*   09/02/09 1.12   REW	Implement GetInitialised().
*   28/11/08 1.11   REW	Keep knowledge of incoming config.
*   13/11/08 1.10   REW	GetParameter() returns value in a string.
*   27/07/08 1.9    REW	SetPort() is no longer virtual.
*   23/07/08 1.8    REW	Add ThreadSocketChanged() function.
*			Add GetAddress() and GetPort() functions.
*			Add a SetAddress() taking a UINT32.
*   15/02/08 1.7    REW	Add m_changePending flag.
*   04/11/07 1.6    REW	Support derived classes using TCP connections.
*   10/09/07 1.5    REW	Add GetParameter().
*   08/08/07 1.4    REW	Add m_ribFull and counts for dropout etc.
*   30/07/07 1.3    REW	Make debug fields protected not private.
*			Add m_defaultPort.
*   26/07/07 1.2    REW	Make network protocol match documented API.
*   25/07/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECV_H
#define _SPX_NETRECV_H

/*
 * Other headers required.
 */
/* We need SPXxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"

/* We need the radar source base class header. */
#include "SPxLibData/SPxRadarSource.h"

/* For SPxNetAddr interface. */
#include "SPxLibNet/SPxNetAddr.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Debug flags.
 */
#define	SPX_NET_RECV_DEBUG_SOCKET	0x00000001	/* Socket control */
#define	SPX_NET_RECV_DEBUG_STATE	0x00000002	/* State change */
#define	SPX_NET_RECV_DEBUG_PARAMS	0x00000004	/* Params */
#define	SPX_NET_RECV_DEBUG_DATA		0x00000010	/* Data */
#define	SPX_NET_RECV_DEBUG_DECODE	0x00000020	/* Decoding */
#define	SPX_NET_RECV_DEBUG_RECV		0x00000040	/* Receipt */
#define	SPX_NET_RECV_DEBUG_BANDWIDTH	0x00000080	/* Bandwidth */
#define	SPX_NET_RECV_DEBUG_PROJ_A	0x01000000	/* Project-specific */
#define	SPX_NET_RECV_DEBUG_PROJ_B	0x02000000	/* Project-specific */
#define	SPX_NET_RECV_DEBUG_PROJ_C	0x04000000	/* Project-specific */
#define	SPX_NET_RECV_DEBUG_PROJ_D	0x08000000	/* Project-specific */
#define	SPX_NET_RECV_DEBUG_VERBOSE	0x80000000	/* Verbose */


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
class SPxNetworkReceive;
class SPxCompress;
class SPxRIB;
class SPxThread;
struct SPxReturnHeader_tag;

typedef void (*SPxNetRecvDataFn_t)(SPxNetworkReceive *obj, 
				   void *userArg,
				   SPxReturnHeader_tag *header, 
				   unsigned char *decodedSamples);

/*
 * Define our network receiver base class, derived from generic radar sources.
 */
class SPxNetworkReceive :public SPxRadarSource, virtual public SPxNetAddr
{
public:
    /*
     * Public types.
     */

    /* Stream context. The fields should be initialised when
     * a stream is created by the caller and then treated as
     * read-only. Some fields will be modified by the object.
     */
    typedef struct
    {
	SPxRIB *rib;			/* RIB to write to. */
	int ribFull;			/* Do we think RIB filled up? */
	int ribCount;			/* Incrementing count for written returns. */
	SPxNetRecvDataFn_t fnData;	/* Return callback function. */
	void *fnDataArg;		/* Callback function arg. */
	int nominalLength;		/* Number of samples per return. */
	REAL32 rangeOfFirstSample;	/* World range of first sample. */
	REAL32 rangeOfLastSample;	/* World range of last sample. */

    } Stream_t;

    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxNetworkReceive(SPxRIB *buffer, unsigned int projCode=0);
    virtual ~SPxNetworkReceive();
    virtual SPxErrorCode Create(const char *addr=NULL, int port=0, 
				const char *ifAddr=NULL);

    /* Address control.  Note that SetPort() is no longer virtual because
     * when it was I had problems with the windows server not linking
     * correctly because one of the windows headers had defined SetPort
     * to SetPortA and therefore it all broke for derived classes that
     * didn't override this function (i.e. all of them).
     */
    virtual SPxErrorCode SetAddress(const char *addr, int port=0,
				    const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 addr, int port=0,
				    const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual SPxErrorCode SetPort(int port) { return SetPortNumber(port); }
    /* The SetPort function calls the SetPortNumber because the winspool
     * header defines SetPort to be either SetPortW or SetPortA which causes
     * issues when linking to a function called SetPort. This inline function
     * does not suffer from this problem.
     */
    SPxErrorCode SetPortNumber(int port);
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

    /* Socket RCVBUF control.  A value of 0 means to use the SPx default. */
    SPxErrorCode SetRcvBufSize(unsigned int numBytes);
    unsigned int GetRcvBufSize(void)		{ return(m_rcvBufSize); }

    /* Pause control (different to enable/disable). */
    SPxErrorCode SetPaused(int paused);
    int GetPaused(void) const			{ return(m_isPaused); }

    /* Parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Status retrieval. */
    unsigned int GetNumBadPackets(void)		{ return(m_badPackets); }
    unsigned int GetNumLostPackets(void)	{ return(m_lostPackets); }
    unsigned int GetNumLostReturns(void)	{ return(m_lostReturns); }
    unsigned int GetNumProblems(void)
    {
	return( m_badPackets + m_lostPackets + m_lostReturns );
    }

    /* Get time of last data packet received. */
    virtual UINT32 GetTimeOfLastDataMsecs(void) { return m_timeOfLastData; }

    /* Do we drop membership of multicast groups when disabled? */
    void SetDropMcastGroup(int drop)	{ m_dropMcastGroup = drop; }
    int GetDropMcastGroup(void)		{ return(m_dropMcastGroup); }

    /* Dropout reporting control. */
    void SetDropoutThreshold(int threshold) { m_dropoutThreshold = threshold; }
    int GetDropoutThreshold(void)	{ return(m_dropoutThreshold); }

    /* Install callback for direct access to data before RIB (may not be
     * supported by all derived classes).
     */
    SPxErrorCode InstallDataFn(SPxNetRecvDataFn_t fn, void *arg)
    {
	m_changePending = TRUE;
	m_mutex.Enter();
	m_fnDataArg = arg;
	m_fnData = fn;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }

    /* Base class status functions. */
    virtual SPxErrorCode GetInitialised(void)
    {
	/* Initialised okay if we have successfully bound or connected. */
	return( static_cast<int>(m_isBoundOrConnected) ? SPX_NO_ERROR : SPX_ERR_UNKNOWN );
    }

    /* Main thread function. */
    virtual void *ThreadFn(SPxThread *thread);

    /* Functions invoked by thread. */
    virtual int ThreadStartFn(void)		{return(SPX_NO_ERROR);}
    virtual int ThreadStateChangedFn(void)	{return(SPX_NO_ERROR);}
    virtual int ThreadSocketChangedFn(void)	{return(SPX_NO_ERROR);}
    virtual int ThreadReadDataFn(unsigned char *buf,
				unsigned int bufSizeBytes,
				unsigned int timeoutMsecs);
    virtual int ThreadHandleDataFn(unsigned char *data,
				unsigned int dataSizeBytes);
    virtual int ThreadDecodeDataFn(unsigned char *data,
				unsigned int dataSizeBytes);
    virtual int ThreadDecodeReturnFn(SPxReturnHeader_tag *hdr,
				     unsigned char *data=NULL);
    virtual int ThreadDecodeReturnFn(Stream_t *stream,
				     SPxReturnHeader_tag *hdr,
				     unsigned char *data=NULL);

    virtual int ThreadNoDataFn(void)		{return(SPX_NO_ERROR);}
    virtual int ThreadBackgroundFn(void)	{return(SPX_NO_ERROR);}
    virtual int ThreadEndFn(void)		{return(SPX_NO_ERROR);}

    /* Function to allow data to be sent out on socket if in TCP mode.
     * Returns the number of bytes actually sent (like send() does).
     */
    virtual int SendData(const void *buf, unsigned int numBytes);

    /* Static public functions for log file support. */
    static void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    static FILE *GetLogFile(void)		{ return(m_logFile); }
    static void SetDebug(UINT32 debug)
    {
	m_debug = debug;
	if( (debug != 0) && (GetLogFile() == NULL) )
	{
	    SetLogFile(stdout);
	}
    }
    static UINT32 GetDebug(void) { return m_debug; }

protected:
    /*
     * Protected fields.
     */
    const unsigned int m_derivedClassProjCode;	/* Only set by some classes */

    /* Radar input buffer where data is written. */
    SPxRIB *m_rib;
    int m_ribFull;		/* Do we think RIB filled up? */
    int m_ribCount;		/* Incrementing count for written returns */

    /* Callback functions. */
    SPxNetRecvDataFn_t m_fnData;
    void *m_fnDataArg;

    /* Worker thread for receiving data. */
    SPxThread *m_thread;

    /* Flag to tell worker thread that someone is trying to change a setting */
    mutable SPxAtomic<int> m_changePending;	/* TRUE if thread should back off a little */

    /* Critical section protection. */
    mutable SPxCriticalSection m_mutex;

    /* Socket information (fd, address structure etc.) */
    int m_createSocket;		/* Does derived class want a socket created? */
    SOCKET_TYPE m_socket;
    struct sockaddr_in m_socketAddress;
    int m_socketIsTCP;		/* TRUE for TCP, FALSE for UDP */
    int m_isTCPNonBlocking;     /* TRUE for non-blocking TCP R/W. */
    int m_isTCPKeepAliveEnabled;/* TRUE to enable keep-alive on TCP socket. */
    SPxAtomic<int> m_isBoundOrConnected;	/* TRUE/FALSE flag */
    int m_isJoined;		/* TRUE/FALSE status */
    int m_dropMcastGroup;	/* TRUE/FALSE option */
    char *m_addressString;	/* Possibly including :port */
    char *m_interfaceAddrString; /* Interface address for multicast, or NULL */
    struct sockaddr_in m_interfaceAddr;	/* Used if AddrString not NULL */
    int m_port;			/* May be overridden by addressString */
    int m_defaultPort;		/* If not specified by user or in string */
    struct sockaddr_in m_fromAddr;	/* Sender address */
    SOCKET_TYPE m_returnSocket;	/* For returning info back to sender */
    unsigned int m_rcvBufSize;	/* Desired size of socket rcvbuf (0=default)*/
    UINT32 m_lastCheckSocketErrTime; /* Last time socker error was reported. */

    /* Timeout for waiting for data. */
    unsigned int m_timeoutMsecs;

    /* Fields for watchdog on received data */
    UINT32 m_timeOfLastData;	/* Ticker when last data received */
    UINT32 m_dataWatchdogMsecs;	/* 0 for no watchdog, else millisecs */

    /* Information on received data. */
    int m_nominalLength;		/* Number of samples per return */
    REAL32 m_rangeOfFirstSample;	/* World range of first sample */
    REAL32 m_rangeOfLastSample;		/* World range of last sample */

    /* Stats etc. */
    UINT16 m_lastSequenceNumber;
    unsigned int m_badPackets;	/* Number of wrong/invalid packets */
    unsigned int m_lostPackets;	/* Number of network packets we've lost? */
    unsigned int m_lostReturns;	/* Number of returns we've discarded/lost */
    UINT32 m_lastWarningTime;	/* Time of last reported warning */
    UINT32 m_lastLicenseTime;	/* Time of last server/client license msg */
    int m_dropoutThreshold;	/* For reporting to sender */

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;	/* Destination for debug messages */
    static UINT32 m_debug;	/* Debug flags */

    /*
     * Protected functions.
     */
    /* Function that may be overridden by testing classes to avoid
     * unwanted PIM clears due to range changes detected for first return received.
     */
    virtual void CheckForChanges(Stream_t *stream, const SPxReturnHeader_tag *hdr);

    /* Internal version of SendData() if mutex is already locked. */
    virtual int sendDataLocked(const void *buf, unsigned int numBytes);

    /* Convenience function for derived classes to cause TCP disconnection. */
    void closeConnection(void);

private:
    /*
     * Private fields.
     */
    /* Flag to say paused or not. */
    int m_isPaused;				/* TRUE or FALSE */

    /* Buffer for reassembly. */
    unsigned char *m_reassemblyBuffer;		/* Buffer for reassembly */
    unsigned int m_reassemblyBufferSize;	/* Size of buffer */
    unsigned int m_reassemblySequence;		/* Seq num being assembled */
    unsigned int m_reassemblyNumChunksSoFar;	/* Num got so far */
    unsigned int m_reassemblyNumChunksTotal;	/* Total expected */
    unsigned int m_reassemblyPayloadSize;	/* Size of total payload */

    /* ZLIB decompression. */
    SPxCompress *m_compress;

    /* Buffer for decoding. */
    unsigned char *m_decodeBuffer;
    unsigned int m_decodeBufferSize;

    /* BUffer for unpacking. */
    unsigned char *m_unpackBuffer;
    unsigned int m_unpackBufferSize;

    /* Errors. */
    UINT32 m_lastBadMsgTime;			/* Last time a bad message was received. */
    UINT32 m_badMsgErrIntervalMsecs;		/* Interval between errors. */

    /*
     * Private functions.
     */
    SPxErrorCode updateAddress(void);
    SPxErrorCode checkReturnSocket(void);
    SPxErrorCode joinOrLeaveMcast(int join);

}; /* SPxNetworkReceive class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECV_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
