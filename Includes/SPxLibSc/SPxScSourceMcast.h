/*********************************************************************
*
* (c) Copyright 2008 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxScSourceMcast class.  This is a scan conversion
*	source class which receives multicast window contents across
*	a network from a remote scan converter and writes it to a
*	destination (such as a bitmap or a display).
*
*
* Revision Control:
*   28/03/19 v1.11   AGC	Rename SetRadarLUT lutId parameter to rid.
*
* Previous Changes:
*   03/11/15 1.10   AGC	PatchSet() patch data now const.
*   04/10/13 1.9    AGC	Simplify headers.
*   22/11/12 1.8    REW	Add PanView().
*   06/04/11 1.7    AGC	Use new SPxPacketDecoderNet class.
*   15/09/10 1.6    REW	Make destructor virtual.
*   21/08/09 1.5    REW	Add SetSweepLine().
*   16/09/08 1.4    REW	Use SPxPacketDecoder object.
*   22/08/08 1.3    REW	Add GetAddress() and GetPort() functions.
*   20/08/08 1.2    REW	Add more Get function.
*   19/08/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SC_SOURCE_MCAST_H
#define _SPX_SC_SOURCE_MCAST_H

/*
 * Other headers required.
 */
/* For SPxPacketParam. */
#include "SPxLibData/SPxPackets.h"

/* Our object is derived from the standard source class. */
#include "SPxLibSc/SPxScSource.h"

/* We also use the destination class. */
#include "SPxLibSc/SPxScDest.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Flags to indicate what has changed in the remote scan converter. */
#define	SPX_SC_MCAST_CHANGED_POS	0x00000001	/* Window position */
#define	SPX_SC_MCAST_CHANGED_DIM	0x00000002	/* Window dimensions */
#define	SPX_SC_MCAST_CHANGED_VIEW	0x00000004	/* View mapping */
#define	SPX_SC_MCAST_CHANGED_RADAR	0x00000008	/* Radar position */
#define	SPX_SC_MCAST_CHANGED_STATE	0x00000010	/* Window state */
#define	SPX_SC_MCAST_CHANGED_FADE	0x00000020	/* Fade rate/type */
#define	SPX_SC_MCAST_CHANGED_COL	0x00000040	/* Colour/brightness */
#define	SPX_SC_MCAST_CHANGED_SOURCE	0x00000080	/* Source info */
#define	SPX_SC_MCAST_CHANGED_SWEEPLINE	0x00000100	/* Sweepline */

/* Maximum number of generic parameters we can handle. */
#define	SPX_SC_SOURCE_MCAST_MAX_PARAMS	4


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

/* Forward declare any classes we need in our own class in case
 * headers are included in the wrong order.
 */
class SPxScSource;
class SPxPacketDecoderNet;
struct SPxTime_tag;


/*
 * Define our class.
 */
class SPxScSourceMcast :public SPxScSource
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxScSourceMcast(SPxScDest *dest, int master = FALSE);
    virtual ~SPxScSourceMcast();

    /* Creation. */
    SPxErrorCode Create(char *address, int port=0);

    /* Address/port control. */
    SPxErrorCode SetAddress(char *address, int port=0);
    SPxErrorCode SetAddress(UINT32 address, int port=0);
    UINT32 GetAddress(void);
    int GetPort(void);

    /* ID control. */
    void SetWindowID(UCHAR id)	{ m_windowID = id; }
    UCHAR GetWindowID(void)	{ return(m_windowID); }

    /* Change-notification. */
    void InstallChangeFn(void (*fn)(SPxScSourceMcast *, void *, UINT32),
				void *arg)
    {
	m_changeFnArg = arg;
	m_changeFn = fn;
    }

    /* Information retrieval. */
    SPxScDest *GetDest(void)    { return(m_dest); }
    int GetState(void)          { return(m_scState); }
    int GetFadeType(void)       { return m_fadeType; }
    int GetFadeRate(void)       { return m_fadeRate; }
    int GetView(REAL32 *xPtr, REAL32 *yPtr, REAL32 *wPtr, REAL32 *hPtr)
    {
	if(xPtr) { *xPtr = m_viewX; }
	if(yPtr) { *yPtr = m_viewY; }
	if(wPtr) { *wPtr = m_viewW; }
	if(hPtr) { *hPtr = m_viewH; }
	return(SPX_NO_ERROR);
    }
    int GetWinGeom(INT16 *screenXPtr, INT16 *screenYPtr,
			UINT16 *screenWPtr, UINT16 *screenHPtr)
    {
	if(screenXPtr)  { *screenXPtr = (INT16)m_screenX; }
	if(screenYPtr)  { *screenYPtr = (INT16)m_screenY; }
	if(screenWPtr)  { *screenWPtr = (UINT16)m_screenW; }
	if(screenHPtr)  { *screenHPtr = (UINT16)m_screenH; }
	return(SPX_NO_ERROR);
    }
    int GetRadarPosition(REAL32 *xPtr, REAL32 *yPtr)
    {
	if(xPtr) { *xPtr = m_radarX; }
	if(yPtr) { *yPtr = m_radarY; }
	return(SPX_NO_ERROR);
    }
    int GetSourceInfo(UINT32 *idPtr, UINT32 *arg1Ptr, UINT32 *arg2Ptr)
    {
	if(idPtr) { *idPtr = m_sourceID; }
	if(arg1Ptr) { *arg1Ptr = m_sourceArg1; }
	if(arg2Ptr) { *arg2Ptr = m_sourceArg2; }
	return(SPX_NO_ERROR);
    }

    /* Overrides for the pure virtual functions from SPxSc base. */
    int SetWinPos(INT16 screenX, INT16 screenY);
    int SetWinGeom(INT16 screenX, INT16 screenY,
				UINT16 screenW, UINT16 screenH);
    int SetWinParam(UINT16 param, UINT32 arg1,
				UINT32 arg2=0, UINT32 arg3=0);
    int SetFade(UCHAR fadeType, UINT16 fadeRate);
    int SetView(REAL32 viewX, REAL32 viewY, REAL32 viewW, REAL32 viewH);
    int PanView(REAL32 viewX, REAL32 viewY, UCHAR clearTrails=0);
    int SetRadarColour(UCHAR rid, UCHAR red, UCHAR green, UCHAR blue);
    int SetRadarBright(UCHAR rid, UCHAR bright);
    int SetRadarLUT(UCHAR rid, UCHAR * lutData);
    int ShowRadar(UCHAR rid, UCHAR state);
    int PatchSet(UCHAR format, UINT16 numPatches,
				const UINT16 *patchData, UINT16 patchLen);
    int SetSource(UINT32 sourceID, UINT32 arg1=0, UINT32 arg2=0);
    int SetRadarPosition(REAL32 radarX, REAL32 radarY, UCHAR clearTrails=0);
    int SetSweepLine(SPxScSweepLineType_t type,
				SPxScSweepLineColMode_t colourMode,
				UINT32 rgb,
				UINT32 reserved1, UINT32 reserved2);

    /* Thread entry point. */
    void *ThreadFn(SPxThread *thread);

    /*
     * Static public functions.
     */
    static void SetLogFile(FILE *logFile) { m_logFile = logFile; }

private:
    /*
     * Private variables.
     */
    /* Our destination object. */
    SPxScDest *m_dest;			/* Destination object */

    /* Critical section. */
    SPxCriticalSection m_mutex;
    int m_waitingForMutex;		/* Main thread wants mutex */

    /* Worker thread for receiving data. */
    SPxThread *m_thread;

    /* Socket for receiving the radar images etc. */
    SOCKET_TYPE m_socket;		/* Socket to send via */
    struct sockaddr_in m_socketAddress;	/* Address structure */
    char *m_addressString;		/* String with dotted-IP address */
    int m_port;				/* Port to send to */ 
    int m_isBound;			/* Is socket bound? */
    struct sockaddr_in m_fromAddr;	/* Address of sender at far end */

    /* Convenient copy of an invalid socket handle. */
    SOCKET_TYPE m_invalidSocket;

    /* Packet decoding object. */
    SPxPacketDecoderNet *m_packetDecoder;   /* Packet decoding */

    /* Flag to say if we are owned by the master of the scan converter. */
    int m_master;

    /* Copy of remote scan converter configuration. */
    INT16 m_screenX, m_screenY;	/* Position of window on screen */
    INT16 m_screenW, m_screenH;	/* Width of window in pixels */
    REAL32 m_viewX, m_viewY;	/* Coords at centre of window */
    REAL32 m_viewW, m_viewH;	/* Width & height of window in world coords */
    REAL32 m_radarX, m_radarY;	/* Position of radar in world coords */
    UCHAR m_scState;		/* State of radar (SPX_SC_STATE...) */
    UCHAR m_fadeType;		/* Fade mode (SPX_RADAR_FADE_...) */
    UINT16 m_fadeRate;		/* Fade rate (revs, or 1/100ths of secs) */
    UCHAR m_red, m_green, m_blue;	/* Colour of radar */
    UCHAR m_bright;			/* Brightness of radar */
    UINT32 m_sourceID;			/* Source ID */
    UINT32 m_sourceArg1, m_sourceArg2;	/* Args to go with source ID */
    SPxScSweepLineType_t m_sweepLineType;	/* Sweep line type */
    SPxScSweepLineColMode_t m_sweepLineColMode;	/* Sweep line mode */
    UINT32 m_sweepLineRGB;			/* Sweep line colour */

    /* Flag to say we still need to receive a configuration message. */
    int m_configPending;

    /* Buffer for receiving messages. */
    unsigned char *m_buffer;	/* Normally starts with an SPxPacketHeaderB */
    unsigned int m_bufferSizeBytes;	/* Size of buffer */

    /* ID of window we're receiving, or zero to not check it. */
    UCHAR m_windowID;

    /* Generic parameters. */
    SPxPacketParam m_paramList[SPX_SC_SOURCE_MCAST_MAX_PARAMS];
    unsigned int m_numParams;

    /* User-installed change function. */
    void (*m_changeFn)(SPxScSourceMcast *, void *, UINT32);
    void *m_changeFnArg;

    /*
     * Private functions.
     */
    static void handlePacket(SPxPacketDecoder *decoder, void *userArg,
				UINT32 packetType,
				struct sockaddr_in *fromAddr,
				SPxTime_tag *timestamp,
				const unsigned char *payload,
				unsigned int numBytes);
    int handleConfig(const unsigned char *data, int numBytes);
    int handlePatches(const unsigned char *data, int numBytes);
    int handleParams(const unsigned char *data, int numBytes);
    int setupSocketAddress(void);

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;		/* Destination for debug msgs */
}; /* SPxScSourceMcast */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_SC_SOURCE_MCAST_H*/

/*********************************************************************
*
* End of file
*
**********************************************************************/
