/*********************************************************************
*
* (c) Copyright 2007 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxScDestScServer class.
*
*
* Revision Control:
*   28/03/19 v1.22   AGC	Rename SetRadarLUT lutId parameter to rid.
*
* Previous Changes:
*   03/11/15 1.21   AGC	PatchSet() patch data now const.
*   04/10/13 1.20   AGC	Simplify headers.
*   22/11/12 1.19   REW	Add PanView().
*   15/09/10 1.18   REW	Make destructor virtual.
*   02/12/09 1.17   SP 	Add EnableTrailHistory().
*   17/09/09 1.16   REW	Support trail history retention.
*   21/08/09 1.15   REW	Add SetSweepLine().
*   20/08/08 1.14   REW	Generic m_dest, not specific m_destNet.
*   13/08/08 1.13   REW	Support Bscan windows with scType in Create().
*   25/10/07 1.12   REW	Change SetSource() to use UINT32 args.
*   25/10/07 1.11   REW	Add channelInfo and use SPxRadarStream.
*   10/09/07 1.10   REW	Add m_sectorBlankProcess.
*   01/08/07 1.9    REW	Use SPxRadarReplay for encoded files, not Raw.
*   30/07/07 1.8    REW	Support user-supplied source (one only).
*   25/07/07 1.7    REW	Support network sources.
*   06/07/07 1.6    REW	Add heartbeat support. Change SetDisplayAddr()
*			to SetDisplayServer().
*   05/07/07 1.5    REW	Add SetClientAddr().
*   03/07/07 1.4    REW	Support file replay and scenario sources.
*   02/07/07 1.3    REW	Move DestNet control here from main.
*   29/06/07 1.2    REW	Add m_logFile support.
*   29/06/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SC_DEST_SC_SERVER_H
#define _SPX_SC_DEST_SC_SERVER_H

/*
 * Other headers required.
 */

/* Our object is derived from the standard destination class. */
#include "SPxLibSc/SPxScDest.h"

/* But we create SPxScDestNet or SPxScDestMcast objects as well. */
#include "SPxLibSc/SPxScDestNet.h"
#include "SPxLibSc/SPxScDestMcast.h"

/* We also need to create local scan converter objects. */
#include "SPxLibSc/SPxScSourceLocal.h"


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

/* Forward declare any classes we need. */
class SPxScDest;
class SPxScDestNet;
class SPxRadarStream;

/*
 * Define the class that translates from the dest object to an internal
 * sc-source object.
 */
class SPxScDestScServer :public SPxScDest
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxScDestScServer(unsigned int numSamples=2048,
			unsigned int numAzis=2048);
    virtual ~SPxScDestScServer();

    /* Creation/setup function. */
    int Create(UCHAR screen, UINT16 maxW, UINT16 maxH,
				UINT32 win1, UINT32 win2,
				UINT32 channelInfo, UINT32 scType);

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

    /* Information retrieval functions. */
    SPxRadarStream *GetRadarStream(void)	{ return(m_radarStream); }


    /*
     * Static public functions.
     */
    static void SetLogFile(FILE *logFile) { m_logFile = logFile; }
    static int SetDisplayServer(const char *addr, int port=0);
    static int SetHeartbeatDest(const char *addr, int port=0);
    static int SetClientAddr(const char *addr);
    static int DeleteAllObjects(void);
    static int EnableTrailHistory(int isEnabled);

private:
    /*
     * Private variables.
     */
    /* Linked list pointer. */
    SPxScDestScServer *m_next;

    /* Resolution. */
    unsigned int m_numSamples;
    unsigned int m_numAzis;

    /* Underlying scan conversion and network-dest objects. */
    SPxScSourceLocal *m_scanConv;
    SPxScDest *m_dest;

    /* Radar stream feeding this window. */
    SPxRadarStream *m_radarStream;

    /* Trail history objects. */
    SPxPIM *m_historyPim;
    SPxRunProcess *m_historyProcess;

    /* Process for passing data to the scan converter. */
    SPxRunProcess *m_scanConvProcess;

    /*
     * Private functions.
     */

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;		/* Destination for debug msgs */
    static char *m_clientAddr;		/* IP address of user's client */
    static char *m_displayAddr;		/* IP address/name of display svr */
    static int m_displayPort;		/* Port number for display svr */
    static int m_displayInitialised;	/* Have we created sockets yet? */
    static int m_displayConnected;	/* Are we connected to the svr? */
    static char *m_heartbeatAddr;	/* IP address for heartbeats */
    static int m_heartbeatPort;		/* Port number for heartbeats */
    static SPxScDestScServer *m_FirstObject;	/* Linked list handle */
    static int m_trailHistEnabled;      /* Enable/disable trail history */

}; /* SPxScDestScServer */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_SC_DEST_SC_SERVER_H*/

/*********************************************************************
*
* End of file
*
**********************************************************************/
