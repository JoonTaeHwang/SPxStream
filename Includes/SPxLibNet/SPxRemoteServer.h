/*********************************************************************
*
* (c) Copyright 2008 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRemoteServer class, which allows client
*	applications to connect to, control and receive information
*	from remote servers.
*
*
* Revision Control:
*   08/08/22 v1.50   SP 	Remove AlertNtoh().
*
* Previous Changes:
*   16/06/22 1.49   AJH	Support multiple connection handlers.
*   26/05/22 1.48   REW	Support ChangeHeartbeatAddrPort().
*   31/03/22 1.47   AGC	Support use with SPxNetTCP for testing.
*   17/06/21 1.46   AJH	Support callback for connection/disconnection.
*   05/05/21 1.45   AGC	Handle fragmented commands/replies.
*   14/04/21 1.44   SP 	Add a double overload for Set/GetRemoteParameter().
*   06/04/21 1.43   SP 	Support parameter set with optional reply.
*   02/03/21 1.42   JGB	Added functions GetRemoteObjects, GetRemoteParameters
*			and GetRemoteParameterValues.
*   18/11/20 1.41   AJH	Change m_pendingAddr to string.
*   03/07/20 1.40   AGC	Add mutex already locked option to Connect().
*   23/03/20 1.39   AGC Support setting margins for CreateTrack().
*   09/03/20 1.38   SP 	Support password protection.
*   11/01/19 1.37   AGC	Support receiving SPx plot status messages.
*   23/07/18 1.36   BTB	Add GetLoadedConfig().
*   29/05/18 1.35   SP 	Support auto reconnect from background thread.
*   11/05/18 1.34   AGC	Make HandleReports() virtual for testing.
*   24/04/17 1.33   AGC	Improve connected state for #448.
*   08/03/17 1.32   SP 	Add RemoveAllSockets().
*   09/11/16 1.31   SP 	Add recvWithTimeout().
*   02/09/16 1.30   AGC	Use atomic variable for thread back off.
*   03/11/15 1.29   AGC	Packet decoder callback data now const.
*   26/01/15 1.28   SP 	Add GetPacketDecoderNet().
*   10/04/14 1.27   AGC	Allows test to provide own packet decoder.
*   24/02/14 1.26   SP 	Add Set/GetCommandTimeout().
*			Add Set/GetConnectionTimeout().
*			Add Set/IsKeepAliveEnabled().
*			Add SetReportConnectionErrors().
*			Support non-blocking connection.
*			Rename IsObjectAvailable() as GetObjectAvailable().
*   17/02/14 1.25   SP 	Add IsObjectAvailable().
*   15/11/13 1.24   SP 	Add functions to get binary data and files.
*   27/09/13 1.23   REW	Add GetMsecsSinceLastTrackMsg().
*   03/05/13 1.22   AGC	Add TrackPackNet/TrackUnpackNet() functions.
*   04/03/13 1.21   AGC	Add RemoveSocket functions.
*   21/12/12 1.20   AGC	Support handling alert messages.
*   26/10/12 1.19   AGC	Add TrackNtoh/TrackHton() functions.
*   18/10/12 1.18   AGC	Add segment and delete poly functions.
*   01/10/12 1.17   AGC	Add TrackerModeChange function.
*   11/09/12 1.16   AGC	Add more tracker control functions.
*   07/09/12 1.15   AGC	Add server and tracker control functions.
*			Use const for all string params.
*			Add V2 message handler.
*   31/08/12 1.14   AGC	Check for tracker object on connect.
*   13/09/11 1.13   AGC	Add ADDRESSSES debugging flag.
*   06/04/11 1.12   AGC	Use new SPxPacketDecoderNet class.
*   15/09/10 1.11   REW	Make destructor virtual.
*   27/05/10 1.10   SP 	Use const for object and param names.
*   22/02/10 1.9    REW	AddSocket() takes optional ifAddr.
*   23/07/09 1.8    REW	Add IsConnected() and IsConnectedToAddrPort().
*   25/06/09 1.7    REW	Handle plot reports.
*   09/02/09 1.6    REW	Handle tracker status reports.
*   13/11/08 1.5    REW	Support non-integer remote parameters.
*   26/09/08 1.4    REW	Use typedef for msg fn ptr.
*   17/09/08 1.3    REW	Use typedefs for fn ptrs. Add timestamp.
*   16/09/08 1.2    REW	Support heartbeats, track reports and msgs.
*   02/09/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_REMOTE_SERVER_H
#define _SPX_REMOTE_SERVER_H

/*
 * Other headers required.
 */

/* We need SPxCommon for common types etc. */
#include "SPxLibUtils/SPxCommon.h"

/* We need SPxAtomic class. */
#include "SPxLibUtils/SPxAtomic.h"

/* We need the SPxCriticalSection class. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Debug flags.
 */
#define	SPX_REMOTE_SVR_DEBUG_CONNECTS	0x00000001	/* Connections */
#define	SPX_REMOTE_SVR_DEBUG_SOCKET	0x00000002	/* Socket control */
#define	SPX_REMOTE_SVR_DEBUG_HANDSHAKES	0x00000004	/* Handshakes */
#define SPX_REMOTE_SVR_DEBUG_ADDRESSES  0x00000008	/* Address logic */
#define	SPX_REMOTE_SVR_DEBUG_COMMANDS	0x00000010	/* Commands */
#define	SPX_REMOTE_SVR_DEBUG_MESSAGES	0x00000040	/* Messages */
#define	SPX_REMOTE_SVR_DEBUG_VERBOSE	0x80000000	/* Verbose */


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

/* Forward declare classes we need in case of wrong order headers. */
class SPxPacketDecoder;
class SPxPacketDecoderNet;
class SPxRemoteServer;
class SPxThread;
class SPxNetTCP;
struct SPxTime_tag;
struct SPxScNetHeartbeatStruct_tag;
struct SPxPacketPlot_tag;
struct SPxPacketTrackMinimal_tag;
struct SPxPacketTrackNormal_tag;
struct SPxPacketTrackExtended_tag;
struct SPxPacketTrackerStatus_tag;
struct SPxPacketAlert_tag;

/* Define the types of some of the callback functions. */
typedef void (*SPxRemoteServerMsgFn_t)(SPxRemoteServer *,	/* this */
					void *,			/* userArg */
					struct sockaddr_in *,	/* from/NULL */
					char *,			/* msg */
					int,			/* msgSize */
					char *,			/* content */
					UINT32);		/* val */
typedef void (*SPxRemoteServerMsgFnV2_t)(SPxRemoteServer *,	/* this */
					void *,			/* userArg */
					struct sockaddr_in *,	/* from/NULL */
					const char *,		/* msg */
					int,			/* msgSize */
					const char *,		/* content */
					UINT32);		/* val */
typedef void (*SPxRemoteServerHbFn_t)(SPxRemoteServer *,	/* this */
					void *,			/* userArg */
					struct sockaddr_in *,	/* from/NULL */
					SPxScNetHeartbeatStruct_tag *);
typedef void (*SPxRemoteServerPlotFn_t)(SPxRemoteServer *,	/* this */
					void *,			/* userArg */
					struct sockaddr_in *,	/* from/NULL */
					SPxTime_tag *,		/* timestamp */
					struct SPxPacketPlot_tag *);
typedef void (*SPxRemoteServerPlotStatusFn_t)(SPxRemoteServer *,	/* this */
					void *,			/* userArg */
					struct sockaddr_in *,	/* from/NULL */
					SPxTime_tag *,		/* timestamp */
					struct SPxPacketPlotStatus_tag *);
typedef void (*SPxRemoteServerTrackFn_t)(SPxRemoteServer *,	/* this */
					void *,			/* userArg */
					struct sockaddr_in *,	/* from/NULL */
					SPxTime_tag *,		/* timestamp */
					struct SPxPacketTrackMinimal_tag *,
					struct SPxPacketTrackNormal_tag *,
					struct SPxPacketTrackExtended_tag *);
typedef void (*SPxRemoteServerTrackerStatusFn_t)(SPxRemoteServer *,/* this */
					void *,			/* userArg */
					struct sockaddr_in *,	/* from/NULL */
					SPxTime_tag *,		/* timestamp */
					struct SPxPacketTrackerStatus_tag *);
typedef void (*SPxRemoteServerAlertFn_t)(SPxRemoteServer *,	/* this */
					 void *,		/* userArg */
					 struct sockaddr_in *,	/* from/NULL */
					 SPxTime_tag *,		/* timestamp */
					 UINT32,		/* Type of message. */
					 SPxPacketAlert_tag *);	/* Common alert message. */
typedef void (*SPxRemoteServerConnectFn_t)(SPxRemoteServer *,	/* this */
					 void *,		/* userArg */
					 int connected);	/* Connected or disconnected. */
					 

/*
 * Define our class.
 */
class SPxRemoteServer :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructors, destructor etc. */
    SPxRemoteServer(const char *hbeatAddr=NULL, int hbeatPort=0);
    SPxRemoteServer(UINT32 hbeatAddr, int hbeatPort=0);
    virtual ~SPxRemoteServer();

    /* Connection/disconnection. */
    SPxErrorCode SetPassword(const char *password);
    SPxErrorCode Connect(const char *addr=NULL, int port=0,
			 int autoReconnect=FALSE, int mutexAlreadyLocked=FALSE);
    SPxErrorCode Connect(UINT32 addr, int port=0,
			 int autoReconnect=FALSE, int mutexAlreadyLocked = FALSE);
    SPxErrorCode Disconnect(int mutexAlreadyLocked=FALSE);
    int IsConnected(void) const { return(m_isConnected); }
    int IsConnectedToAddrPort(UINT32 addr, int port) const;
    SPxErrorCode SetConnectionTimeoutMsecs(unsigned int msecs);
    unsigned int GetConnectionTimeoutMsecs(void) const { return m_connTimeoutMsecs; }
    SPxErrorCode SetKeepAliveEnabled(int isEnabled);
    int IsKeepAliveEnabled(void) const { return m_isKeepAliveEnabled; }
    SPxErrorCode SetReportConnectionErrors(int isEnabled);
    SPxErrorCode CheckRemoteSet(void);

    /* Control over extra sockets to listen to. */
    SPxErrorCode AddSocket(const char *addr, int port, const char *ifAddr=NULL);
    SPxErrorCode AddSocket(UINT32 addr, int port, const char *ifAddr=NULL);
    SPxErrorCode RemoveSocket(const char *addr, int port);
    SPxErrorCode RemoveSocket(UINT32 addr, int port);
    SPxErrorCode RemoveAllSockets(void);

    /* Change the heartbeat address/port (initially set in constructor). */
    SPxErrorCode ChangeHeartbeatAddrPort(const char *hbeatAddr, int hbeatPort);
    SPxErrorCode ChangeHeartbeatAddrPort(UINT32 hbeatAddr, int hbeatPort);

    /* Information retrieval. */
    UINT32 GetAddress(void) const;	/* of remote server */
    int GetPort(void) const;		/* of remote server */
    UINT32 GetHeartbeatAddress(void) const;
    int GetHeartbeatPort(void) const;
    UINT32 GetMsecsSinceLastTrackMsg(void) const;
    SPxErrorCode GetObjectAvailable(const char *objectName);
    SPxErrorCode SetCommandTimeoutMsecs(unsigned int msecs);
    unsigned int GetCommandTimeoutMsecs(void) const { return m_cmdTimeoutMsecs; }
    SPxPacketDecoderNet *GetPacketDecoderNet(void) { return m_packetDecoder; }

    /* Remote parameter assignment. */
    SPxErrorCode SetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    const char *parameterValue,
                                    char *valueBuf=NULL,
                                    int valueBufLen=0);
    SPxErrorCode SetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    int parameterValue,
                                    char *valueBuf=NULL,
                                    int valueBufLen=0);
    SPxErrorCode SetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    unsigned int parameterValue,
                                    char *valueBuf=NULL,
                                    int valueBufLen=0);
    SPxErrorCode SetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    REAL32 parameterValue,
                                    char *valueBuf=NULL,
                                    int valueBufLen=0);
    SPxErrorCode SetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    double parameterValue,
                                    char *valueBuf=NULL,
                                    int valueBufLen=0);

    /* Remote parameter retrieval. */
    SPxErrorCode GetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    int *valuePtr);
    SPxErrorCode GetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    unsigned int *valuePtr);
    SPxErrorCode GetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    REAL32 *valuePtr);
    SPxErrorCode GetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    double *valuePtr);
    SPxErrorCode GetRemoteParameter(const char *objectName, 
                                    const char *parameterName,
                                    char *valueBuf, 
                                    int bufLen);

    /* Remote bulk object & parameter retrieval. */
    SPxErrorCode GetRemoteObjects(char* objBuf, int bufLen);
    SPxErrorCode GetRemoteParameters(const char* objectName, char* paramBuf, int bufLen);
    SPxErrorCode GetRemoteParameterValues(const char* objectName, char* paramBuf, int bufLen);

    /* Remote data retrieval. */
    SPxErrorCode GetRemoteData(const char *objectName,
                               const char *dataName,
                               unsigned char *dataBuf,
                               UINT32 bufLen,
                               UINT32 *packetType,
                               UINT32 *dataLen);

    SPxErrorCode GetRemoteFile(const char *objectName,
                               const char *srcFilename,
                               const char *dstFilename=NULL);

    SPxErrorCode SyncFilesToRemote(const char *objectName,
                                   const char *localDir,
                                   const char *filenameRegex=NULL,
                                   int allowLocalDelete=FALSE);

    /* Server control. */
    SPxErrorCode LoadConfig(const char *filename);
    SPxErrorCode GetLoadedConfig(char *filenameBuf, int bufLen);
    SPxErrorCode SaveConfig(const char *filename);

    /* Tracker control. */
    SPxErrorCode TrackerModeChange(UINT32 flags);
    SPxErrorCode CreateTrack(double rangeMetres, double aziDegs,
                             int optionalModelRef=0,
                             double rangeMargin=0.0, double azimuthMargin=0.0,
                             double rangeSizeInput=0.0, double azimuthSizeInput=0.0);
    SPxErrorCode DeleteTrack(UINT32 trackId);
    SPxErrorCode DeleteAllTracks(void);
    SPxErrorCode RepairTrack(UINT32 trackId, double rangeMetres, double aziDegs);
    SPxErrorCode SwapTrackLabels(UINT32 trackId1, UINT32 trackId2);
    SPxErrorCode CreateTrackParamArea(const char *areaName);
    SPxErrorCode DeleteTrackParamArea(const char *areaName);
    SPxErrorCode RecalculateTrackParamArea(const char *areaName);
    SPxErrorCode CreateTrackParamPoly(const char *areaName, const char *polyName, 
	const SPxPoint *points, unsigned int numPoints);
    SPxErrorCode CreateTrackParamPolyHole(const char *areaName, const char *polyName, 
	const SPxPoint *points, unsigned int numPoints);
    SPxErrorCode DeleteTrackParamPoly(const char *areaName, const char *polyName);
    SPxErrorCode CreateTrackParamSegment(const char *areaName, const char *segName,
	REAL32 startRangeMetres, REAL32 endRangeMetres, 
	REAL32 startAziDegs, REAL32 endAziDegs);
    SPxErrorCode DeleteTrackParamSegment(const char *areaName, const char *segName);
    SPxErrorCode SetTrackDebug(UINT32 debugFlags, int debugAllHyp=TRUE);

    /* Report receipt, either one-shot with timeout or thread-based. */
    virtual SPxErrorCode HandleReports(unsigned int timeoutMsecs);
    SPxErrorCode StartReportHandlerThread(void);
    SPxErrorCode StopReportHandlerThread(void);

    /* Installation of report callbacks. */
    SPxErrorCode InstallHandlerMsgs(SPxRemoteServerMsgFn_t fn,
					void *arg,
					const char *prefix);
    SPxErrorCode InstallHandlerMsgs(SPxRemoteServerMsgFnV2_t fn,
					void *arg,
					const char *prefix);
    SPxErrorCode InstallHandlerHeartbeats(SPxRemoteServerHbFn_t fn,
					void *arg)
    {
	m_threadBackOff = TRUE;
	m_mutex.Enter();
	m_fnHb = fn;
	m_fnHbArg = arg;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }
    SPxErrorCode InstallHandlerPlots(SPxRemoteServerPlotFn_t fn,
					void *arg)
    {
	m_threadBackOff = TRUE;
	m_mutex.Enter();
	m_fnPlots = fn;
	m_fnPlotsArg = arg;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }
    SPxErrorCode InstallHandlerPlotStatus(SPxRemoteServerPlotStatusFn_t fn,
					  void *arg)
    {
	m_threadBackOff = TRUE;
	m_mutex.Enter();
	m_fnPlotStatus = fn;
	m_fnPlotStatusArg = arg;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }
    SPxErrorCode InstallHandlerTracks(SPxRemoteServerTrackFn_t fn,
					void *arg)
    {
	m_threadBackOff = TRUE;
	m_mutex.Enter();
	m_fnTracks = fn;
	m_fnTracksArg = arg;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }
    SPxErrorCode InstallHandlerTrackerStatus(
					SPxRemoteServerTrackerStatusFn_t fn,
					void *arg)
    {
	m_threadBackOff = TRUE;
	m_mutex.Enter();
	m_fnTrackerStatus = fn;
	m_fnTrackerStatusArg = arg;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }
    SPxErrorCode InstallHandlerAlerts(SPxRemoteServerAlertFn_t fn,
					void *arg)
    {
	m_threadBackOff = TRUE;
	m_mutex.Enter();
	m_fnAlert = fn;
	m_fnAlertArg = arg;
	m_mutex.Leave();
	return(SPX_NO_ERROR);
    }
    SPxErrorCode InstallHandlerConnect(SPxRemoteServerConnectFn_t fn,
					void *arg);

    /* Static public functions for log file support. */
    static void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    static FILE *GetLogFile(void)		{ return(m_logFile); }
    static void SetDebug(UINT32 debug)		{ m_debug = debug; }

    /* Thread functions, not to be called by users. */
    void *ThreadFunc(SPxThread *thread);
    void *ConnectionThreadFunc(SPxThread *thread);

    /* Static functions for bytes-swapping track packets. */
    static SPxErrorCode TrackNtoh(SPxPacketTrackMinimal_tag *minRpt,
	SPxPacketTrackNormal_tag *normRpt);
    static SPxErrorCode TrackHton(SPxPacketTrackMinimal_tag *minRpt,
	SPxPacketTrackNormal_tag *normRpt);
    static int TrackPackNet(const SPxPacketTrackMinimal_tag *minRpt,
	const SPxPacketTrackNormal_tag *normRpt,
	const SPxPacketTrackExtended_tag *extRpt,
	unsigned char *buf, unsigned int bufLen);
    static SPxErrorCode TrackUnpackNet(const unsigned char *buf, unsigned int bufLen,
	SPxPacketTrackMinimal_tag *minRpt,
	SPxPacketTrackNormal_tag *normRpt,
	SPxPacketTrackExtended_tag *extRpt);

protected:
    /* Testing constructors. */
    SPxRemoteServer(const char *hbeatAddr, int hbeatPort, SPxPacketDecoderNet *decoder);
    SPxRemoteServer(const char *hbeatAddr, int hbeatPort, SPxNetTCP *tcp);

private:
    /*
     * Private fields.
     */
    /* Critical section protection. */
    SPxCriticalSection m_mutex;

    /* Connection socket. */
    SPxAtomic<int> m_isConnected;
    SOCKET_TYPE m_sockConn;
    struct sockaddr_in m_sockConnAddress;
    mutable UINT32 m_alternateAddr;
    unsigned int m_connTimeoutMsecs;
    unsigned int m_cmdTimeoutMsecs;
    int m_isKeepAliveEnabled;
    int m_reportConnErrs;
    char m_pendingAddr[32];
    int m_pendingPort;
    int m_isDisconnectPending;
    const char *m_password;
    int m_serverSupportsCmd;
    SPxNetTCP *m_cmdTcp;

    /* Thread for handling connection. */
    SPxThread *m_connectionThread;

    /* Asynchronous message socket. */
    SOCKET_TYPE m_sockMsgs;
    struct sockaddr_in m_sockMsgsAddress;

    /* Heartbeat socket. */
    SOCKET_TYPE m_sockHb;
    struct sockaddr_in m_sockHbAddress;

    /* List of user-added socket/addresses. */
    struct SPxRemoteServerSockInfo_tag *m_sockList;

    /* Convenient copy of an invalid socket handle. */
    SOCKET_TYPE m_sockInvalid;

    /* Thread for handling reports etc. */
    SPxThread *m_thread;
    SPxAtomic<int> m_threadBackOff;	/* Flag to say back off */
    UINT32 m_lastTrackMsgMsecs;		/* Timestamp for last message */

    /* Buffer for receiving reports etc. */
    char *m_buf;			/* Buffer */
    int m_bufSizeBytes;			/* Size of buffer in bytes */

    /* Buffer for receiving binary data. */
    unsigned char *m_binBuf;            /* Binary data buffer. */
    int m_binBufSizeBytes;              /* Size of data buffer in bytes. */

    /* Buffer for receiving files. */
    unsigned char *m_fileBuf;           /* File buffer. */
    int m_fileBufSizeBytes;             /* Size of file buffer in bytes. */

    /* Packet decoder used to decode received binary data. */
    SPxPacketDecoder *m_binPacketDecoder;

    /* Object for decoding packets. */
    SPxPacketDecoderNet *m_packetDecoder;

    /* Received binary data. */
    SPxErrorCode m_binErr;      /* Received error code. */
    UINT32 m_binPacketType;     /* Received packet type. */
    UINT32 m_binRecvBytes;      /* Received data size. */

    /* Flags indicating if objects exist. */
    int m_haveServer;	/* The connected server has a server object. */
    int m_haveTracker;	/* The connected server has a tracker object. */

    /* User-installed handler functions. */
    struct SPxRemoteServerMsgFnInfo_tag *m_fnMsgsList;
    SPxRemoteServerHbFn_t m_fnHb;
    void *m_fnHbArg;
    SPxRemoteServerPlotFn_t m_fnPlots;
    void *m_fnPlotsArg;
    SPxRemoteServerPlotStatusFn_t m_fnPlotStatus;
    void *m_fnPlotStatusArg;
    SPxRemoteServerTrackFn_t m_fnTracks;
    void *m_fnTracksArg;
    SPxRemoteServerTrackerStatusFn_t m_fnTrackerStatus;
    void *m_fnTrackerStatusArg;
    SPxRemoteServerAlertFn_t m_fnAlert;
    void *m_fnAlertArg;
    struct SPxRemoteServerConnectFnInfo_tag *m_fnConnectList;

    /*
     * Private functions.
     */
    /* Creation. */
    void commonConstructor(const char *hbeatAddr, int hbeatPort,
			   SPxPacketDecoderNet *decoder=NULL,
			   SPxNetTCP *tcp=NULL);
    SOCKET_TYPE setupSocket(struct sockaddr_in *socketAddress,
				const char *addr, int port, const char *ifAddr=NULL);
    SPxErrorCode setupMessageSocket(void);

    /* Command sending. */
    SPxErrorCode connectNow(const char *addr, int port);
    SPxErrorCode disconnectNow(void);
    void checkServerObjects(void);
    SPxErrorCode sendCommand(const char *commandString, int commandLen,
				char *replyBuf=NULL, int replyBufSize=0);

    SPxErrorCode sendBinaryCommand(const char *commandString, 
                                   int commandLen,
                                   unsigned char *dataBuf, 
                                   UINT32 dataBufSize,
                                   UINT32 *packetType,
                                   UINT32 *dataSize);

    SPxErrorCode recvWithTimeout(SOCKET_TYPE sock,
                                 int timeoutMsecs,
                                 char *recvBuf,
                                 unsigned int recvBufBytes,
                                 unsigned int *recvBytesRtn,
                                 unsigned int *retryCountRtn=NULL);

    /* Message handling. */
    SPxErrorCode installHandlerMsgs(SPxRemoteServerMsgFn_t fn,
				    SPxRemoteServerMsgFnV2_t fn2,
				    void *arg,
				    const char *prefix);
    SPxErrorCode handleMsg(const char *msg, int msgSizeBytes,
				struct sockaddr_in *from);

    /* Packet handling. */
    static void handleHeartbeats(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);
    static void handlePlots(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);
    static void handlePlotStatus(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);
    static void handleTracks(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);
    static void handleTrackerStatus(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);
    static void handleAlertError(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);
    static void handleAlertHPxAlarm(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);
    static void handleAlertLogic(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);

    /* Binary packet handling (from TCP connection). */
    static void handleBinError(SPxPacketDecoder *decoder, void *userArg,
                               UINT32 packetType,
                               struct sockaddr_in *fromAddr,
                               SPxTime_tag *timestamp,
                               const unsigned char *payload,
                               unsigned int numBytes);

    static void handleBinData(SPxPacketDecoder *decoder, void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *fromAddr,
                              SPxTime_tag *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);
    
    /* Utilities. */
    int exists(const char *name);

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;	/* Destination for debug messages */
    static UINT32 m_debug;	/* Debug flags */
}; /* SPxRemoteServer class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_REMOTE_SERVER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
