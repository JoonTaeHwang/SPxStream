/*********************************************************************
 *
 * (c) Copyright 2015 - 2016, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxTimelineRecorder.h,v $
 * ID: $Id: SPxTimelineRecorder.h,v 1.14 2016/12/07 15:15:14 rew Exp $
 *
 * Purpose:
 *   SPx SPxTimelineRecorder database class header.
 *
 * Revision Control:
 *   07/12/16 v1.14  SP 	Return decimal value from GetSessionDurationSecs().
 *
 * Previous Changes:
 *   01/02/16 1.13  SP 	Add mutex to AddImage().
 *   22/01/16 1.12  SP 	Use UINT8 for channel ID.
 *   10/12/15 1.11  SP 	Add GetSessionDurationSecs().
 *   04/12/15 1.10  SP 	Further rework.
 *   17/11/15 1.9   SP 	Add GetCurrentFile().
 *   16/11/15 1.8   SP  Support changes to the base class.
 *   05/06/15 1.7   SP  Rename some functions.
 *   15/05/15 1.6   SP 	Add extra args to AddImage().
 *   05/05/15 1.5   SP 	Support more robust channel status reporting.
 *   14/04/15 1.4   SP 	Add GetSessionEndTime().
 *   26/03/15 1.3   SP 	Support EVENT_CHANNEL_INFO.
 *   20/02/15 1.2   SP 	Rename some functions.
 *   16/02/15 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_TIMELINE_RECORDER_H
#define _SPX_TIMELINE_RECORDER_H

/* Other headers required. */
#include "SPxLibUtils/SPxTimelineDatabase.h"
#include "SPxLibUtils/SPxStampDatabase.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibData/SPxChannelDatabase.h"

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
class SPxRunProcess;

class SPxTimelineRecorder : public SPxTimelineDatabase
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructors and destructor. */
    SPxTimelineRecorder(SPxChannelDatabase *channelDB,
                         SPxStampDatabase *stampDB,
                         SPxRunProcess *recPro);
    virtual ~SPxTimelineRecorder(void);

    /* Session control. */
    virtual SPxErrorCode StartSession(const char *sessionName);
    virtual SPxErrorCode StopSession(void);
    virtual int IsSessionStopped(void);

    /* Session info. */
    virtual SPxErrorCode GetSessionCurrentTime(SPxTime_t *time);
    virtual double GetSessionDurationSecs(void);

    /* Files in current session. */
    virtual unsigned int GetNumFiles(void);
    virtual SPxErrorCode GetCurrentFile(char *buf, unsigned int bufLen);
    virtual SPxErrorCode GetFileStartTime(SPxTime_t *time);
    virtual SPxErrorCode GetFileCurrentTime(SPxTime_t *time);
    virtual SPxErrorCode GetFileEndTime(SPxTime_t *time);

    /* Override functions. */
    virtual SPxErrorCode AddRecord(SPxPacketRecord *rec);
    virtual SPxErrorCode AddImage(SPxImage *image, 
                                  UINT8 chanID,
                                  const SPxTime_t *time,
                                  char *filenameBuf=NULL,
                                  unsigned int filenameBufSize=0);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxStampDatabase *m_stampDB;
    SPxRunProcess *m_recPro;
    SPxCriticalSection m_imageBufMutex;
    UINT8 *m_imageBuf;
    unsigned int m_imageBufSizeBytes;
    SPxTime_t m_sessionEndTime;

    /*
     * Private functions.
     */

    /* Channel event handling. */
    void processChannelEvent(const SPxChannelDatabase::Event_t *evt);
    void onChannelAdd(const SPxTime_t *time,
                   SPxChannelDatabase::EventType_t type,
                   const SPxChannelDatabase::EventChannelInfo_t *info,
                   const char *text);
    void onChannelRemove(const SPxTime_t *time,
                   SPxChannelDatabase::EventType_t type,
                   const SPxChannelDatabase::EventChannelInfo_t *info,
                   const char *text);
    void onChannelInfo(const SPxTime_t *time,
                   SPxChannelDatabase::EventType_t type,
                   const SPxChannelDatabase::EventChannelInfo_t *info,
                   const char *text);
    void onChannelStatus(const SPxTime_t *time,
                         SPxChannelDatabase::EventType_t type,
                         const SPxChannelDatabase::EventChannelStatusInfo_t *info,
                         const char *text);

    /* Stamp event handling. */
    void processStampEvent(const SPxStampDatabase::Event_t *evt);
    void onStampLog(const SPxTime_t *time,
                   SPxStampDatabase::EventType_t type, 
                   const SPxStampDatabase::EventStampLogInfo_t *info,
                   const char *text);

    /*
     * Private static functions.
     */

    static int channelEventHandler(void *invokingObjPtr,
                                   void *userObjPtr,
                                   void *eventPtr);

    static int stampEventHandler(void *invokingObjPtr,
                                 void *userObjPtr,
                                 void *eventPtr);

    static int logChannelStatusIterator(void *channelDBVoid,
                                        void *objVoid,
                                        void *channelVoid);

}; /* SPxTimelineRecorder */

#endif /* _SPX_TIMELINE_RECORDER_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
