/*********************************************************************
*
* (c) Copyright 2007 - 2019, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxSCDestNet class, which provides the
*	network transmission for the distribution of scan-converted
*	radar video to a display process.
*
* Revision Control:
*   14/06/21 v1.26   SP 	Support view control.
*
* Previous Changes:
*   28/03/19 1.25   AGC	Rename SetRadarLUT lutId parameter to rid.
*   03/11/15 1.24   AGC	PatchSet() patch data now const.
*   04/07/14 1.23   AGC	Simplify headers.
*   17/02/14 1.22   AGC	Use const where appropriate.
*   01/02/13 1.21   REW	Add GetScType().
*   22/11/12 1.20   REW	Add PanView().
*   25/04/12 1.19   REW	Add EnableKeepAlive() function.
*   15/09/10 1.18   REW	Make destructor virtual.
*   18/11/09 1.17   REW	Add timeoutMsecs to Connect().
*   21/08/09 1.16   REW	Add SetSweepLine().
*   23/06/09 1.15   REW	SendCommand() can return reply buffer.
*   13/08/08 1.14   REW	Support Bscan windows with scType arguments.
*   11/02/08 1.13   REW	Fix #005 with patch packet acknowledgements.
*   29/10/07 1.12   REW	Add mutex.
*   25/10/07 1.11   REW	Change SetSource() to use UINT32 args.
*   22/10/07 1.10   REW	Add channelInfo arg to Create() call.
*   15/08/07 1.9    REW	Remove deprecated API.
*   06/07/07 1.8    REW	Big API change and improve heartbeat support.
*   04/07/07 1.7    REW	Rework, dynamically assign ports etc.
*   27/06/07 1.6    REW	Support name lookups.
*   20/06/07 1.5    REW	Create function does not need scanConverter.
*   13/06/07 1.4    DGJ	Changed window positions to signed.
*   08/06/07 1.3    DGJ	Tweaked name capitalisation. Class derived
*			from SPxSc base class ultimately.
*   07/06/07 1.2    REW	Formatting etc.
*   03/06/07 1.1    DGJ	Initial Version.
**********************************************************************/

#ifndef _SPxScDestNet_h
#define _SPxScDestNet_h

/*
 * Other headers required.
 */
#include "stdio.h"
#include "SPxLibUtils/SPxCommon.h"
#include "SPxScDest.h"
#include "SPxScSourceLocal.h"
#include "SPxScNet.h"

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
class SPxViewControl;

/*
 * Define the class itself for network distribution of scan converted radar.
 *
 * For each of the functions provided by the parent class SPxScDest (the
 * scan-conversion destination), this class provides one side of a network
 * interface that moves the requests and data across to a remote server
 * where the functions are implemented.  This is effectively implementing
 * a remote procedure call of the functions.
 */
class SPxScDestNet :public SPxScDest
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor etc. */
    SPxScDestNet(void);
    virtual ~SPxScDestNet(void);
    int Create(UCHAR screenID, UINT16 maxWidth, UINT16 maxHeight,	
				UINT32 ulayWindow, UINT32 olayWindow,
				UINT32 channelInfo = 0,
				UINT32 scType = SPX_SC_TYPE_PPI);

    /* Network initialisation etc. (per class, not per object) */
    static int MakeNetwork(const char *serverAddress=NULL,
				int serverPort=0,
				int heartbeatPort=0,
				const char *heartbeatAddress=NULL);
    static int Connect(unsigned int timeoutMsecs=0);
    static int Disconnect(void);
    static int EnableKeepAlive(void);

    /* Debug control (per class, not per object). */
    static void SetLogFile(FILE *logfile)	{m_logFile = logfile;}      
    static void SetDebug(UINT32 debug)		{m_debug = debug;}
    static void ShowMessages(void (*fn)(char*, int)) {m_showMessageFn = fn;}

    /* Message sending. */
    int SendCommand(char *commandString, int commandLen, int *returnVal, 
			int override=0, int showMessage=1,
			int replyBufSize=0, char *replyBuf=NULL);
    static int FormatHeartbeat(const char *buf, SPxScNetHeartbeatStruct *heartbeat); 

    /* Message receipt/checking. */
    static int PollHeartbeat(SPxScNetHeartbeatStruct *heartbeat,
				UINT32 *sendingAddress = 0);
    static int PollServer(int poll, char *buf, int len, 
				int *returnBytesRead, UINT32 *senderAddr=0);
    static int PollMessage(char * buf, int len);

    /* The base class implementation. */
    int SetWinPos(INT16 screenX, INT16 screenY);
    int SetWinGeom(INT16 screenX, INT16 screenY,UINT16 screenW,UINT16 screenH);
    int SetWinParam(UINT16 param, UINT32 arg1, UINT32 arg2, UINT32 arg3);
    int SetFade(UCHAR fadeType, UINT16 fadeRate);
    int SetView(REAL32, REAL32, REAL32, REAL32);
    int PanView(REAL32 viewX, REAL32 viewY, UCHAR clearTrails=0);
    int SetRadarColour(UCHAR rid, UCHAR arg1, UCHAR arg2, UCHAR arg3);
    int SetRadarBright(UCHAR rid, UCHAR bright);
    int SetRadarLUT(UCHAR rid, UCHAR *lutData);
    int ShowRadar(UCHAR rid, UCHAR state);
    int PatchSet(UCHAR format, UINT16 numPatches,const UINT16 *patchData,UINT16 len);
    int SetRadarPosition(REAL32 radarX, REAL32 radarY, UCHAR clearTrails=0);
    int SetSource(UINT32, UINT32, UINT32);            
    int SetSweepLine(SPxScSweepLineType_t type,
				SPxScSweepLineColMode_t colourMode,
				UINT32 rgb,
				UINT32 reserved1, UINT32 reserved2);
    int GetScType(void) { return(m_scType); }

    /* External view control. */
    SPxErrorCode SetViewControl(SPxViewControl *obj,
				unsigned int radarIndex = 0,
				unsigned int platformIndex = 0);
    SPxViewControl *GetViewControl(void) const;

private:
    /*
     * Private variables.
     */
    /* Non-static, per instance members. */
    int m_screenID;
    int m_maxWindowWidth;
    int m_maxWindowHeight;
    int m_underlay, m_overlay;		
    int m_wid;					// The Window ID.
    int m_constructOK;				// Made ok?
    int m_scType;

    /* Socket for connecting to server and sending commands. */
    static SOCKET_TYPE m_commandSocket;
    static struct sockaddr_in m_commandAddress;
    static char *m_serverAddressString;
    static int m_commandPort;
    static int m_keepAliveEnabled;

    /* Socket for sending patch data. */
    static SOCKET_TYPE m_patchSocket;
    static struct sockaddr_in m_patchAddress;
    static int m_patchPort;
    static UINT16 m_patchSeq;		/* Sequence number being sent */
    static UINT16 m_patchSeqRecv;	/* Sequence number received */

    /* Socket for receiving heartbeats. */
    static SOCKET_TYPE m_heartbeatSocket;
    static struct sockaddr_in m_heartbeatAddress;
    static const char *m_heartbeatAddressString;
    static int m_heartbeatPort;

    /* Socket for receiving asynchronous messages. */
    static SOCKET_TYPE m_messageSocket;
    static struct sockaddr_in m_messageAddress;
    static int m_messagePort;

    /* Debug. */
    static FILE *m_logFile;
    static UINT32 m_debug;

    /* Flags to say things are initialised. */
    static int m_networkOK;
    static int m_connectOK;
    static int m_handshakeSent;

    /* Function to show messages. */
    static void (*m_showMessageFn)(char *text, int len);

    /* Buffer for handling commands.*/
    static char m_commandBuf[SPX_SCNET_MAX_SIZE_COMMAND_BUF_BYTES];	

    /* Mutex for protecting static fields such as m_commandBuf. */
    static SPxCriticalSection *m_mutex;

    /* External view control. */
    SPxViewControl *m_viewCtrl;          /* Installed view control object. */
    unsigned int m_viewCtrlRadarIndex;   /* Index of radar in view control. */
    unsigned int m_viewCtrlPlatformIndex;/* Index of platform in view control. */

    /*
     * Private functions.
     */
    void checkForAcknowledgements(void);

    /* View control callbacks. */
    static int viewControlHandler(void *invokingObj,
				  void *contextPtr,
				  void *changeFlagsPtr);
    static int viewControlDeleted(void *invokingObject,
				  void *userObject,
				  void *arg);
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPxScDestNet_h */

/*********************************************************************
*
* End of file
*
**********************************************************************/

