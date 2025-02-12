/*********************************************************************
 *
 * (c) Copyright 2015 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx SPxTimelineReplay class header.
 *
 * Revision Control:
 *   18/11/21 v1.24  SP 	Add IsExtractionInProgress().
 *
 * Previous Changes:
 *   24/01/20 1.23  SP 	Fix N/C #740 - High disk usage when
 *                     	session does not contain thumbnails.
 *   24/01/19 1.22  SP 	Add Skip() and other utility functions.
 *   25/06/18 1.21  AGC	Correctly suppress cppcheck warning.
 *   12/04/17 1.20  AGC	Correct SPxThread forward declaration.
 *   07/12/16 1.19  SP 	Return decimal value from 
 *                     	GetSessionDurationSecs().
 *                     	Add PeekSession().
 *   28/07/16 1.18  SP 	Use baseclass function to get SQL database filename.
 *   22/02/16 1.17  SP 	Add m_isEndOfReplay flag.
 *   22/01/16 1.16  SP 	Use UINT8 for channel ID.
 *   11/01/16 1.15  SP 	Create background thread.
 *   10/12/15 1.14  SP 	Create private thread for extraction.
 *   04/12/15 1.13  SP 	Further rework.
 *   16/11/15 1.12  SP 	Support changes to the base class.
 *                     	Control radar replay source directly.
 *                     	Support multiple thumbnail channels.
 *   04/11/15 1.11  SP 	Improve handling of new session.
 *   03/11/15 1.10  AGC	Packet decoder callback data now const.
 *   17/06/15 1.9   SP 	Fix mutex lock issues.
 *   05/06/15 1.8   SP 	Support session deletion.
 *                     	Add playback control functions.
 *   18/05/15 1.7   SP 	Separate record and image extraction.
 *   15/05/15 1.6   SP 	Remove unused packet handlers.
 *                     	Move file scanning here from RDR app.
 *   22/04/15 1.5   SP 	Support A/B markers.
 *   14/04/15 1.4   SP 	Add GetSessionEndTime().
 *   26/03/15 1.3   SP 	Add SetMarkerTime().
 *   20/02/15 1.2   SP 	Move sesion name functions to base class.
 *   16/02/15 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_TIMELINE_REPLAY_H
#define _SPX_TIMELINE_REPLAY_H

/* Other headers required. */
#include "SPxLibUtils/SPxTimelineDatabase.h"
#include "SPxLibData/SPxPacketDecoderFile.h"

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
class SPxRadarReplay;
class SPxThread;

class SPxTimelineReplay : public SPxTimelineDatabase
{
public:

    /*
     * Public types.
     */

    /* Entry in file list. */
    typedef struct
    {
        char basename[FILENAME_MAX]; /* File basename. */
        SPxTime_t startTime;         /* File start date/time. */
        SPxTime_t endTime;           /* File end date/time. */
        
    } FileInfo_t;

    /* Replay state. */
    typedef enum
    {
        REPLAY_STATE_STOP = 0,
        REPLAY_STATE_PAUSE,
        REPLAY_STATE_PLAY

    } ReplayState_t;

    /*
     * Public variables.
     */

    /*
     * Public static functions.
     */

    static SPxErrorCode PeekSession(const char *sessionDir,
                                    SPxTime_t *startTimeRtn,
                                    SPxTime_t *endTimeRtn);

    /*
     * Public functions.
     */

    /* Constructors and destructor. */
    SPxTimelineReplay(SPxChannelDatabase *channelDB,
                      SPxRadarReplay *replaySrc);
    virtual ~SPxTimelineReplay(void);

    /* Playback control. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode LoadSession(const char *sessionName);
    virtual int IsSessionLoaded(void);
    virtual int IsExtractionInProgress(void);
    virtual SPxErrorCode GenerateSQLDatabase(void);
    virtual double GetGenerateSQLDatabaseProgress(void);
    virtual SPxErrorCode SetReplayState(ReplayState_t state);
    virtual ReplayState_t GetReplayState(void);
    virtual SPxErrorCode Rewind(void);
    virtual int IsSessionStopped(void);
    virtual SPxErrorCode GotoTime(const SPxTime_t *time);
    virtual SPxErrorCode Skip(double secs);
    virtual SPxErrorCode SetAutoLoop(int enable);
    virtual int GetAutoLoop(void) const	{ return(m_autoLoop); }
    virtual SPxErrorCode SetSpeedupFactor(double factor);
    virtual double GetSpeedupFactor(void);

    /* Current session info. */
    virtual SPxErrorCode GetSessionCurrentTime(SPxTime_t *time);
    virtual double GetSessionDurationSecs(void);
    virtual SPxErrorCode GetSessionExtractionTime(SPxTime_t *time);

    /* Replay A/B markers. */
    virtual SPxErrorCode SetAMarkerTime(const SPxTime_t *time);
    virtual int GetAMarkerTime(SPxTime_t *time);
    virtual SPxErrorCode GetSessionStartOrAMarkerTime(SPxTime_t *time);
    virtual int IsSessionStartOrAMarkerTime(const SPxTime_t *time);
    virtual SPxErrorCode SetBMarkerTime(const SPxTime_t *time);
    virtual int GetBMarkerTime(SPxTime_t *time);
    virtual SPxErrorCode GetSessionEndOrBMarkerTime(SPxTime_t *time);
    virtual int IsSessionEndOrBMarkerTime(const SPxTime_t *time);

    /* Files in current session. */
    virtual SPxErrorCode LoadFile(const char *basename);
    virtual int IsFileLoaded(void);
    virtual unsigned int GetNumFiles(void);
    virtual SPxErrorCode GetFileStartTime(SPxTime_t *time);
    virtual SPxErrorCode GetFileCurrentTime(SPxTime_t *time);
    virtual SPxErrorCode GetFileEndTime(SPxTime_t *time);
    virtual void *GetFileListAndLock(void);
    virtual SPxErrorCode UnlockFileList(void *fileList);
    virtual int GetFirstFile(char *buf, unsigned int bufLen);
    virtual int GetCurrentFile(char *buf, unsigned int bufLen);
    virtual int GetLastFile(char *buf, unsigned int bufLen);
    virtual int GetFileAtTime(const SPxTime_t *time, 
                              char *buf, unsigned int bufLen);
    virtual int GetNextOrPrevFile(int getNext, char *buf, 
                                  unsigned int bufLen);
    virtual int GetFileIndex(const char *basename, int lockMutex);

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
    SPxThread *m_backgroundThread;      /* Background processing thread. */
    SPxThread *m_extractionThread;      /* Extraction processing thread. */
    int m_isExtractionThreadExiting;    /* Flag set when thread needs to exit. */
    int m_isExtractionInProgress;       /* If the initial extraction in progress. */
    SPxRadarReplay *m_replaySrc;        /* Replay replay source. */
    SPxPacketDecoderFile m_recDecoder;  /* Decoder used to extract records. */
    SPxPacketDecoderFile m_imgDecoder;  /* Decoder used to extract images. */
    UINT32 m_lastFileScanMsecs;         /* Last file scan time. */
    int m_isSessionChanged;             /* Flag set when session changed. */
    SPxTime_t m_lastRecExtractionTime;  /* Time of last record extraction. */
    SPxTime_t m_lastImgExtractionTime;  /* Time of last image extraction. */
    unsigned int m_numImgsExtracted;    /* Number of images extracted from a session. */
    int m_imgExtractionDisabled;        /* Suspend image extraction for current session. */
    int m_autoLoop;                     /* Should we automatically loop? */
    int m_isAMarkerSet;                 /* Is A marker set? */
    SPxTime_t m_aMarkerTime;            /* A marker time. */
    int m_isBMarkerSet;                 /* Is B marker set? */
    SPxTime_t m_bMarkerTime;            /* B marker time. */
    int m_isEndOfReplay;                /* Flag set when end of replay reached. */

    /*
     * Private functions.
     */

    /* Scanning for session files. */
    void scanForFiles(int forceUpdate);

    /* Extracting records and images. */
    void extractRecords(int startNew);
    void extractImages(int startNew);
    void addImageToCache(SPxTime_t *timestamp,
                         UINT8 chanIndex,
                         const unsigned char *payload,
                         unsigned int numBytes);
    void addRecordToDatabase(const unsigned char *payload,
                             unsigned int numBytes);
    SPxErrorCode getFilename(const char *basename,
                             char *buf, unsigned int bufLen);

    /* Background processing function called from thread. */
    virtual void doBackgroundProcessing(void);

    /* Extraction processing function called from thread. */
    virtual void doExtractionProcessing(void);

    /*
     * Private static functions.
     */

    static void *backgroundThreadHandler(SPxThread *thread);

    static void *extractionThreadHandler(SPxThread *thread);

    static void packetHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *from,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    static void imageHandler(SPxPacketDecoder *decoder,
                             void *userArg,
                             UINT32 packetType,
                             struct sockaddr_in *from,
                             SPxTime_t *timestamp,
                             const unsigned char *payload,
                             unsigned int numBytes);

    static void recordHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *from,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    static int replaySrcDeletionHandler(void *invokingObj,
                                        void *userArg,
                                        void *notUsed);

}; /* SPxTimelineReplay */

#endif /* _SPX_TIMELINE_REPLAY_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
