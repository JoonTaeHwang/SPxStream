/*********************************************************************
*
* (c) Copyright 2019-2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotReceiver class, for receiving plots.
*
*
* Revision Control:
*   29/09/22 v1.8    AGC	Support pass through of sensor specific data.
*
* Previous Changes:
*   20/05/21 1.7    SP 	Make SetFullRevAtZero() virtual.
*   19/05/21 1.6    SP 	Add debugging support.
*   22/10/20 1.5    AGC	Implement SPxPlotSourceIface.
*			Support heading correction.
*			Support user adjustment.
*   14/10/20 1.4    AGC	Improve shutdown behaviour.
*   04/02/20 1.3    AJH	Add sector crossing from plots option.
*   27/06/19 1.2    AGC	Add full rev at zero option.
*   25/06/19 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_PLOT_RECEIVER_H
#define _SPX_PLOT_RECEIVER_H

/*
 * Other headers required.
 */
/* We need some library headers for common types, functions, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxPim.h"
#include "SPxLibData/SPxPlotSourceIface.h"
#include "SPxLibNet/SPxNetAddr.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debug flags. */
static const UINT32 SPX_PLOT_RECV_DEBUG_MESG   = 0x00000001;
static const UINT32 SPX_PLOT_RECV_DEBUG_DECODE = 0x00000002;

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

/* Forward declare classes we need. */
class SPxNavData;
class SPxTargetDB;
class SPxMHT;
class SPxPIM;
class SPxPacketDecoder;
class SPxPacketDecoderNet;
class SPxAsterixDecoder;
class SPxAsterixMsg;
struct SPxPacketTrackExtended_tag;
struct SPxPacketTrackerStatus_tag;
struct SPxPacketPlot_tag;
struct SPxBlob_tag;
struct SPxTime_tag;

/*
 * Define our class, derived from the generic base class.
 */
class SPxPlotReceiver : public SPxObj, public SPxPlotSourceIface
{
public:
    /*
     * Public members.
     */
    /* Constructor and destructor. */
    explicit SPxPlotReceiver(void);
    virtual ~SPxPlotReceiver(void);

    /* Functions to link this to other objects. */
    SPxErrorCode SetTargetDB(SPxTargetDB *db);
    SPxTargetDB *GetTargetDB(void) const	{ return(m_targetDB); }
    SPxErrorCode SetTracker(SPxMHT *tracker);
    SPxMHT *GetTracker(void) const		{ return(m_tracker); }
    SPxErrorCode SetNavData(SPxNavData *navData);
    SPxNavData *GetNavData(void) const		{ return(m_navData); }

    /* Functions to set up operating modes etc. */
    virtual SPxErrorCode SetType(unsigned int type);
    unsigned int GetType(void) const		{ return(m_type); }
    SPxErrorCode SetSectorLatency(unsigned int sectorLatency);
    unsigned int GetSectorLatency(void) const	{ return(m_sectorLatency); }
    virtual SPxErrorCode SetFullRevAtZero(int enable);
    int IsFullRevAtZero(void) const		{ return(m_fullRevAtZero); }
    SPxErrorCode SetAutoSector(int enable);
    int IsAutoSector(void) const		{ return( m_autoSector ); }
    SPxErrorCode SetAziRefMode(SPxAziRefMode aziRefMode);
    SPxAziRefMode GetAziRefMode(void) const	{ return(m_aziRefMode); }
    SPxErrorCode SetAziAdjustDegs(double aziAdjustDegs);
    double GetAziAdjustDegs(void) const		{ return(m_aziAdjustDegs); }

    /* Information retrieval functions. */
    double GetAveragePeriod(void) const		{ return(m_averagePeriodSecs); }
    double GetAntennaAngleDegs(void) const;
    UINT32 GetLastUpdateTime(void) const	{ return(m_lastUpdateTime); }

    /* Plot source interface. */
    virtual int AddsPlotsToDatabase(void) const { return TRUE; }
    virtual SPxErrorCode AddPlotCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode RemovePlotCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode AddStatusCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode RemoveStatusCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Generic parameter assignment. */
    int SetParameter(char *name, char *value);
    int GetParameter(char *name, char *valueBuf, int bufLen);

    /* Debug. */
    void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    FILE *GetLogFile(void)		{ return(m_logFile); }
    void SetDebug(UINT32 debug)		{ m_debug = debug; }
    UINT32 GetDebug(void)		{ return(m_debug); }

protected:
    /*
     * Protected members.
     */
    /* Callbacks. */
    static void asterixTrackFnStatic(const SPxAsterixDecoder *decoder,
                                     void *userArg,
                                     struct SPxPacketTrackExtended_tag *track,
                                     const SPxAsterixMsg *msg);
    void asterixTrackFn(struct SPxPacketTrackExtended_tag *track,
                        const SPxAsterixMsg *msg);
    static void asterixStatusFnStatic(const SPxAsterixDecoder *decoder,
                                      void *userArg,
                                      struct SPxPacketTrackerStatus_tag *status,
                                      const SPxAsterixMsg *msg);
    void asterixStatusFn(struct SPxPacketTrackerStatus_tag *status,
                         const SPxAsterixMsg *msg);

    /* Functions to process specific messages. */
    SPxErrorCode processPlot(struct SPxPacketPlot_tag *plot,
                             struct SPxTime_tag *timestamp,
                             const UINT8 *sensorSpecificData=NULL,
                             UINT8 sensorSpecificDataLen=0);
    SPxErrorCode processBlob(struct SPxBlob_tag *blob);
    SPxErrorCode processAntennaUpdate(unsigned int sector);
    void writeDebug(UINT32 flags, const char *format, ...) const;

private:
    /*
     * Private variables.
     */
    mutable SPxCriticalSection m_mutex;	/* Mutex to protect internals */

    /* Linked objects. */
    SPxTargetDB *m_targetDB;		/* Target database to put plots into */
    SPxMHT *m_tracker;			/* Tracker to drive around */
    SPxNavData *m_navData;		/* Navigation data input. */

    /* Configuration options. */
    unsigned int m_type;		/* Type of receiver */
    unsigned int m_sectorLatency;	/* Num sectors to defer tracking */
    int m_fullRevAtZero;		/* Drive full revolution at zero pos? */
    int m_autoSector;			/* Generate sector crossings from plots? */
    SPxAziRefMode m_aziRefMode;		/* The mode for azimuth referencing */
    double m_aziAdjustDegs;		/* Azimuth adjustment. */

    /* Status information. */
    unsigned int m_currentSector;	/* Sector number of antenna position */
    double m_averagePeriodSecs;		/* Average period. */
    UINT32 m_sectorZeroTimestamp;	/* Last time for sector zero. */
    UINT32 m_lastUpdateTime;		/* Last time a message was received. */

    /* Debug. */
    FILE* m_logFile;			/* Log file for debug output. */
    UINT32 m_debug;			/* Debug flags. */

    /* Callbacks. */
    SPxAutoPtr<SPxCallbackList> m_plotCallback;
    SPxAutoPtr<SPxCallbackList> m_plotStatusCallback;

    SPxErrorCode applyRef(struct SPxBlob_tag *blob,
                          unsigned int *sector) const;
    double getTotalAziAdjustDegs(void) const;
    void reportSector(unsigned int sector);
    static int targetDBDeletedFn(void *invokingObject,
                                 void *userObject,
                                 void *arg);
    static int trackerDeletedFn(void *invokingObject,
                                void *userObject,
                                void *arg);

}; /* SPxPlotReceiver */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_PLOT_RECEIVER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
