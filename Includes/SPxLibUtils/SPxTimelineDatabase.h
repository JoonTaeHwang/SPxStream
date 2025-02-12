/*********************************************************************
 *
 * (c) Copyright 2014 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx SPxTimelineDatabase abstract class header.
 *
 * Revision Control:
 *   01/02/21 v1.32  SP 	Support change to GetImage().
 *
 * Previous Changes:
 *   03/01/20 1.31  SP 	Add GetNumRecords().
 *   22/02/19 1.30  SP 	Add IsRecordHidden().
 *   25/06/18 1.29  AGC	Correctly suppress cppcheck warning.
 *   12/04/17 1.28  AGC	Correct SPxThread forward declaration.
 *   07/12/16 1.27  SP 	Return decimal value from GetSessionDurationSecs()
 *                      and GetFileDurationSecs().
 *   08/07/16 1.26  SP 	Support info marker.
 *   08/04/16 1.25  SP 	Support link to SQL database.
 *   22/02/16 1.24  SP 	Add extra arg to SetSelectedRecID().
 *   04/02/16 1.23  SP 	Support get session by name.
 *   01/02/16 1.22  SP 	Use session directory for thumbnail cache.
 *                      Add image events.
 *   22/01/16 1.21  SP 	Use UINT8 for channel ID.
 *   10/12/15 1.20  SP 	Create private background thread.
 *   04/12/15 1.19  SP 	Further rework.
 *                      Add session events.
 *   01/12/15 1.18  SP  Make some protected functions public.
 *   16/11/15 1.17  SP  Move session code into SPxTimelineSession.
 *   04/11/15 1.16  SP  Improvements to image cache handling.
 *   17/06/15 1.15  SP  Remove SetTime().
 *   05/06/15 1.14  SP  Support session deletion.
 *                      Rename some functions.
 *   15/05/15 1.13  SP  Add AddImage().
 *   22/04/15 1.12  SP  Add delete all images.
 *   14/04/15 1.11  SP  Add GetSessionEndTime().
 *   26/03/15 1.10  SP  Add SetMarkerTime() stub.
 *   23/02/15 1.9   SP  Make NextRecordID a member variable.
 *   20/02/15 1.8   SP  Move sesion name functions here.
 *   16/02/15 1.7   SP  Move record/replay specific functions
 *                      to derived classes.
 *                      Move record structure to SPxPackets.h.
 *   05/02/15 1.6   SP  Change constructor args.
 *                      Combine stamp and channel records.
 *   26/01/15 1.5   SP  Change constructor.
 *                      Handle channel add/remove.
 *   07/01/15 1.4   SP  Include position in stamp records.
 *                      Support stamp record callback.
 *   11/12/14 1.3   SP  Different constructors for record and replay.
 *                      Support thumbnail images.
 *   24/11/14 1.2   SP  Further development.
 *   21/11/14 1.1   SP  Initial version.
 *
 *********************************************************************/

#ifndef _SPX_TIMELINE_DATABASE_H
#define _SPX_TIMELINE_DATABASE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxSysUtils.h"
#include "SPxLibUtils/SPxViewControl.h" /* For SPxLatLong_t */
#include "SPxLibUtils/SPxTimelineSession.h"
#include "SPxLibData/SPxPackets.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxThread;
class SPxCriticalSection;
class SPxChannelDatabase;
class SPxThread;
class SPxImage;
class SPxSQLDatabase;

class SPxTimelineDatabase : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Selection state. */
    typedef enum
    {
        STATE_SELECT,
        STATE_DESELECT,
        STATE_TOGGLE
    
    } State_t;

    /* Notification event type. */
    typedef enum
    {
        EVENT_SESSION_CREATE,     /* Session created. */
        EVENT_SESSION_DELETE,     /* Session deleted. */
        EVENT_SESSION_START,      /* Session replay/record started. */
        EVENT_SESSION_STOP,       /* Session replay/record stopped. */
        EVENT_RECORD_ADD,         /* Record added. */
        EVENT_RECORD_REMOVE,      /* Record removed. */
        EVENT_RECORD_SELECT_L1,   /* Record selected (single click). */
        EVENT_RECORD_SELECT_L2,   /* Record selected (double click). */
        EVENT_RECORD_DESELECT,    /* Record deselected. */
        EVENT_IMAGE_ADD           /* Thumbnail image added. */

    } EventType_t;

    /* Session event info. Used by:
     *  EVENT_SESSION_CREATE
     *  EVENT_SESSION_DELETE
     *  EVENT_SESSION_START
     *  EVENT_SESSION_STOP
     */
    typedef struct
    {
        SPxTimelineSession *session;
        
    } EventSessionInfo_t;

    /* Record event info. Used by:
     *  EVENT_RECORD_ADD
     *  EVENT_RECORD_REMOVE
     *  EVENT_RECORD_SELECT_L1
     *  EVENT_RECORD_SELECT_L2
     *  EVENT_RECORD_DESELECT
     */
    typedef struct
    {
        const SPxPacketRecord *record;

    } EventRecordInfo_t;

    /* Session event info. Used by:
     *  EVENT_IMAGE_ADD
     */
    typedef struct
    {
        /* Nothing at present. */
        
    } EventImageInfo_t;

    /* Union of all event info structures. */
    typedef union
    {
        EventSessionInfo_t sessionEvt;
        EventRecordInfo_t recordEvt;
        EventImageInfo_t imageEvt;
    
    } EventInfo_t;

    typedef struct
    {
        SPxTime_t time;     /* Event time. */
        EventType_t type;   /* Event type. */
        EventInfo_t info;   /* Event details. */

    } Event_t;

    /*
     * Public variables.
     */

    /*
     * Public static functions.
     */

    static int IsRecordHidden(const SPxPacketRecord *rec);

    /*
     * Public functions.
     */

    /* Constructors and destructor. */

    /* Constructor and destructor. */
    SPxTimelineDatabase(SPxChannelDatabase *channelDB);
    virtual ~SPxTimelineDatabase(void);

    /* General. */
    virtual SPxChannelDatabase *GetChannelDatabase(void) {return m_channelDB;}
    virtual int IsRecordNotReplay(void) { return m_isRecordNotReplay; }

    /* Set/get optional SQL database. */
    virtual SPxErrorCode SetSQLDatabase(SPxSQLDatabase *sqlDB);
    virtual SPxSQLDatabase *GetSQLDatabase(void) { return m_sqlDB; } 

    /* Session base directory. */
    virtual SPxErrorCode SetSessionBaseDir(const char *dirname);
    virtual SPxErrorCode GetSessionBaseDir(char *buf, unsigned int bufSize);

    /* Current session info. */
    virtual int IsSessionStopped(void)=0;
    virtual SPxErrorCode GetSessionName(char *buf,unsigned int bufSize);
    virtual SPxErrorCode GetSessionDir(char *buf, unsigned int bufSize);
    virtual SPxErrorCode GetSessionStartTime(SPxTime_t *time);
    virtual SPxErrorCode GetSessionEndTime(SPxTime_t *time,
                                           int allowCurrentTime=FALSE);
    virtual double GetSessionDurationSecs(void)=0;
    virtual SPxErrorCode GetSessionCurrentTime(SPxTime_t *time)=0;

    /* Iterate through sessions. */
    virtual SPxErrorCode IterateSessionsForward(SPxCallbackListFn_t fn,
                                                void *userArg);

    virtual SPxErrorCode IterateSessionsReverse(SPxCallbackListFn_t fn,
                                                void *userArg);

    /* Access to session objects. */
    virtual SPxErrorCode TestSessionName(const char *sessionName);
    virtual unsigned int GetNumSessions(void);
    virtual SPxTimelineSession *GetSessionAndLock(UINT16 sessionID,
                                                  int hideErrors=FALSE);
    virtual SPxTimelineSession *GetSessionAndLock(const char *sessionName,
                                                  int hideErrors=FALSE);
    virtual SPxTimelineSession *GetCurrentSessionAndLock(void);
    virtual SPxErrorCode UnlockSession(SPxTimelineSession *session);
    virtual SPxErrorCode DeleteSession(const char *sessionName);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode DeleteAllSessions(void);

    /* Add thumbnail images to current session. */
    virtual SPxErrorCode AddImage(SPxImage *image, 
                                  UINT8 chanID,
                                  const SPxTime_t *time,
                                  char *filenameBuf=NULL,
                                  unsigned int filenameBufSize=0);

    /* Get thumbnail image from any session. */
    virtual SPxErrorCode GetImage(SPxImage *image,
                                  UINT8 chanID,
                                  const SPxTime_t *startTime,
                                  const SPxTime_t *endTime,
                                  int getFirstImage,
                                  SPxTime_t *imageTime,
                                  UINT32 *sessionIDRtn);

    /* Files in current session. */
    virtual unsigned int GetNumFiles(void)=0;
    virtual SPxErrorCode GetFileStartTime(SPxTime_t *time)=0;
    virtual SPxErrorCode GetFileCurrentTime(SPxTime_t *time)=0;
    virtual SPxErrorCode GetFileEndTime(SPxTime_t *time)=0;
    virtual double GetFileDurationSecs(void);

    /* Add record to current session. */
    virtual SPxErrorCode AddRecord(SPxPacketRecord *rec);

    /* Access a record from all sessions. */
    virtual const SPxPacketRecord *GetRecordAndLock(UINT32 recID);
    virtual const SPxPacketRecord *GetNearestRecordAndLock(
                                  const SPxLatLong_t *ll, 
                                  double *distMetresRtn);
    virtual const SPxPacketRecord *GetNearestRecordAndLock(
                                  const SPxTime_t *time, 
                                  double *distSecsRtn,
                                  int chanIDFilter=-1);
    virtual SPxErrorCode UnlockRecord(const SPxPacketRecord *rec);

    /* Record selection across all sessions. */
    virtual SPxErrorCode SetSelectedRecID(UINT32 recID, State_t state,
                                          int isDoubleClick=FALSE,
                                          int reportEvents=TRUE);
    virtual UINT32 GetSelectedRecID(void) { return m_selectedRecID; }

    /* Iterate through records for all sessions. */
    virtual unsigned int GetNumRecords(int chanIdFilter1=-1,
                                       UINT32 typesFilter1=0,
                                       int chanIdFilter2=-1,
                                       UINT32 typesFilter2=0);

    virtual SPxErrorCode IterateRecordsForward(SPxCallbackListFn_t fn,
                                               void *userArg,
                                               int chanIdFilter1=-1,
                                               UINT32 typesFilter1=0,
                                               int chanIdFilter2=-1,
                                               UINT32 typesFilter2=0);

    virtual SPxErrorCode IterateRecordsReverse(SPxCallbackListFn_t fn,
                                               void *userArg,
                                               int chanIdFilter1=-1,
                                               UINT32 typesFilter1=0,
                                               int chanIdFilter2=-1,
                                               UINT32 typesFilter2=0);

    /* Event callbacks. */
    virtual SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, 
                                          void *userObj);
    virtual SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn,
                                             void *userObj);

    /* Event reporting. */
    virtual SPxErrorCode ReportEvent(Event_t *evt);

    /* User-defined marker. */
    virtual SPxErrorCode SetInfoMarkerTime(const SPxTime_t *time,
                                           void *owner=NULL);
    virtual int GetInfoMarkerTime(SPxTime_t *time);
    virtual void *GetInfoMarkerOwner(void) { return m_infoMarkerOwner; }

protected:

    /*
     * Protected functions.
     */

    /* Session handling. */
    virtual SPxErrorCode createSession(const char *sessionName);

    /*
     * Protected variables.
     */

    /* General. */
    SPxChannelDatabase *m_channelDB;    /* The channel database. */
    UINT8 m_imageType;                  /* One of SPX_PACKET_IMAGE_TYPE_xxx */
    int m_isRecordNotReplay;            /* TRUE for recording, FALSE for replay. */
    SPxSQLDatabase *m_sqlDB;            /* Optional SQL database. */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /*
     * Protected static functions.
     */

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxThread *m_thread;            /* Background thread. */
    UINT16 m_nextSessionID;         /* Next unique session ID. */
    UINT32 m_selectedRecID;         /* Currently selected record. */
    int m_isInfoMarkerSet;          /* Is info marker set? */
    SPxTime_t m_infoMarkerTime;     /* Info marker time. */
    void *m_infoMarkerOwner;        /* Info marker owner. */

    /* Callback lists. */
    SPxCallbackList *m_eventCBList;
    SPxCriticalSection m_eventMutex;

    /*
     * Private functions.
     */

    /* General. */
    SPxTimelineSession *getSession(UINT32 sessionID);
    SPxTimelineSession *getSession(const char *name);
    SPxErrorCode deleteSessionInternal(SPxTimelineSession *session);
    double getMetresFromRecord(const SPxPacketRecord *rec, 
                               const SPxLatLong_t *ll);
    double getSecondsFromRecord(const SPxPacketRecord *rec, 
                                const SPxTime_t *time);

    /* Background processing function called from thread. */
    void doBackgroundProcessing(void);

    /*
     * Private static functions.
     */

    static void *backgroundThreadHandler(SPxThread *thread);

}; /* SPxTimelineDatabase */

#endif /* _SPX_TIMELINE_DATABASE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
