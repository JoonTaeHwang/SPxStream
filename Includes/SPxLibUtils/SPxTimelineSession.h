/*********************************************************************
 *
 * (c) Copyright 2015 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxTimelineSession class header.
 *
 * Revision Control:
 *   01/02/21 v1.11  SP 	Support last image in GetImage().
 *
 * Previous Changes:
 *   03/01/20 1.10  SP 	Add GetNumRecords().
 *   25/06/18 1.9   AGC	Correctly suppress cppcheck warnings.
 *   12/04/17 1.8   AGC	Correct SPxThread forward declaration.
 *   28/07/16 1.7   SP 	Add getSQLDatabaseFilename().
 *   22/02/16 1.6   SP 	Add extra arg to SetSelectedRecID().
 *   01/02/16 1.5   SP 	Support user data.
 *   22/01/16 1.4   SP 	Use UINT8 for channel ID.
 *                      Add Set/GetUserData().
 *   04/12/15 1.3   SP 	Further rework.
 *   01/12/15 1.2   SP 	Make some protected functions public.
 *   16/11/15 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_TIMELINE_SESSION_H
#define _SPX_TIMELINE_SESSION_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxSysUtils.h"
#include "SPxLibUtils/SPxViewControl.h" /* For SPxLatLong_t */
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
class SPxImage;
class SPxTimelineDatabase;
class SPxChannelDatabase;

class SPxTimelineSession : public SPxObj
{
    /* Grant SPxTimelineDatabase and derived classes private access. */
    friend class SPxTimelineDatabase;
    friend class SPxTimelineRecorder;
    friend class SPxTimelineReplay;

public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Session info. */
    virtual SPxErrorCode GetSessionName(char *buf,unsigned int bufSize);
    virtual UINT16 GetID(void) { return m_sessionID; }
    virtual SPxErrorCode GetStartTime(SPxTime_t *time);
    virtual SPxErrorCode GetEndTime(SPxTime_t *time);

    /* Background processing. */
    virtual void DoBackgroundProcessing(void);

    /* Add, retrieve and delete thumbnail images. */
    virtual SPxErrorCode AddImage(SPxImage *image, 
                                  UINT8 chanID,
                                  const SPxTime_t *time,
                                  char *filenameBuf=NULL,
                                  unsigned int filenameBufSize=0);

    virtual SPxErrorCode GetImage(SPxImage *image,
                                  UINT8 chanID,
                                  const SPxTime_t *startTime,
                                  const SPxTime_t *endTime,
                                  int getFirstImage,
                                  SPxTime_t *imageTime=NULL);

    // cppcheck-suppress virtualCallInConstructor
    virtual void DeleteAllImages(void);

    /* Add and remove records. */
    virtual SPxErrorCode AddRecord(SPxPacketRecord *rec);
    virtual SPxErrorCode DeleteRecord(UINT32 recID);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode DeleteAllRecords(void);

    /* Access a record. */
    virtual const SPxPacketRecord *GetRecord(UINT32 recID);
    virtual const SPxPacketRecord *GetNearestRecord(const SPxLatLong_t *ll, 
                                                    double *distMetresRtn);
    virtual const SPxPacketRecord *GetNearestRecord(const SPxTime_t *time, 
                                                    double *distSecsRtn,
                                                    int chanIDFilter=-1);

    /* Iterate through records. */
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

    /* User data. */
    virtual SPxErrorCode SetUserData(void *obj, void *data);
    virtual void *GetUserData(void *obj);

protected:

    /*
     * Protected variables.
     */

    /* General. */
    SPxTimelineDatabase *m_timelineDB;  /* The timeline database. */
    UINT16 m_sessionID;                 /* ID for this session. */
    UINT16 m_nextRecordID;              /* ID of next record. */
    UINT8 m_imageType;                  /* One of SPX_PACKET_IMAGE_TYPE_xxx */
    
    /*
     * Protected functions.
     */

    /* Constructor and destructor. */
    SPxTimelineSession(SPxTimelineDatabase *timelineDB,
                       const char *sessionName,
                       UINT16 sessionID);
    virtual ~SPxTimelineSession(void);

    /* Configure session. */
    virtual SPxErrorCode setStartTime(SPxTime_t *time);
    virtual SPxErrorCode setEndTime(SPxTime_t *time);

    /*
     * Protected static functions.
     */

private:

    /*
     * Private types.
     */

    /* Used to store user data in an array. */
    typedef struct
    {
        void *obj;      /* Object that installed the data. */
        void *data;     /* The object's user data. */
        
    } UserItem_t;

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxTime_t m_startTime;
    SPxTime_t m_endTime;
    
    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode getImageBasename(const SPxTime_t *time,
                                  UINT8 chanID,
                                  char *buf,
                                  unsigned int bufSize);

    SPxErrorCode getImageFilename(const SPxTime_t *time,
                                  UINT8 chanID,
                                  char *buf,
                                  unsigned int bufSize);
    
    SPxErrorCode getImageCacheDir(char *buf, unsigned int bufSize);

    void cleanImageLists(void);

    SPxErrorCode getSQLDatabaseFilename(char *buf, unsigned int bufLen);

    const SPxPacketRecord *getRecordInternal(UINT32 recID);
    
    SPxErrorCode deleteRecordInternal(const SPxPacketRecord *rec);

    double getMetresFromRecord(const SPxPacketRecord *rec,
                               const SPxLatLong_t *ll);

    double getSecondsFromRecord(const SPxPacketRecord *rec,
                                const SPxTime_t *time);

    UserItem_t *getUserItem(void *obj);
    
    /*
     * Private static functions.
     */

}; /* SPxTimelineSession */

#endif /* _SPX_TIMELINE_SESSION_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
