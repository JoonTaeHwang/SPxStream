/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxScSourceNet class, which provides the
*	network receiver for the receipt of scan-converted radar
*	video on a display process.
*
* Revision Control:
*   25/07/22 v1.53   AGC	Add client ID to command handling.
*
* Previous Changes:
*   29/03/22 1.52   AGC	Add connection ID to handle socket re-use.
*   28/06/21 1.51   AGC	Pass socket and from address to command processing.
*   18/06/21 1.50   AGC	Replace SPxScSrcNetMsgRpt.
*   06/05/21 1.49   AGC	Add default for ProcessCommand() socket argument.
*   05/05/21 1.48   AGC	Fix unused parameter warning.
*   05/05/21 1.47   AGC	Add socket to ProcessCommand().
*   02/12/20 1.46   AJH	Support remote heartbeat addr/port change.
*   30/09/20 1.45   SP 	Add SetServerAddress().
*   12/03/20 1.44   SP 	Fix N/C #761 - H/B clientAddr byte order incorrect.
*   20/02/20 1.43   SP 	Add SendHeartbeat() overload.
*   13/01/20 1.42   SP 	Support changing heartbeat address and port.
*   23/05/19 1.41   AGC	Derive from new remote control interface.
*   28/03/19 1.40   AGC	Rename SetRadarLUT lutId parameter to rid.
*   20/02/19 1.39   AGC	Support changing server port.
*   21/02/17 1.38   AGC	Add option to disable control.
*   13/09/16 1.37   SP 	Support numChannels in heartbeat.
*   02/09/16 1.36   AGC	Use atomic variable for stop command loop.
*   03/11/15 1.35   AGC	PatchSet() patch data now const.
*   23/02/15 1.34   AGC	Disable 64-bit warning from newer Windows SDKs.
*   05/02/15 1.33   AGC	Disable warnings from newer Windows SDKs.
*   15/11/13 1.32   SP 	Support binary get commands.
*   26/11/12 1.31   REW	Avoid compiler warning in AV build.
*   22/11/12 1.30   REW	Add PanView().
*   11/09/12 1.29   AGC	Issue warning if ID outside UINT16 range.
*   07/09/12 1.28   AGC	Use const for string constructor params.
*   25/04/12 1.27   REW	Remember state of EnableKeepAlive() function.
*   14/11/11 1.26   AGC	Add EnableKeepAlive() function.
*   16/06/11 1.25   REW	Add GetServerPort(). Fix line endings.
*   09/06/11 1.24   AGC	Add SendPacketB() function.
*			Avoid warnings when header compiled
*			with warning level 4 in Visual Studio.
*   24/09/10 1.23   REW	Fix socket check in GetClientAddr().
*   15/09/10 1.22   REW	Make destructor virtual.
*   15/07/10 1.21   REW	Set SPX_DISPLAY_SERVER_MAX_WINDOWS to 12 (was 8)
*   03/11/09 1.20   SP 	Add SPxScReleaseDisplay().
*   21/08/09 1.19   REW	Add SetSweepLine().
*   20/03/09 1.18   REW	Add support for binding to specific IP address.
*   13/11/08 1.17   REW	Support string/real return values in commands.
*   13/08/08 1.16   REW	Add scType to SPxScCreateDisplay().
*   11/02/08 1.15   REW	Fix #005 with patch packet acknowledgements.
*   25/10/07 1.14   REW	Change SetSource() to use UINT32 args.
*   22/10/07 1.13   REW	Add channelInfo to SPxScCreateDisplay().
*   03/09/07 1.12   REW	Add m_msgReporter.
*   15/08/07 1.11   REW	Remove deprecated API.
*   25/07/07 1.10   REW	Remove myCloseSocket().
*   09/07/07 1.9    REW	Add m_lastBackgroundMsecs.
*   06/07/07 1.8    REW	Big API change and improve heartbeat support.
*   05/07/07 1.7    REW	Add GetClientAddr().
*   04/07/07 1.6    REW	Big rework. Dynamically allocate some ports.
*   18/06/07 1.5    REW	Remove SPxScSource param to CreateDisplay().
*   13/06/07 1.4    DGJ	Window positions now signed.
*   08/06/07 1.3    DGJ	Reworked/renamed SPxSc class hierarchy.
*   07/06/07 1.2    REW	Tidy headers, add multi-include protection.
*   03/06/07 1.1    DGJ	Initial Version.
**********************************************************************/

#ifndef _SPX_SC_SOURCENET_H
#define _SPX_SC_SOURCENET_H

/*
 * Other headers required.
 */
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4068)
#ifdef _WIN64
#pragma warning(disable: 4163)
#endif
#include "winsock2.h"
#pragma warning(pop)
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif /* WIN32 */

/* Do these after the system libraries */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxMsgRpt.h"
#include "SPxScSource.h"
#include "SPxScDest.h"
#include "SPxScNet.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Define the maxinum number of windows that can be handled. */
#define SPX_DISPLAY_SERVER_MAX_WINDOWS 12


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

/* Remote control interface. */
class SPxRemoteControlIface : public SPxMessageReporterIface
{
public:
    SPxRemoteControlIface() {}
    virtual ~SPxRemoteControlIface() {}

    virtual int ProcessCommand(const char *commandString, int len,
			       char *returnBuf, int returnBufLen,
			       int *returnBufFilledPtr,
			       SOCKET_TYPE sock=SPX_SOCK_INVALID,
			       UINT32 fromAddr=0,
			       UINT32 id=0,
			       const char *clientID=NULL) = 0;
    virtual void CloseCommand(SOCKET_TYPE /*sock*/, UINT32 /*id*/=0) {};

};

/*
 * Define the class itself.
 */
class SPxScSourceNet :public SPxScSource, public SPxRemoteControlIface
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructors and destructor. */
    explicit SPxScSourceNet(FILE *log=NULL);   
    explicit SPxScSourceNet(int serverPort,
			    FILE *log=NULL,
			    int heartbeatPort=0,
			    const char *heartbeatAddress=NULL,
			    const char *serverAddress=NULL,
			    int controlAvailable=TRUE);
    virtual ~SPxScSourceNet();

    /* Function to check constructor was okay and sockets are created.
     * Returns TRUE if everything is okay, FALSE otherwise.
     * Also, functions to get information about the object after creation.
     */
    int SocketsOK(void)	{ return( m_socketError ? FALSE : TRUE); }
    int EnableKeepAlive(void);
    SPxErrorCode SetServerAddress(const char *ifAddr);
    const char *GetServerAddress(void) { return m_serverAddress; }
    int SetServerPort(int serverPort);
    int GetServerPort(void)	{ return(m_serverPort); }

    /* Function to try to accept a client connection (non-blocking). */
    SOCKET_TYPE TryAccept(void);
    const struct sockaddr_in *GetClientAddr(void)
    {
	/* Return NULL if no client connected, or address struct otherwise. */
	if( m_TCPcommandSocket == SPX_SOCK_INVALID )
	{
	    return(NULL);
	}
	return(&m_TCPcommandAddress);
    }

    /* Function to run the command loop after accepting a connection,
     * and other functions to manipulate that loop.
     */
    void CommandLoop(void);
    void StopCommandLoop(void)			{m_stopCommandLoop = 1;}
    void SetEventHandler(void (*fn)(void))	{m_userFDhandler = fn;}
    void SetEventSource(int fd)			{m_userFD = fd;}
    void SetTimeout(unsigned int second, unsigned int microseconds)
    {
	m_timeoutS = second;
	m_timeoutUS = microseconds;
	while(m_timeoutUS > 999999)
	{
	    m_timeoutS++;
	    m_timeoutUS -= 1000000;
	}
    } /* SetTimeout() */
    void SetBackgroundProc(void (*fn)(void*), void* userData);

    /* Function to set/change the log file after construction. */
    void SetLogFile(FILE *logFile)	{m_logFile = logFile;}
    void SetDebug(UINT32 debug)		{m_debug = debug;}

    /* Heartbeat control. */
    SPxErrorCode SetHeartbeatAddress(const char *addr, int port=0);
    const char *GetHeartbeatAddress(void) { return m_heartbeatAddressString; }
    SPxErrorCode SetHeartbeatPort(int port);
    int GetHeartbeatPort(void) { return m_heartbeatPort; }
    void SendHeartbeat(void);
    void SendHeartbeat(UINT8 health,
		       UINT16 sourceStatus,
		       UINT8 sourceType,
		       UINT8 numChannels);
    void EnableHeartbeats(int state)	{m_heartbeatEnable = state;}
    void SetState(int state)		{m_currentState = state;}
    int GetState(void)			{return m_currentState;}
    void SetID(int id);
    int GetID(void)			{return m_appId;}
    void SetNumChannels(UINT8 numChannels);
    int GetNumChannels(void)		{return m_numChannels;}

    /* Other functions.  Should these all be public? */
    void Reset(void);			/* Reset state - new connection */
    int AddDisplay(SPxScDest *display);	/* Add a window */
    void RemoveDisplay(int windowId);	/* Remove a window */
    int IsWIDok(int wid);		/* Check a window ID */
    virtual int ProcessCommand(const char *commandString, int len,
			       char *returnBuf, int returnBufLen,
			       int *returnBufFilledPtr,
			       SOCKET_TYPE sock=SPX_SOCK_INVALID,
			       UINT32 fromAddr=0, UINT32 id=0,
			       const char *clientID=NULL);
    int ProcessPatch(UINT32 *patchSet, int len,
			struct sockaddr *senderAddr, int senderSize);
    virtual int SendMessage(const char *buf, int len=0); /* Send UDP msg to client */
    SPxErrorCode SendPacketB(UINT16 packetType,
			     UINT32 timeSecs, UINT32 timeUsecs,
			     const unsigned char *payload,
			     unsigned int payloadSizeBytes);

    /* Null implementation of the base scan conversion API. */
    int SetWinPos(INT16 /*screenX*/, INT16 /*screenY*/)		{return 0;}
    int SetWinGeom(INT16 /*screenX*/, INT16 /*screenY*/,
		   UINT16 /*screenW*/, UINT16 /*screenH*/)	{return 0;}
    int SetWinParam(UINT16 /*param*/, UINT32 /*arg1*/,
		    UINT32 /*arg2*/=0, UINT32 /*arg3*/=0)	{return 0;}
    int SetFade(UCHAR /*fadeType*/, UINT16 /*fadeRate*/)	{return 0;}
    int SetView(REAL32 /*viewX*/, REAL32 /*viewY*/,
		REAL32 /*viewW*/, REAL32 /*viewH*/)		{return 0;}
    int PanView(REAL32 /*viewX*/, REAL32 /*viewY*/, UCHAR /*clearTrails */=0)
								{return 0;}
    int SetRadarColour(UCHAR /*rid*/, UCHAR /*red*/,
		       UCHAR /*green*/, UCHAR /*blue*/)		{return 0;}
    int SetRadarBright(UCHAR /*rid*/, UCHAR /*bright*/)		{return 0;}
    int SetRadarLUT(UCHAR /*rid*/, UCHAR * /*lutData*/)	{return 0;}
    int ShowRadar(UCHAR /*rid*/, UCHAR /*state*/)		{return 0;}
    int PatchSet(UCHAR /*format*/, UINT16 /*numPatches*/,
		 const UINT16 * /*patchData*/, UINT16 /*patchLen*/)	{return 0;}
    int SetSource(UINT32 /*sourceID*/, UINT32 /*arg1*/=0, 
		  UINT32 /*arg2*/=0)				{return 0;}
    int SetRadarPosition(REAL32 /*radarX*/, REAL32 /*radarY*/,
			 UCHAR /*clearTrails*/=0)		{return 0;}
    int SetSweepLine(SPxScSweepLineType_t /*type*/,
		     SPxScSweepLineColMode_t /*colourMode*/,
		     UINT32 /*rgb*/,
		     UINT32 /*reserved1*/, UINT32 /*reserved2*/) {return 0;}

    /*
     * Static public functions.
     */

private:
    /*
     * Private variables.
     */
    /* Handle of file to output logging messages to, or NULL. */
    FILE *m_logFile;
    UINT32 m_debug;

    /*
     * Sockets and addresses.
     */
    /* The main server socket. */
    SOCKET_TYPE m_TCPserverSocket;		/* Main server socket */
    sockaddr_in m_TCPserverAddress;		/* Address of main socket */
    int m_serverPortSet;			/* Server port set (may be zero). */
    int m_serverPort;				/* Port number for server */
    char *m_serverAddress;			/* Address to use */
    int m_controlAvailable;			/* Is control available? */
    int m_tcpKeepAliveEnabled;			/* Flag */

    /* The socket for receiving patch data.*/
    SOCKET_TYPE m_UDPpatchSocket;		/* Patch data socket */
    sockaddr_in m_UDPpatchAddress;		/* Address of patch socket */
    int m_patchPort;				/* Port number for patches */

    /* The socket for sending heartbeats. */
    SOCKET_TYPE m_UDPheartbeatSocket;		/* Heartbeat socket */
    sockaddr_in m_UDPheartbeatAddress;		/* Address for heartbeats */
    int m_heartbeatPort;			/* Port number */
    char *m_heartbeatAddressString;		/* Address as a string */

    /* The socket for sending asynchronous messages. */
    SOCKET_TYPE m_UDPmessageSocket;		/* Message socket */
    sockaddr_in m_UDPmessageAddress;		/* Address for messages */

    /* The socket on which our current connection is active. */
    SOCKET_TYPE m_TCPcommandSocket;		/* Connection socket */
    struct sockaddr_in m_TCPcommandAddress;	/* Address for socket */
    UINT32 m_clientAddress;			/* Client addr */

    /* Flag to indicate if constructor had problems creating sockets.
     * Use SocketsOK() to check this.
     */
    int m_socketError;

    /* Values for main command loop. */
    unsigned int m_timeoutS, m_timeoutUS;		/* Timeout */
    unsigned char *m_buffer;		/* Packet B buffer. */
    unsigned int m_bufferSizeBytes;	/* Size of m_buffer. */
    char m_commandBuffer[SPX_SCNET_MAX_SIZE_COMMAND_BUF_BYTES];	/* Commands */
    UINT32 m_patchBuffer[SPX_SCNET_MAX_SIZE_PATCH_BUF_UINT32];	/* Patches */
    unsigned char *m_binBuffer;         /* Binary data buffer. */
    unsigned int m_binBufferSizeBytes;	/* Size of m_binBuffer. */
    SPxAtomic<int> m_stopCommandLoop;	/* Set to TRUE to stop loop */
    int m_userFD;			/* Extra file descriptor to check */
    void (*m_userFDhandler)();		/* Function to call for m_userFD */

    /* Heartbeat control etc. */
    SPxScNetHeartbeatStruct m_heartbeat;	/* The heartbeat structure */
    int m_heartbeatEnable;			/* Are they enabled? */
    UINT16 m_appId;				/* Application ID */
    UINT8 m_numChannels;			/* Number of channels. */
    int m_currentState;				/* Application state */

    /* User-supplied periodic background function. */
    void (*m_backgroundFunction)(void *);
    void *m_backgroundUserData;
    UINT32 m_lastBackgroundMsecs;

    /* Information about active windows. */
    int m_numActiveWindows;			/* Number of windows */
    SPxScDest *m_mapIDtoObject[SPX_DISPLAY_SERVER_MAX_WINDOWS];	/* Table */

    /* Async message reporting. */
    SPxMessageReporter *m_msgReporter;

    /*
     * Private functions.
     */
    /* Constructor and creation assistance. */
    void commonInit(FILE *log=0);
    int initSockets(void);
    void closeSockets(int closeTCP=TRUE);
    void handleCommand(char *commandString, int bytesRead);
    void handleBinaryCommand(char *commandString, int bytesRead);

}; /* SPxScSourceNet */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* This is a function that must be provided by the application.  There
 * is a stub implementation in the library to allow linking when applications
 * are not using this class, but if they do use it they must supply a
 * real implementation to override it.
 */
extern SPxScDest *SPxScCreateDisplay(UCHAR screen,
					UINT16 maxW, UINT16 maxH,
					UINT32 win1, UINT32 win2,
					UINT32 channelInfo,
					UINT32 scType,
					UINT32 *errPtr);

/* This is a function that may be provided by the application to override
 * the default implementation.  
 */
extern void SPxScReleaseDisplay(SPxScDest *disp);

#endif /* _SPX_SC_SOURCENET_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
