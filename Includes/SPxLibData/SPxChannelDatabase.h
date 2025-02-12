/*********************************************************************
 *
 * (c) Copyright 2014 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannelDatabase class header.
 *
 * Revision Control:
 *   23/02/21 v1.41  SP 	Add mutexAlreadyLocked flag to GetNumChannels().
 *
 * Previous Changes:
 *   01/02/21 1.40  SP 	Add merge config.
 *   06/11/20 1.39  SP 	Add wasIdle to EventChannelStatusInfo_t.
 *   28/08/20 1.38  AGC	Remove unused include.
 *   09/06/20 1.37  SP 	Add SetAllOutputsEnabled().
 *   02/01/20 1.36  SP 	Add stampID to channel event.
 *   12/11/19 1.35  SP 	Support extended heartbeats.
 *   27/02/19 1.34  SP 	Fix line endings.
 *   20/02/19 1.33  SP 	Report activity state in EventChannelStatusInfo_t.
 *   25/06/18 1.32  AGC	Correctly suppress cppcheck warnings.
 *   03/08/17 1.31  SP 	Allow status updates to be disabled.
 *   21/09/16 1.30  SP 	Add SetOutputOnlyHint().
 *                     	Remove isOutputOnly flag from Load().
 *   30/03/16 1.29  SP 	Add Set/GetSQLDatabase().
 *   03/03/16 1.28  SP 	Support multiple channels with same file ID.
 *   04/02/16 1.27  SP 	Add updateNextChannelID().
 *   01/02/16 1.26  SP 	Support default lat/long.
 *   22/01/16 1.25  SP 	Support configurable fileID.
 *   17/11/15 1.24  SP 	Add optional type filter to GetNumChannels().
 *   16/11/15 1.23  SP 	Improvements to channel locking.
 *   03/11/15 1.22  AGC	Packet decoder callback data now const.
 *   28/10/15 1.21  SP 	Changes to way recording process is installed.
 *   14/10/15 1.20  SP 	Fix header formatting.
 *   01/10/15 1.19  SP 	Use renamed classes.
 *                     	Add virtual to some functions.
 *   05/06/15 1.18  SP 	Add optional argument to Load().
 *   21/05/15 1.17  SP 	Add GetBytesPerSec().
 *   18/05/15 1.16  SP 	Synchronise channel status reporting.
 *   05/05/15 1.15  SP 	Rename event as EVENT_CHANNEL_STATUS.
 *   29/04/15 1.14  SP 	Add channel filter args to Save().
 *   26/03/15 1.13  SP 	Remove DISPLAY_NAME_CHANGE and LINK_CHANGE
 *                     	events and add INFO event.
 *                     	Support channel output selection.
 *                     	Support AIS and ADS-B channels.
 *   06/03/15 1.12  SP 	Support TYPE_RAW channel.
 *   26/02/15 1.11  SP 	Remove lock arg from ReportLinkChangeEvent().
 *   23/02/15 1.10  SP 	Add replay buffer.
 *   20/02/15 1.9   SP 	Add GetChannelNoLock().
 *   16/02/15 1.8   SP 	Support channel linking.
 *                     	Remove event mutex.
 *   05/02/15 1.7   SP 	Add optional text to event messages.
 *                     	Remove link to stamp database.
 *                     	Support saving/loading from config.
 *                     	Support replay from file source.
 *   26/01/15 1.6   SP 	Add link to stamp database.
 *                     	Add typeFilters to Iterate..() functions.
 *   07/01/15 1.5   SP 	Changes to status event.
 *   21/11/14 1.4   SP 	Further development.
 *   14/10/14 1.3   REW	Fix icc trailing-comma enum warning.
 *   14/10/14 1.2   SP 	Further development.
 *   24/09/14 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CHANNEL_DATABASE_H
#define _SPX_CHANNEL_DATABASE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibData/SPxChannel.h"

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
class SPxSQLDatabase;
class SPxExtHeartbeatSender;

class SPxChannelDatabase : public SPxObj
{
    /* Grant SPxChannel private access. */
    friend class SPxChannel;

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
        EVENT_CHANNEL_SELECT,             /* Channel selected or deselected.*/
        EVENT_CHANNEL_ADD,                /* Channel added. */
        EVENT_CHANNEL_REMOVE,             /* Channel removed. */
        EVENT_CHANNEL_INFO,               /* Channel information. */
        EVENT_CHANNEL_STATUS              /* Channel status notification. */

    } EventType_t;

    /* Channel event info. Used by:
     *  EVENT_CHANNEL_SELECT
     *  EVENT_CHANNEL_ADD
     *  EVENT_CHANNEL_REMOVE
     *  EVENT_CHANNEL_INFO
     */
    typedef struct
    {
        SPxChannel *channel;    /* Channel handle. */
        unsigned int stampID;   /* System stamp ID. */

    } EventChannelInfo_t;

    /* Channel status event info. Used by:
     *  EVENT_CHANNEL_STATUS
     */
    typedef struct
    {
        SPxChannel *channel;     /* Channel handle. */
        SPxStatus_t oldStatus;   /* Previous channel status. */
        SPxStatus_t newStatus;   /* New channel status. */
        int wasIdle;             /* Was the channel previously idle? */
        int isIdle;              /* Is the channel now idle? */

    } EventChannelStatusInfo_t;

    /* Union of all event info structures. */
    typedef union
    {
        EventChannelInfo_t channelEvt;
        EventChannelStatusInfo_t channelStatusEvt;

    } EventInfo_t;

    /* Top-level event structure. Use type to determine the event type 
     * and then access one (and only one) of the event info structures 
     * that follow.
     */
    typedef struct
    {
        SPxTime_t time;     /* Event time. */
        EventType_t type;   /* Event type. */
        EventInfo_t info;   /* Event info. */
        char text[128];     /* Optional text. */

    } Event_t;

    /* Map of merged channels. */
    typedef struct
    {
        unsigned int fileIndex; /* Zero-based file index. */
        UINT8 oldChanID;        /* Channel ID before merge. */
        UINT8 oldFileID;        /* File ID before merge. */
        char oldName[256];      /* Display name before merge. */
        UINT8 newChanID;        /* Channel ID after merge. */
        UINT8 newFileID;        /* File ID after merge. */
        char newName[256];      /* Display name after merge. */

    } MergedChannel_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxChannelDatabase(void);
    virtual ~SPxChannelDatabase(void);

    /* Channel event callbacks. */
    virtual SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, 
                                          void *userObj);
    virtual SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, 
                                             void *userObj);

    /* Get name to assign to a channel. */
    virtual void GetChannelName(char *buf, unsigned int bufSizeBytes,
                                SPxChannel::Type_t type, UINT8 id=0);

    /* Create a new channel. */
    virtual SPxChannel *CreateChannel(SPxErrorCode *errRtn,
                                      SPxChannel::Type_t type,
                                      SPxChannel::Media_t inputMedia,
                                      SPxChannel::Media_t outputMedias,
                                      UINT8 id=0,
                                      UINT8 fileID=0,
                                      int inputIndex=-1,
                                      int outputIndex=-1);

    /* Access a channel. */
    virtual unsigned int GetNumChannels(SPxChannel::Type_t typeFilters=SPxChannel::TYPE_NONE,
                                        int mutexAlreadyLocked=FALSE);
    virtual SPxChannel *GetChannelNoLock(UINT8 id, int reportErr=TRUE);
    virtual SPxChannel *GetChannelAndLock(UINT8 id, int reportErr=TRUE);
    virtual SPxChannel *GetSelectedChannelAndLock(void);
    virtual SPxErrorCode UnlockChannel(SPxChannel *channel);

    /* Iterate through channels. */
    virtual SPxErrorCode IterateForward(SPxCallbackListFn_t fn, 
                      void *userArg,
                      SPxChannel::Type_t typeFilters=SPxChannel::TYPE_NONE);
    virtual SPxErrorCode IterateReverse(SPxCallbackListFn_t fn, 
                      void *userArg,
                      SPxChannel::Type_t typeFilters=SPxChannel::TYPE_NONE);

    /* Select a channel. */
    virtual SPxErrorCode SetSelectedChannel(SPxChannel *channel, 
                                            State_t state);

    /* Clear the database. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode DeleteAllChannels(void);

    /* Get overall status of all channels. */
    virtual SPxStatus_t GetStatus(void);

    /* Get overall data rate of all channels. */
    virtual UINT32 GetBytesPerSec(void);

    /* Event reporting. */
    virtual SPxErrorCode ReportEvent(Event_t *evt);

    /* Status update. */
    virtual SPxErrorCode SetStatusUpdateEnabled(int isEnabled);
    virtual int IsStatusUpdateEnabled(void) { return m_isStatusUpdateEnabled; }

    /* Set/get output only hint. */
    virtual SPxErrorCode SetOutputOnlyHint(int isEnabled);
    virtual int IsOutputOnlyHint(void) { return m_isOutputOnlyHint; }

    /* Set/get recording process. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetRecordingProcess(SPxRunProcess *recPro);
    virtual SPxRunProcess *GetRecordingProcess(void) { return m_recPro; }

    /* Set/get SQL database. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetSQLDatabase(SPxSQLDatabase *sqlDB);
    virtual SPxSQLDatabase *GetSQLDatabase(void) { return m_sqlDB; }

    /* Set/get extended heartbeat sender. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetExtHeartbeatSender(SPxExtHeartbeatSender *extHBSender,
                                               const char *streamSuffix);
    virtual SPxExtHeartbeatSender *GetExtHeartbeatSender(void) { return m_extHBSender; }
    virtual const char *GetExtHeartbeatStreamSuffix(void);

    /* Set/get file replay source. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetReplaySource(SPxRadarReplay *fileSrc,
                                         int isDefaultReplay=FALSE);
    virtual SPxRadarReplay *GetReplaySource(void) { return m_fileSrc; }
    virtual int IsDefaultReplayNotLive(void) { return m_isDefaultReplayNotLive; }

    /* Config load/save. */
    virtual SPxErrorCode Load(const char *filename);
    virtual SPxErrorCode Save(const char *filename, 
                              UINT8 *incChanIDs=NULL,
                              unsigned int numIncChanIDs=0);
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

    /* Conversion between file ID and channel ID. */
    virtual SPxErrorCode GetChanIDFromFileID(UINT8 fileID, SPxChannel::Type_t type, UINT8 *chanID);
    virtual SPxErrorCode GetFileIDFromChanID(UINT8 chanID, UINT8 *fileID);

    /* Default channel position. */
    virtual SPxErrorCode SetDefaultLatLong(double latDegs, double longDegs);
    virtual SPxErrorCode GetDefaultLatLong(double *latDegs, double *longDegs);
    virtual int IsDefaultLatLongSet(void) { return m_isDefaultLatLongSet; }

    /* Channel output control. */
    virtual SPxErrorCode SetAllOutputsEnabled(int isEnabled);

    /*
     * Public static functions.
     */

    static SPxErrorCode MergeConfig(const char *dbName,
                                    const char *filenameA,
                                    const char *filenameB,
                                    const char *mergedFilename,
                                    MergedChannel_t *chansRtn[],
                                    unsigned int *numChansRtn);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Background processing function called from thread. */
    virtual void doBackgroundProcessing(void);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Next channel ID to allocate (zero is invalid). */
    UINT8 m_nextChannelID;

    /* Event callback list. */
    SPxCallbackList *m_eventCBList;

    /* Selected channel. */
    SPxChannel *m_selectedChannel;

    /* Recording process. */
    SPxRunProcess *m_recPro;

    /* SQL database. */
    SPxSQLDatabase *m_sqlDB;

    /* Extended heartbeat sender. */
    SPxExtHeartbeatSender *m_extHBSender;

    /* File replay source. */
    SPxRadarReplay *m_fileSrc;
    int m_isDefaultReplayNotLive;

    /* Background thread. */
    SPxThread *m_thread;

    /* Time of last config packet. */
    UINT32 m_lastConfigPacketTimeMsecs;

    /* Replay packet buffer. */
    UINT8 *m_replayBuf;
    unsigned int m_replayBufSizeBytes;

    /* Last time status was reported. */
    UINT32 m_lastStatusReportMs;

    /* Is default lat/long set? */
    double m_defaultLatDegs;        /* Default channel latitude. */
    double m_defaultLongDegs;       /* Default channel longitude. */
    int m_isDefaultLatLongSet;      /* Is default lat/long set? */

    /* Are channels in the database only used for output? */
    int m_isOutputOnlyHint;

    /* Status updating. */
    int m_isStatusUpdateEnabled;

    /*
     * Private functions.
     */

    /* Add a channel. */
    virtual SPxErrorCode addChannel(SPxChannel *channel, 
                                    SPxTime_t *timeRtn);
    virtual SPxErrorCode removeChannel(SPxChannel *channel,
                                       SPxTime_t *timeRtn);

    /* Get a channel. */
    SPxChannel *getChannelFromID(UINT8 id);
    SPxChannel *getChannelFromFileID(UINT8 fileID, SPxChannel::Type_t type);

    /* Event reporting. */
    virtual SPxErrorCode reportEventInternal(Event_t *evt);
    virtual SPxErrorCode setSelectedChannelInternal(SPxChannel *channel,
                                                    State_t state);

    /* Channel creation. */
    SPxChannel *createRawChannel(SPxErrorCode *errRtn,
                                 SPxChannel::Media_t inputMedia,
                                 SPxChannel::Media_t outputMedias,
                                 UINT8 id,
                                 UINT8 fileID);

    SPxChannel *createRadarChannel(SPxErrorCode *errRtn,
                                   SPxChannel::Media_t inputMedia,
                                   SPxChannel::Media_t outputMedias,
                                   UINT8 id,
                                   UINT8 fileID,
                                   int ricIndex,
                                   int rocIndex);
    
    SPxChannel *createRadarNetChannel(SPxErrorCode *errRtn,
                                      SPxChannel::Media_t inputMedia,
                                      SPxChannel::Media_t outputMedias,
                                      UINT8 id,
                                      UINT8 fileID,
                                      int rocIndex);
    
    SPxChannel *createRadarHPxChannel(SPxErrorCode *errRtn,
                                      SPxChannel::Media_t inputMedia,
                                      SPxChannel::Media_t outputMedias,
                                      UINT8 id,
                                      UINT8 fileID,
                                      int ricIndex,
                                      int rocIndex);
    
    SPxChannel *createNavDataChannel(SPxErrorCode *errRtn,
                                     SPxChannel::Media_t inputMedia,
                                     SPxChannel::Media_t outputMedias,
                                     UINT8 id,
                                     UINT8 fileID);
    
    SPxChannel *createTracksChannel(SPxErrorCode *errRtn,
                                    SPxChannel::Media_t inputMedia,
                                    SPxChannel::Media_t outputMedias,
                                    UINT8 id,
                                    UINT8 fileID);
    
    SPxChannel *createAISChannel(SPxErrorCode *errRtn,
                                 SPxChannel::Media_t inputMedia,
                                 SPxChannel::Media_t outputMedias,
                                 UINT8 id,
                                 UINT8 fileID);

    SPxChannel *createADSBChannel(SPxErrorCode *errRtn,
                                  SPxChannel::Media_t inputMedia,
                                  SPxChannel::Media_t outputMedias,
                                  UINT8 id,
                                  UINT8 fileID);

    /* Config reporting. */
    void writeConfigPacket(void);

    /* Replay packet handling. */
    void forwardReplayPacket(UINT8 fileID,
                             UINT32 packetType,
                             SPxTime_t *time,
                             const unsigned char *payload,
                             unsigned int payloadSize);

    /* Channel filtering in Save(). */
    int isChannelIncluded(SPxChannel *chan,
                          UINT8 *incChanIDs,
                          unsigned int numIncChanIDs);

    void updateNextChannelID(void);

    /*
     * Private static functions.
     */

    /* Used to handle some file replay packets. */
    static void handleReplayPacket(SPxPacketDecoder *pktDecoder,
                                   void *userArg,
                                   UINT32 packetType,
                                   struct sockaddr_in *from,
                                   SPxTime_t *time,
                                   const unsigned char *payload,
                                   unsigned int payloadSize);

    /* Used to remove link if file source is deleted. */
    static int fileSrcDeletionHandler(void *invokingObj,
                                      void *userArg,
                                      void *notUsed);

    /* Used to remove link if recording process is deleted. */
    static int recProDeletionHandler(void *invokingObj,
                                     void *userArg,
                                     void *notUsed);

    /* Used to remove link if SQL database is deleted. */
    static int sqlDBDeletionHandler(void *invokingObj,
                                    void *userArg,
                                    void *notUsed);

    /* Used to remove link if extended heartbeat sender is deleted. */
    static int extHBSenderDeletionHandler(void *invokingObj,
                                          void *userArg,
                                          void *notUsed);

    /* Background thread function. */
    static void *backgroundThreadHandler(SPxThread *thread);
    
}; /* SPxChannelDatabase */

#endif /* _SPX_CHANNEL_DATABASE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
