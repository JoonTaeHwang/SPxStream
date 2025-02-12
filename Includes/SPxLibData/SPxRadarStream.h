/*********************************************************************
*
* (c) Copyright 2007, 2009, 2010, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRadarStream.h,v $
* ID: $Id: SPxRadarStream.h,v 1.12 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header for SPxRadarStream class.
*
*
* Revision Control:
*   04/10/13 v1.12   AGC	Simplify headers.
*
* Previous Changes:
*   15/09/10 1.11   REW	Make destructor virtual.
*   04/12/09 1.10   REW	Add GetNetSource() etc.
*   04/12/09 1.9    REW	Add m_debugProcess and m_lastProcess.
*   20/11/09 1.8    SP 	Add userA plugin process.
*   17/09/09 1.7    REW	Add pimB to CreateAndAddProcess().
*   31/03/09 1.6    REW	Add range blank process.
*   07/12/07 1.5    REW	Add threshold process.
*   08/11/07 1.4    REW	Add range rings process.
*   25/10/07 1.3    REW	Add ReplaceSource().
*   25/10/07 1.2    REW	Change SetSource() to use UINT32 args.
*   25/10/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_RADAR_STREAM_H
#define _SPX_RADAR_STREAM_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"

/* We need the network receiver definitions. */
#include "SPxLibNet/SPxNetRecv.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"


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

/* Forward declare any classes we need in our own class in case
 * headers are included in the wrong order.
 */
class SPxRIB;
class SPxPIM;
class SPxTestGenerator;
class SPxScenarioGenerator;
class SPxRadarReplay;
class SPxNetworkReceive;
class SPxSyncCombineSrc;
class SPxRunProcess;
class SPxProcess;
class SPxPluginHandler;
class SPxRadarDatabase;

/*
 * Define out class.
 */
class SPxRadarStream :public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRadarStream(UINT32 channelInfo = 0,
			unsigned int numSamples = 2048,
			unsigned int numAzis = 2048);
    virtual ~SPxRadarStream();

    /* Functions to add user-supplied sources. */
    int AddSource(SPxRadarSource *src, unsigned int setNumber,
				int freeOnDestructor);
    int ReplaceSource(SPxRadarSource *src, unsigned int srcType);

    /* Process addition/removal. */
    SPxRunProcess *CreateAndAddProcess(SPxProcess *process,
					SPxPIM *pimB,
					void *userArgs);
    int RemoveAndDeleteProcess(SPxRunProcess *runProcess);

    /* Configuration functions. */
    int SetSource(UINT32 sourceID, UINT32 arg1=0, UINT32 arg2=0);

    /* Information retrieval functions. */
    SPxRIB *GetRIB(void)	{ return(m_rib); }
    SPxPIM *GetPIM(void)	{ return(m_pim); }
    int IsSingleChannel(void)
    {
	/* Single-channel unless we have a SyncCombine source. */
	return( (m_srcSyncCombine == NULL) ? TRUE : FALSE );
    }
    SPxRadarSource *GetUserSource(void)		{ return(m_srcUser); }
    SPxNetworkReceive *GetNetSource(void)	{ return(m_srcNet); }
    SPxTestGenerator *GetTestSource(void)	{ return(m_srcTest); }
    SPxScenarioGenerator *GetScenSource(void)	{ return(m_srcScen); }
    SPxRadarReplay *GetReplaySource(void)	{ return(m_srcReplay); }


    /*
     * Static public functions.
     */
    static SPxRadarStream *GetStream(UINT32 channelInfo = 0,
					unsigned int numSamples = 2048,
					unsigned int numAzis = 2048);
    static void SetLogFile(FILE *logFile) { m_logFile = logFile; }
    static void SetRadarDatabase(SPxRadarDatabase *db) { m_rdb = db; }
    static int SetFileReplayName(const char *name);
    static int DeleteAllObjects(void);

private:
    /*
     * Private variables.
     */
    /* Linked list information. */
    SPxRadarStream *m_next;

    /* Channel information. */
    UINT32 m_channelInfo;

    /* Resolution. */
    unsigned int m_numSamples;
    unsigned int m_numAzis;

    /* RIB, PIM etc. */
    SPxRIB *m_rib;
    SPxPIM *m_pim;

    /* Plugin handler */
    SPxPluginHandler *m_pluginHdlr;

    /* Processes. */
    SPxRunProcess *m_debugProcess;
    SPxRunProcess *m_userAPluginProcess;
    SPxRunProcess *m_sectorBlankProcess;
    SPxRunProcess *m_rangeBlankProcess;
    SPxRunProcess *m_thresholdProcess;
    SPxRunProcess *m_rangeRingsProcess;
    SPxRunProcess *m_debugPostProcess;
    SPxRunProcess *m_lastProcess;

    /* 'Normal' radar sources. */
    SPxTestGenerator *m_srcTest;
    SPxScenarioGenerator *m_srcScen;
    SPxRadarReplay *m_srcReplay;
    SPxNetworkReceive *m_srcNet;

    /* 'Multi-channel' radar sources. */
    SPxSyncCombineSrc *m_srcSyncCombine;
    SPxRadarStream *m_syncStreams[2];	/* Handles of feeding streams */
    SPxRunProcess *m_syncProcesses[2];	/* Handles of processes */
    unsigned int m_numSyncStreams;	/* Number of handles */

    /* User-supplied source. */
    SPxRadarSource *m_srcUser;		/* If installed with AddSource() */
    unsigned int m_srcUserNumber;	/* For SetSource() */
    unsigned int m_srcUserShouldFree;	/* Flag */


    /*
     * Private functions.
     */

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;		/* Destination for debug msgs */
    static SPxRadarDatabase *m_rdb;	/* For scenario generator */
    static char *m_FileReplayName;	/* Name of file for replay, if any */
    static SPxRadarStream *m_FirstStream;	/* Linked list handle */
}; /* SPxRadarStream */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_RADAR_STREAM_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
