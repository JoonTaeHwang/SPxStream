/*********************************************************************
*
* (c) Copyright 2008 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxScDestMcast class.  This is a scan conversion
*	destination class which multicasts the window contents across
*	a network for one or more SPxScSourceMcast objects to receive.
*
*
* Revision Control:
*   28/03/19 v1.7    AGC	Rename SetRadarLUT lutId parameter to rid.
*
* Previous Changes:
*   03/11/15 1.6    AGC	PatchSet() patch data now const.
*   04/10/13 1.5    AGC	Simplify headers.
*   22/11/12 1.4    REW	Add PanView().
*   15/09/10 1.3    REW	Make destructor virtual.
*   21/08/09 1.2    REW	Add SetSweepLine().
*   19/08/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SC_DEST_MCAST_H
#define _SPX_SC_DEST_MCAST_H

/*
 * Other headers required.
 */
/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxPacketParam. */
#include "SPxLibData/SPxPackets.h"

/* Our object is derived from the standard destination class. */
#include "SPxLibSc/SPxScDest.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Maximum number of generic parameters we can handle. */
#define	SPX_SC_DEST_MCAST_MAX_PARAMS	4


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
class SPxScDest;

/*
 * Define our class.
 */
class SPxScDestMcast :public SPxScDest
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxScDestMcast(void);
    virtual ~SPxScDestMcast();

    /* Address/port control. */
    SPxErrorCode SetAddress(char *address, int port=0);
    SPxErrorCode SetAddress(UINT32 address, int port=0);

    /* ID control. */
    void SetWindowID(UCHAR id)	{ m_windowID = id; }
    UCHAR GetWindowID(void)	{ return(m_windowID); }

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

    /*
     * Static public functions.
     */
    static void SetLogFile(FILE *logFile) { m_logFile = logFile; }

private:
    /*
     * Private variables.
     */
    /* Critical section. */
    SPxCriticalSection m_mutex;

    /* Socket for sending out the radar images etc. */
    SOCKET_TYPE m_socket;		/* Socket to send via */
    struct sockaddr_in m_socketAddress;	/* Address structure */
    char *m_addressString;		/* String with dotted-IP address */
    int m_port;				/* Port to send to */ 

    /* Convenient copy of an invalid socket handle. */
    SOCKET_TYPE m_invalidSocket;

    /* Copy of scan converter configuration. */
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

    /* Flag to say we need to send configuration data out. */
    int m_configPending;

    /* Time of last configuration message (in ticks). */
    UINT32 m_lastConfigMsgTime;

    /* Buffer for building messages. */
    unsigned char *m_buffer;	/* Normally starts with an SPxPacketHeaderB */
    unsigned int m_bufferSizeBytes;	/* Size of buffer */

    /* ID of window we're sending. */
    UCHAR m_windowID;

    /* Patch sequence numbers. */
    UINT16 m_seqNum;		/* Number sent */
    UINT16 m_seqNumAck;		/* Last number acknowledged */

    /* List of generic parameters. */
    SPxPacketParam m_paramList[SPX_SC_DEST_MCAST_MAX_PARAMS];
    unsigned int m_numParams;

    /*
     * Private functions.
     */
    int setupSocketAddress(void);
    int sendConfig(UINT32 now);
    int sendParams(UINT32 now);
    int checkForAcks(void);

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;		/* Destination for debug msgs */
}; /* SPxScDestMcast */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_SC_DEST_MCAST_H*/

/*********************************************************************
*
* End of file
*
**********************************************************************/
