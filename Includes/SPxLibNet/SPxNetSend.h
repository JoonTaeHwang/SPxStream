/*********************************************************************
*
* (c) Copyright 2007 - 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for base class of network sending objects.
*
*
* Revision Control:
*   06/09/22 v1.54   AGC	Suppress spurious cppcheck ODR warning.
*
* Previous Changes:
*   21/06/22 1.53   AGC	Add silent option to GetSenderPort().
*   07/02/20 1.52   SP 	Add GetSenderPort().
*   12/11/19 1.51   AGC	Fix data race on debug flags.
*   05/11/19 1.50   REW	Support access to UDP client license counts.
*   19/08/19 1.50   SP 	Support Set/GetActive().
*   22/07/19 1.49   REW	Support per-scan bandwidth stats.
*   24/04/19 1.48   SP 	Support converting samples to 1, 2 and 4-bits 
*			before calling EncodeReturn().
*   16/07/18 1.47   AGC	Remove GetExtraSender/Name().
*   23/05/18 1.46   AGC	Add virtual SendPacketWithBlockingCheck().
*   23/10/17 1.45   AGC	Add GetExtraSender/Name().
*   18/10/17 1.44   AGC	Add IsEncodeFormatSupported().
*   07/07/17 1.43   AGC	Add overloadable SendPacket() for testing.
*   31/05/17 1.42   AJH	Add m_sourceInfo.
*   12/04/17 1.41   AGC	Make mutex mutable.
*   21/02/17 1.40   AGC	Lock mutex while installing data function.
*   30/11/16 1.39   AGC	Rename SetPort() for more robust SetPortA fix.
*   23/09/16 1.38   AGC	Improve mutex behaviour.
*   10/12/15 1.37   AGC	Add virtual GetProjectCode().
*   15/07/15 1.36   AGC	Support TTL parameter.
*   27/08/14 1.35   SP 	Support stripping of pre-trigger video.
*   05/03/14 1.34   SP 	Allow bandwidth to be retrieved remotely.
*   22/11/13 1.33   AGC	Make get functions const.
*   15/10/13 1.32   AGC	Fix warnings from SPxLibAVMfc.
*   04/10/13 1.31   AGC	Simplify headers.
*   25/04/13 1.30   AGC	Use new SPxCompress class.
*   13/02/13 1.29   REW	Support control over sndbuf size.
*   12/02/13 1.28   SP 	Add SetMTU() and GetMTU().
*			Make m_mtu protected (same as last change).
*   07/02/13 1.27   REW	Keep PIM info for derived classes to use.
*			Move m_mtu to protected for derived classes.
*   29/01/13 1.26   REW	Optionally report client dropouts.
*   25/01/13 1.25   REW	Support client dropout reports.
*   24/10/12 1.24   REW	Support server-based client licensing.
*   31/08/12 1.23   AGC	Derive from SPxNetAddr interface.
*   21/08/12 1.22   AGC	Add GetIfAddress()/GetIfAddressString() functions.
*   14/11/11 1.21   AGC	Add GetAddressString() function.
*   02/11/11 1.20   SP 	Add ReportStats().
*                       Make bandwidth stats protected.
*                       Add protected m_forceDataBigEndian flag.
*   25/10/11 1.19   AGC	Add ifAddr arguments to SetAddress().
*   30/08/11 1.18   SP 	Add checkAllocReturnBuf().
*   19/08/11 1.17   SP 	Add data callback for testing.
*   27/06/11 1.16   SP 	Add Set/GetSendRepeatEnable().
*   23/08/10 1.15   REW	Add SetTTL() etc.
*   16/12/09 1.14   REW	Add SPX_NET_SEND_DEBUG_BLOCK.
*   23/11/09 1.13   REW	Add optional ifAddr to Create().
*   03/11/09 1.12   REW	Throttle errors about clients keeping up.
*   29/10/09 1.11   REW	Add TCPClientDisconnected().
*   28/10/09 1.10   REW	Support multiple clients for TCP streams.
*   19/01/09 1.9    REW	Support derived classes using TCP streams.
*   14/11/08 1.8    REW	Support range resolution reduction.
*			Add GetParameter() support.
*   14/07/08 1.7    REW	Add SetPimIdx() and GetPimIdx().
*   27/06/08 1.6    REW	Add GetAddress() and GetPort().
*   07/08/07 1.5    REW	Add InstallBandwidthFunc().
*   03/08/07 1.4    REW	Add m_enabled, SetEnable() etc.
*   30/07/07 1.3    REW	Make debug fields protected, not private.
*			Add m_defaultPort.
*   26/07/07 1.2    REW	Make network protocol match documented API.
*   25/07/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETSEND_H
#define _SPX_NETSEND_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"

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
#define	SPX_NET_SEND_DEBUG_SOCKET	0x00000001	/* Socket control */
#define	SPX_NET_SEND_DEBUG_STATE	0x00000002	/* State change */
#define	SPX_NET_SEND_DEBUG_PARAMS	0x00000004	/* Params */
#define	SPX_NET_SEND_DEBUG_DATA		0x00000010	/* Data sending */
#define	SPX_NET_SEND_DEBUG_ENCODE	0x00000020	/* Encoding */
#define	SPX_NET_SEND_DEBUG_SEND		0x00000040	/* Network */
#define	SPX_NET_SEND_DEBUG_BANDWIDTH	0x00000080	/* Bandwidth */
#define	SPX_NET_SEND_DEBUG_BLOCK	0x00000100	/* Blocking */
#define	SPX_NET_SEND_DEBUG_LICENSE	0x00000200	/* License */
#define	SPX_NET_SEND_DEBUG_DROPOUT	0x00000400	/* Client dropout */
#define	SPX_NET_SEND_DEBUG_VERBOSE	0x80000000	/* Verbose */


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
class SPxCompress;
class SPxRunProcess;
struct SPxReturn_tag;
struct SPxReturnHeader_tag;
struct SPxNetChunkHeader_tag;
class SPxPIM;
class SPxThread;

/* Define the structure used for maintaining TCP client connections. */
// Suppression required due to differing definitions on Win/Linux.
// cppcheck-suppress ctuOneDefinitionRuleViolation
typedef struct SPxNetworkSendClient_tag {
    /* Control info. */
    unsigned int idx;		/* Index in array */
    unsigned int counter;	/* Client number 1, 2, etc. */
    UINT32 lastErrorMsgTime;	/* Last time error reported */

    /* Client socket. */
    SOCKET_TYPE sock;		/* Handle, or SPX_SOCK_INVALID */
    struct sockaddr_in addr;	/* Address of client */

    /* Output buffers etc. */
    unsigned char *sendBuf;		/* Buffer for sending data */
    unsigned int sendBufSize;		/* Size of buffer in bytes */
    unsigned int sendBufBytesPresent;	/* How many bytes are pending */

    /* Input buffers etc. */
    unsigned char *recvBuf;		/* Buffer for receiving data */
    unsigned int recvBufSize;		/* Size of buffer in bytes */
    unsigned int recvBufBytesPresent;	/* How many bytes are present */

} SPxNetworkSendClient_t;

/* Similar for UDP clients.  */
typedef struct SPxNetworkSendClientUDP_tag {
    struct sockaddr_in addr;		/* Address of client */
    UINT32 lastMsgTime;			/* Last time msg received */
} SPxNetworkSendClientUDP_t;


/*
 * Define our network sender base class.
 */
class SPxNetworkSend :public SPxObj, public SPxNetAddr
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxNetworkSend();
    virtual ~SPxNetworkSend();
    virtual SPxErrorCode Create(const char *addr=NULL,int port=0,const char *ifAddr=NULL);

    /* State control. */
    void SetEnable(int enable) { SetActive(enable); }
    int GetEnable(void)		{ return(GetActive()); }

    /* Address control. */
    virtual SPxErrorCode SetAddress(const char *addr, int port=0, const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 addr, int port=0, const char *ifAddr=NULL);
    /* The SetPort function calls the SetPortNumber because the winspool
     * header defines SetPort to be either SetPortW or SetPortA which causes
     * issues when linking to a function called SetPort. This inline function
     * does not suffer from this problem.
     *
     * Derived clases may override SetPort() or SetPortNumber(), but the same
     * isses with the winspool header may occue if overriding SetPort().
     */
    virtual SPxErrorCode SetPort(int port) { return SetPortNumber(port); }
    virtual SPxErrorCode SetPortNumber(int port);
    virtual SPxErrorCode SetTTL(int ttl);	/* 0 to 255, or -1 not set */
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddr(void) const { return(m_interfaceAddrString); }
    virtual const char *GetIfAddressString(void) const { return GetIfAddr(); }
    virtual int GetTTL(void)		{ return(m_ttl); }
    virtual int GetSenderPort(int silent=FALSE) const;

    /* Type of network sender. */
    virtual UINT32 GetProjectCode(void) const { return 110; }

    /* Source of data. */
    virtual void SetPimIdx(int idx)	{ m_pimIdx = idx; }
    virtual int GetPimIdx(void)		{ return(m_pimIdx); }

    /* Source info for chunk header. */
    virtual void SetSourceInfo(unsigned int sourceInfo) { m_sourceInfo = sourceInfo; }
    virtual unsigned int GetSourceInfo(void) { return( m_sourceInfo ); }

    /* Resolution reduction. */
    SPxErrorCode SetRangeReductionFactor(unsigned int factor);
    unsigned int GetRangeReductionFactor(void)
    {
	return(m_rangeReductionFactor);
    }

    /* Misc control */
    SPxErrorCode SetSendRepeatEnable(int isEnabled);
    int GetSendRepeatEnable(void) { return m_sendRepeatEnable; }
    SPxErrorCode SetMTU(unsigned int sizeBytes);
    unsigned int GetMTU(void) { return m_mtu; }

    /* Socket SNDBUF control.  A value of 0 means to use the SPx default. */
    SPxErrorCode SetSndBufSize(unsigned int numBytes);
    unsigned int GetSndBufSize(void)	{ return(m_sndBufSize); }

    /* Parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Input of data. */
    int NewData(SPxRunProcess *runProcess, SPxReturn_tag *firstReturn,
			unsigned int firstAziIdx, unsigned int numAzis);

    /* Encoding control (where available). */
    virtual void SetEncodeFormat(int format);
    int GetEncodeFormat(void);
    virtual int IsEncodeFormatSupported(int format) const;

    int InstallBandwidthFunc(void (*fn)(void *userArg,
					SPxNetworkSend *obj,
					double encBytesPerSec,
					double ratio),
				void *userArg=NULL,
				unsigned int reportPeriodSecs=5)
    {
	m_mutex.Enter();
	m_bwReportFunc = fn;
	m_bwReportArg = userArg;
	m_bwReportSecs = reportPeriodSecs;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }
    unsigned int GetLastScanTotalEncodedBytes(void) const
    {
	return(m_bwLastScanTotal);
    }

    /* Access functions for TCP streams. */
    unsigned int GetNumTCPClientsConnected(void)
    {
	return(m_tcpNumClientsConnected);
    }
    unsigned int GetNumTCPClientsSupported(void)
    {
	return(m_tcpNumClientsAllocated);
    }
    unsigned int GetNumTCPClientsEver(void)
    {
	return(m_tcpClientCount);
    }

    /* Control over whether client dropout is reported or not. */
    void SetReportClientDropout(int report) { m_reportClientDropout = report;}
    int GetReportClientDropout(void) { return(m_reportClientDropout); }

    /* Install callback for direct access to data before it is sent.
     * This callback is primarily intended to be used for testing 
     * purposes.
     */
    SPxErrorCode InstallDataFn(void (*fn)(SPxNetworkSend *, 
                                          void *,
                                          SPxReturnHeader_tag *,
                                          unsigned char *),
                               void *arg)
    {
	m_mutex.Enter();
	m_fnDataArg = arg;
	m_fnData = fn;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }

    /* Internal worker-thread function.  Not for direct use by users. */
    void *ThreadFnTCP(SPxThread *thread);

    /* Static public functions for log file support. */
    static void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    static FILE *GetLogFile(void)		{ return(m_logFile); }
    static void SetDebug(UINT32 debug)		{ m_debug = debug; }

protected:
    /*
     * Protected functions (likely to be overridden by derived classes).
     */
    /* Encoder function. */
    virtual unsigned char *EncodeReturn(const SPxReturn_tag *rtn,
					unsigned char *destBuf,
					unsigned int destBufSizeBytes,
					unsigned int *encodedSizeBytesPtr,
					unsigned int *formatUsedPtr);

    /* Data sending functions. */
    virtual int SendReturn(const SPxReturnHeader_tag *rtnHeader,
			   unsigned char *data,
			   unsigned int numBytes,
			   unsigned int formatUsed);
    virtual int SendPacketWithBlockingCheck(const unsigned char *data,
					    unsigned int numBytes);
    virtual int SendPacket(const unsigned char *data,
			   unsigned int numBytes);

    /* Function to report stats, bandwidth, etc. */
    virtual void ReportStats(const SPxReturnHeader_tag *rtnHeader);

    /* Functions to support TCP streams for derived classes. */
    virtual void TCPClientConnected(SPxNetworkSendClient_t * /*client*/) {return;}
    virtual void TCPClientDisconnected(SPxNetworkSendClient_t * /*cl*/) {return;}
    virtual void TCPLoopFunction(void);
    virtual void TCPDataReceived(SPxNetworkSendClient_t *client);
    virtual SPxErrorCode TCPQueueToAllClients(unsigned char *data,
						unsigned int numBytes);
    virtual SPxErrorCode TCPQueueToClient(SPxNetworkSendClient_t *client,
						unsigned char *data,
						unsigned int numBytes);
    virtual SPxErrorCode TCPFlushToAllClients(void);
    virtual SPxErrorCode TCPFlushToClient(SPxNetworkSendClient_t *client);
    virtual SPxNetworkSendClient_t *TCPGetClientInfo(unsigned int idx);


    /*
     * Protected fields.
     */

     /* Critical section protection. */
    mutable SPxCriticalSection m_mutex;

    /* Socket information (fd, address structure etc.) */
    SOCKET_TYPE m_socket;
    struct sockaddr_in m_socketAddress;
    int m_socketIsTCP;		/* TRUE=TCP, FALSE=UDP, before Create() */
    char *m_addressString;	/* Possibly including :port */
    char *m_interfaceAddrString; /* Interface address for multicast, or NULL */
    struct sockaddr_in m_interfaceAddr; /* Used if AddrString not NULL */
    int m_port;			/* May be overridden by addressString */
    int m_defaultPort;		/* If not specified by user or in string */
    int m_ttl;			/* TTL, 0 to 255 (or -1 for not set) */
    unsigned int m_sndBufSize;	/* Desired size of socket sndbuf (0=default) */
    int m_isDataSent;		/* Has data been sent? */

    /* Maximum number of bytes to send on network in each packet. */
    unsigned int m_mtu;			/* Maximum transmission unit */

    /* Fields for use with TCP streams (see private ones too). */
    SPxThread *m_thread;		/* Thread for handling connections */
    unsigned int m_tcpMaxClients;	/* Max number of clients allowed */
    unsigned int m_tcpBackoffMsecs;	/* How long to back off for */
    unsigned int m_tcpDefaultSendBufSize; /* Size to allocate */
    unsigned int m_tcpDefaultRecvBufSize; /* Size to allocate */

    /* Encoding information. */
    unsigned int m_encodeFormat;	/* One of SPX_RIB_PACKING_.. */
    unsigned int m_resolutionFormat;	/* One of SPX_RIB_PACKING_.. */

    /* Index of PIM to send data from if run-process has more than 1. */
    SPxAtomic<int> m_pimIdx;		/* 0 for A, 1 for B, 2 for C */
    SPxPIM *m_pim;			/* Pointer to PIM for this spoke */
    int m_pimAzi;			/* Azimuth in PIM numbering scheme */

    /* Source info for chunk header. */
    unsigned int m_sourceInfo;

    /* Callback functions. */
    void (*m_fnData)(SPxNetworkSend *, void *,
                     SPxReturnHeader_tag *, unsigned char *);
    void *m_fnDataArg;

    /* Bandwidth estimation. */
    unsigned int m_bwTotalRaw;		/* Total raw bytes */
    unsigned int m_bwTotalEncoded;	/* Total encoded bytes */
    unsigned int m_bwTotalHeaders;	/* Total headers sent */
    unsigned int m_bwLastScanTotal;	/* Total last scan encoded+headers */
    unsigned int m_bwBytesPerSec;       /* Last calculated data rate. */
    double m_bwCompRatio;               /* Last calculated compression ratio.*/
    UINT16 m_bwLastAzi;			/* Last azimuth considered */
    
    /* ZLIB compression. */
    SPxCompress *m_compress;

    /* Flags. */
    int m_forceDataBigEndian; /* Send multi-byte data in big-endian order */ 
    int m_sendPreTriggerVideoEnabled; /* Send pre-trigger video? */

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;	/* Destination for debug messages */
    static SPxAtomic<UINT32> m_debug;	/* Debug flags */

private:
    /*
     * Private fields.
     */
    /* The m_buf is allocated memory, with m_buf2 being a second buffer for
     * use when trying different encoding methods.
     * With m_buf, there are several pointers to the start of the chunk
     * header, return header and data itself.  These are pointers into m_buf
     * and should be updated whenever m_buf is updated, but never freed.
     */
    unsigned char *m_buf;		/* Buffer for packing and sending */
    unsigned char *m_buf2;		/* Second (temporary) buffer */
    unsigned int m_bufSizeBytes;	/* Total size of buffer */
    unsigned char *m_bufChunkStart;	/* Start of chunk header in buf */
    unsigned int m_bufChunkSizeBytes;	/* Size of chunk header section */
    SPxReturnHeader_tag *m_bufHeaderStart;	/* Start of return header in buf */
    unsigned int m_bufHeaderSizeBytes;	/* Size of return header section */
    unsigned char *m_bufDataStart;	/* Start of encoded data in buffer */
    unsigned int m_bufDataSizeBytes;	/* Size of encoded data section */

    /* Sequence number. */
    unsigned int m_sequenceCount;	/* Sequence number for sent msgs */
    int m_reportClientDropout;		/* Flag to say report dropout */

    /* Bandwidth reporting. */
    UINT32 m_bwLastTimeMsecs;		/* Last time reported */
    void (*m_bwReportFunc)(void *userArg, SPxNetworkSend *obj,
				double encBytesPerSec,
				double ratio);
    void *m_bwReportArg;		/* User arg for m_bwReportFunc */
    unsigned int m_bwReportSecs;	/* Period between report funcs */

    /* Resolution reduction. */
    unsigned int m_rangeReductionFactor;	/* Resolution reduction */
    SPxReturn_tag *m_returnBuf;                 /* Return buffer */
    unsigned int m_returnBufSizeBytes;		/* Total size of buffer */
    unsigned int m_returnBufDataSizeBytes;	/* Size of encoded data section */

    /* Fields for use with TCP streams (see protected ones too). */
    unsigned int m_tcpClientCount;	/* How many ever? */
    unsigned int m_tcpNumClientsAllocated; /* Number of clients allowed for */
    unsigned int m_tcpNumClientsConnected; /* Number of current TCP clients */
    SPxNetworkSendClient_t *m_tcpClients; /* Array of client structures */

    /* Fields for use with UDP streams. */
    unsigned int m_udpNumClientsAllocated; /* Number of clients allowed for */
    SPxNetworkSendClientUDP_t *m_udpClients; /* Array of client structures */
    unsigned int m_udpNumClients;	/* Number of current clients */
    unsigned int m_udpMaxClients;	/* Max number of permitted clients */
    UINT32 m_udpLastCheckTime;		/* Time of last check */
    UINT32 m_udpLastErrorTime;		/* Time of last error message */

    /* Flags */
    int m_sendRepeatEnable; /* Control sending of repeated azimuths */

    /*
     * Private functions.
     */
    int setupSocketAddress(void);	/* Initialise/update address */
    int checkAllocReturnBuf(void);      /* Check/create return buffer */
    int reduceRangeResolution(const SPxReturn_tag *rtn);    /* Range reduction */
    int stripPreTriggerVideo(const SPxReturn_tag *rtn);	    /* Remove neg start range. */
    int changeSampleResolution(const SPxReturn_tag *rtn);   /* Change sample size. */
    SPxErrorCode tryAcceptingClient(void);	/* TCP connections */
    unsigned int tryReadingFromClient(unsigned int idx); /* TCP clients */
    SPxErrorCode readUDPClientReturns(void);	/* UDP clients */
    unsigned int countUDPClients(void);		/* UDP clients */
    SPxErrorCode handlePacketBLicense(UINT32 now, struct sockaddr_in *from,
					void *packet, int numBytes);
    SPxErrorCode handlePacketBDropout(UINT32 now, struct sockaddr_in *from,
					void *packet, int numBytes);
}; /* SPxNetworkSend class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETSEND_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
