/*********************************************************************
 *
 * (c) Copyright 2014 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannel class header.
 *
 * Revision Control:
 *   30/04/21 v1.57  SP 	Add Status parameter.
 *
 * Previous Changes:
 *   19/04/21 1.56  AJH	Add TimeSinceLastPacketMsec parameter.
 *   23/02/21 1.55  SP 	Support external parameters.
 *   01/02/21 1.54  SP 	Expose more config parameters.
 *   30/10/20 1.53  AGC	Reduce background thread sleep time.
 *   09/07/20 1.52  AGC	Support setting RTSP/Onvif source details.
 *   06/07/20 1.51  AGC	Support retrieving RTSP/Onvif source details.
 *   27/05/20 1.50  SP 	Add TYPE_META.
 *   10/12/19 1.49  SP 	Add TYPE_AV_ANY.
 *   12/11/19 1.48  SP 	Support extended heartbeats.
 *   28/06/19 1.47  SP 	Support audio MEDIA_NET_RTSP.
 *   25/06/19 1.46  SP 	Add SetOutputEnabledOrPending().
 *                      Add IsOutputEnabledOrPending().
 *   20/03/19 1.45  SP 	Further changes to support MEDIA_NET_RTP.
 *   12/03/19 1.44  BTB	Add MEDIA_NET_RTP.
 *   27/02/19 1.43  SP 	Add functions for getting device indicies.
 *   20/02/19 1.42  SP 	Support activity state.
 *   13/02/19 1.41  SP 	Add MEDIA_HW_DSOUND.
 *   06/02/19 1.40  SP 	Add TYPE_AUDIO.
 *   29/01/19 1.39  AGC	Support set/get saved output state.
 *   18/12/18 1.38  SP 	Correct MAX_NUM_MEDIAS again.
 *   03/10/18 1.37  SP 	Fix N/C #611 - Update MAX_NUM_MEDIAS.
 *   09/07/18 1.36  AGC	Add MEDIA_NET_ONVIF.
 *   07/03/18 1.35  SP 	Add MEDIA_NET_P217.
 *   03/08/17 1.34  SP 	Allow status updates to be disabled.
 *   13/07/17 1.33  SP 	Allow short status check hold-off period
 *                     	when channel is enabled.
 *   05/07/17 1.32  SP 	Remove GetLiveInputNetCtrlIf().
 *   12/04/17 1.31  AGC	Correct SPxThread forward declaration.
 *   21/09/16 1.30  SP 	Add IsOutputOnlyHint().
 *   16/09/16 1.29  SP 	Add TYPE_AGENT_SCREEN.
 *   23/08/16 1.28  SP 	Rename some medias and support replay medias.
 *                     	Add GetReplayMediaFromPacket().
 *   08/07/16 1.27  SP 	Add MEDIA_INPUT_ONLY mask.
 *   30/03/16 1.26  SP 	Store SQL database handle.
 *   01/02/16 1.25  SP 	Add parameter constants.
 *   22/01/16 1.24  SP 	Add IsDeviceIndexSupported().
 *                     	Support configurable file ID.
 *                     	Add GetDefaultNetOutputMedia().
 *                     	Change ID to UINT8 type.
 *   10/12/15 1.23  SP 	Add TYPE_VIDEO_ANY.
 *   17/11/15 1.22  SP 	Add channel type groups.
 *   03/11/15 1.21  AGC	Packet decoder callback data now const.
 *   28/10/15 1.20  SP 	Changes to way record and replay
 *                     	objects are installed.
 *   14/10/15 1.19  SP 	Add RemoveFromDatabase().
 *   01/10/15 1.18  SP 	Rename some medias.
 *                     	Add type and medias for TV video.
 *                     	Move some studs to source to avoid warnings
 *                     	when building SPxLibAV.
 *   10/07/15 1.17  SP 	Add serial sender.
 *   18/05/15 1.16  SP 	Synchronise channel status reporting.
 *   15/05/15 1.15  SP 	Remove writing of channel config packets.
 *   05/05/15 1.14  SP 	Support periodic channel status reporting.
 *   26/03/15 1.13  SP 	Support new media and channel types.
 *   06/03/15 1.12  SP 	Support inhibit/restore outputs.
 *                     	No longer need to report name change event.
 *   06/03/15 1.11  SP 	Rename MEDIA_NET_RAW as MEDIA_NET_UDP.
 *                     	Add TYPE_RAW and MEDIA_NET_TCP_RAW.
 *   26/02/15 1.10  SP 	Add IsLatLongSet().
 *                     	Add GetOutputInfo().
 *   20/02/15 1.9   SP 	Include packet B header in replay packets.
 *   16/02/15 1.8   SP 	Support channel linking.
 *   05/02/15 1.7   SP 	Remove link to stamp database.
 *                     	Allow ID to be set in the constructor.
 *   26/01/15 1.6   SP 	Implement GetNumOutputs() and GetOutputMedia()
 *                     	in this class.
 *   21/11/14 1.5   SP 	Further development.
 *   11/11/14 1.4   SP 	Remove unused m_userData.
 *   14/10/14 1.3   SP 	Fix ICC warnings.
 *   14/10/14 1.2   SP 	Further development.
 *   24/09/14 1.1   SP 	Initial version.
 * *********************************************************************/

#ifndef _SPX_CHANNEL_H
#define _SPX_CHANNEL_H

/* Other headers required. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxRib.h"
#include "SPxLibData/SPxPim.h"
#include "SPxLibData/SPxSerial.h"
#include "SPxLibNet/SPxNetAddr.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/* Name to use for channel objects. This will be prefixed with 
 * the database object name and followed by the channel ID.
*/
#define SPX_CHANNEL_OBJ_NAME            "Channel"

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
class SPxChannelDatabase;
class SPxThread;
class SPxSerialPort;
class SPxSQLDatabase;
class SPxExtHeartbeatSender;

class SPxChannel : public SPxObj, public SPxNetAddr, public SPxSerial
{
    /* Grant these classes private access. */
    friend class SPxChannelDatabase;
    friend class SPxAVChannelPeek;

public:

    /*
     * Public types.
     */

    /* Common parameter names. */
    static const char *PARAM_TYPE;
    static const char *PARAM_INPUT_MEDIA;
    static const char *PARAM_OUTPUT_MEDIAS;
    static const char *PARAM_INPUT_DEVICE_INDEX;
    static const char *PARAM_OUTPUT_DEVICE_INDEX;
    static const char *PARAM_FILE_ID;
    static const char *PARAM_DISPLAY_NAME;
    static const char *PARAM_LINKED_CHANNEL_ID;
    static const char *PARAM_LINKED_CHANNEL_FLAGS;
    static const char *PARAM_INPUT_ENABLED;
    static const char *PARAM_OUTPUTS_ENABLED;
    static const char *PARAM_IS_LAT_LONG_SET; 
    static const char *PARAM_LATITUDE_DEGS;
    static const char *PARAM_LONGITUDE_DEGS;
    static const char *PARAM_TIME_SINCE_LAST_PACKET_MSEC;
    static const char *PARAM_STATUS;

    /* Channel types. Do not change values as they are
     * used in configuration files. Be sure to update tables
     * in SPxChannel.cpp and SPxChannelDatabase::CreateChannel() 
     * when adding new types.
     */
    typedef UINT32 Type_t;

    /* No type - MUST BE ZERO. */
    static const Type_t TYPE_NONE = 0; 

    /* Raw data. */
    static const Type_t TYPE_RAW = 0x00000001;

    /* Radar video. */
    static const Type_t TYPE_RADAR = 0x00000002;

    /* Navigation data. */
    static const Type_t TYPE_NAV_DATA = 0x00000004;

    /* Track reports. */
    static const Type_t TYPE_TRACKS = 0x00000008;

    /* AIS. */
    static const Type_t TYPE_AIS = 0x00000010;

    /* ADSB. */
    static const Type_t TYPE_ADSB = 0x00000020;  

    /* TV video. */
    static const Type_t TYPE_VIDEO = 0x00000040;

    /* Screen capture via agent. */
    static const Type_t TYPE_AGENT_SCREEN = 0x00000080;

    /* Audio. */
    static const Type_t TYPE_AUDIO = 0x00000100;

    /* Meta data. */
    static const Type_t TYPE_META = 0x00000200;

    /* Number of types - KEEP THIS CONSISTENT WITH ABOVE. 
     * The number of types includes TYPE_NONE.
     */
    static const Type_t MAX_NUM_TYPES = 11;

    /* Flags used to match any track related channel type.
     * KEEP THIS CONSISTENT WITH ABOVE. 
     */
    static const Type_t TYPE_TRACKS_ANY = (TYPE_TRACKS |
                                           TYPE_AIS |
                                           TYPE_ADSB);

    /* Flags used to match any PPI related channel type.
     * KEEP THIS CONSISTENT WITH ABOVE. 
     */
    static const Type_t TYPE_PPI_ANY = (TYPE_RADAR |
                                        TYPE_NAV_DATA |
                                        TYPE_TRACKS_ANY);

    /* Flags used to match any video related channel type.
     * KEEP THIS CONSISTENT WITH ABOVE. 
     */
    static const Type_t TYPE_VIDEO_ANY = (TYPE_VIDEO |
                                          TYPE_AGENT_SCREEN);

    /* Flags used to match any audio related channel type.
     * KEEP THIS CONSISTENT WITH ABOVE. 
     */
    static const Type_t TYPE_AUDIO_ANY = (TYPE_AUDIO);

    /* Flags used to match any audio related channel type.
     * KEEP THIS CONSISTENT WITH ABOVE. 
     */
    static const Type_t TYPE_META_ANY = (TYPE_META);

    /* Flags used to match an AV related channel type. */
    static const Type_t TYPE_AV_ANY = (TYPE_VIDEO_ANY |
                                       TYPE_AUDIO_ANY |
                                       TYPE_META_ANY);
                                           
    /* Media types. Do not change values as they are
     * used in configuration files. Be sure to update tables
     * in SPxChannel.cpp when adding new types.
     */
    typedef UINT32 Media_t;

    /* No media - MUST BE ZERO. */
    static const Media_t MEDIA_NONE = 0;

    /* Hardware medias 0x0000001 - 0x00000800. */

    /* HPx/ROC. */
    static const Media_t MEDIA_HW_HPX = 0x00000001;

    /* Serial. */
    static const Media_t MEDIA_HW_SERIAL = 0x00000002;

    /* Direct Show video/audio capture device. */
    static const Media_t MEDIA_HW_DSHOW = 0x00000004;

    /* Direct Sound audio output. */
    static const Media_t MEDIA_HW_DSOUND = 0x00000008;

    /* Network medias 0x00001000 - 0x00800000. */

    /* Network RTP */
    static const Media_t MEDIA_NET_RTP = 0x00008000;

    /* Network UDP with no headers. */
    static const Media_t MEDIA_NET_UDP_RAW = 0x00010000;

    /* Network UDP with SPx headers. */
    static const Media_t MEDIA_NET_UDP_SPX = 0x00020000;

    /* Network UDP with ASTERIX headers. */
    static const Media_t MEDIA_NET_UDP_ASTERIX = 0x00040000;

    /* Network P226 (Simrad) radar video format. */
    static const Media_t MEDIA_NET_P226 = 0x00080000;

    /* Network TCP without headers. */
    static const Media_t MEDIA_NET_TCP_RAW = 0x00100000;

    /* Network RTSP. */
    static const Media_t MEDIA_NET_RTSP = 0x00200000;

    /* Network P217 radar video format. */
    static const Media_t MEDIA_NET_P217 = 0x00400000;

    /* Network Onvif. */
    static const Media_t MEDIA_NET_ONVIF = 0x00800000;

    /* Replay medias 0x01000000 - 0x80000000. */

    /* Data with no headers. */
    static const Media_t MEDIA_REPLAY_RAW = 0x01000000;

    /* Data with SPx headers. */
    static const Media_t MEDIA_REPLAY_SPX = 0x02000000;

    /* Data with ASTERIX headers. */
    static const Media_t MEDIA_REPLAY_ASTERIX = 0x04000000;

    /* Number of medias - KEEP THIS CONSISTENT WITH ABOVE.
     * The number of medias includes TYPE_NONE but not any
     * groups of medias such as MEDIA_NET_ANY.
     */
    static const Media_t MAX_NUM_MEDIAS = 17;

    /* Flags used to match any hardware media.
     * KEEP THIS CONSISTENT WITH ABOVE.
     *
     * Note that we purposely do not include MEDIA_HW_DSOUND 
     * as this is an output only device and needs to be handled
     * as a special case.
     */
    static const Media_t MEDIA_HW_ANY = (MEDIA_HW_HPX |
                                         MEDIA_HW_SERIAL |
                                         MEDIA_HW_DSHOW);

    /* Flags used to match any network media.
     * KEEP THIS CONSISTENT WITH ABOVE. 
     */
    static const Media_t MEDIA_NET_ANY = (MEDIA_NET_RTP |
                                          MEDIA_NET_UDP_RAW |
                                          MEDIA_NET_UDP_SPX | 
                                          MEDIA_NET_UDP_ASTERIX | 
                                          MEDIA_NET_P226 |
                                          MEDIA_NET_P217 |
                                          MEDIA_NET_TCP_RAW |
                                          MEDIA_NET_RTSP |
                                          MEDIA_NET_ONVIF);

    /* Flags used to match any live media.
     * KEEP THIS CONSISTENT WITH ABOVE. 
     */
    static const Media_t MEDIA_LIVE_ANY = (MEDIA_HW_ANY | 
                                           MEDIA_NET_ANY);

    /* Flags used to match any replay media.
     * KEEP THIS CONSISTENT WITH ABOVE. 
     */
    static const Media_t MEDIA_REPLAY_ANY = (MEDIA_REPLAY_RAW |
                                             MEDIA_REPLAY_SPX | 
                                             MEDIA_REPLAY_ASTERIX);

    /* Flags used to identify any medias which are input
     * only - i.e. are not available for output use.
     */
    static const Media_t MEDIA_INPUT_ONLY = (MEDIA_HW_DSHOW |
                                             MEDIA_NET_P226 |
                                             MEDIA_NET_P217);

    /* 
     * Channel link control flags. 
     */

    /* Sync lat/long. */
    static const UINT32 LINK_SYNC_LATLONG = 0x00000001; 

    /*
     * Public variables.
     */

    /*
     * Public static functions.
     */

    /* Static functions used to query channel types. */
    static Media_t GetTypeFromIndex(unsigned int index);
    static int IsTypeValid(Type_t type);
    static const char *GetTypeShortName(Type_t type);
    static const char *GetTypeLongName(Type_t type);
    static Type_t GetTypeFromPacket(UINT32 packetType);
    static Media_t GetReplayMediaFromPacket(UINT32 packetType,
                                            const unsigned char *payload=NULL,
                                            unsigned int numBytes=0);

    /* Static functions used to query channel medias. */
    static Media_t GetMediaFromIndex(unsigned int index);
    static int IsMediaValid(Media_t media);
    static const char *GetMediaName(Media_t media);
    static int IsMediaSupported(Type_t type, Media_t media);
    static int AreMediasSupported(Type_t type, Media_t medias, int matchAll);
    static Media_t GetDefaultNetOutputMedia(Type_t type);
    static Media_t GetBestNetOutputMedia(Type_t type, Media_t inputMedia);
    static int IsDeviceIndexSupported(Type_t type, Media_t media);

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxChannel(SPxChannelDatabase *channelDB, 
               UINT8 id,
               UINT8 fileID);
    SPxErrorCode Create(SPxChannel::Type_t type,
                        SPxChannel::Media_t inputMedia,
                        SPxChannel::Media_t outputMedias);
    virtual ~SPxChannel(void);

    /* Access. */
    virtual SPxChannelDatabase *GetChannelDB(void);
    virtual SPxStatus_t GetStatus(char *buf=NULL, 
                                  unsigned int bufSizeBytes=0);
    virtual int IsIdle(void) { return m_isIdle; }
    virtual void RequestStatusUpdate(int forceChange=FALSE);
    virtual UINT32 GetPacketsPerSec(void) const { return m_packetsPerSec; }
    virtual UINT32 GetBytesPerSec(void) const { return m_bytesPerSec; }
    virtual UINT32 GetLastPacketTime(void) { return m_lastDataPacketTime; }
    virtual UINT32 GetLastInputEnabledTime(void) { return m_lastInputEnabledTime; }

    /* General configuration. */
    virtual SPxErrorCode SetDisplayName(const char *name);
    virtual SPxErrorCode GetDisplayName(char *buf, unsigned int bufSizeBytes);
    virtual Type_t GetType(void) const { return m_type; }
    virtual UINT8 GetID(void) const { return m_id; }
    virtual UINT8 GetFileID(void) { return m_fileID; }

    /* Input configuration. */
    virtual Media_t GetInputMedia(void) const { return m_inputMedia; }
    virtual SPxErrorCode SetInputEnabled(int isEnabled);
    virtual int IsInputEnabled(void) const;
    virtual SPxErrorCode SetInputReplayNotLive(int isReplay);
    virtual int IsInputReplayNotLive(void) const { return m_isReplayNotLive; }

    /* Replay input configuration. */
    virtual int IsReplayInputAvail(void) const;

    /* Live input config functions implemented in a derived class. */
    virtual void GetLiveInputInfo(char *buf, 
                                  unsigned int bufSizeBytes) const=0;
    virtual int IsLiveInputAvail(void) const=0;
    virtual int GetLiveInputDeviceIndex(void) const { return -1; }
    
    /* Output configuration. */
    virtual Media_t GetOutputMedias(void) const { return m_outputMedias; }
    virtual int IsOutputOnlyHint(void) { return m_isOutputOnlyHint; }
    virtual unsigned int GetNumOutputs(void) const;
    virtual Media_t GetOutputMedia(unsigned int index) const;
    virtual SPxErrorCode SetAllOutputsEnabled(int isEnabled);
    virtual SPxErrorCode InhibitAllOutputs(void);
    virtual SPxErrorCode RestoreAllOutputs(void);
    virtual int AreOutputsInhibited(void) const { return m_areOutputsInhibited; }
    virtual SPxErrorCode SetOutputEnabledOrPending(unsigned int index, int isEnabled);
    virtual int IsOutputEnabledOrPending(unsigned int index) const;
    
    /* Output config functions implemented in a derived class. */
    virtual int IsOutputMediaAvail(Media_t media) const=0;
    virtual SPxErrorCode SetOutputEnabled(unsigned int index, int isEnabled)=0;
    virtual int IsOutputEnabled(unsigned int index) const=0;
    virtual void GetOutputInfo(unsigned int index,
                               char *buf, 
                               unsigned int bufSizeBytes) const=0;
    virtual SPxNetAddr *GetOutputNetCtrlIf(unsigned int index);
    virtual SPxSerial *GetOutputSerCtrlIf(unsigned int index);
    virtual int GetOutputDeviceIndex(void) const { return -1; }

    /* Channel linking. */
    virtual SPxErrorCode SetLinkedChannelID(UINT8 id, UINT32 flags);
    virtual UINT8 GetLinkedChannelID(void) { return m_linkedChanID; }
    virtual UINT32 GetLinkedChannelFlags(void) { return m_linkedChanFlags; }

    /* User data. */
    virtual SPxErrorCode SetUserData(void *obj, void *data);
    virtual void *GetUserData(void *obj);

    /* Message handler support. */
    virtual SPxErrorCode AddMesgHandler(SPxCallbackListFn_t fn, 
                                        void *userObj)
    {
        return (SPxErrorCode)m_mesgHandlerList->AddCallback(fn, userObj);
    }
    virtual SPxErrorCode RemoveMesgHandler(SPxCallbackListFn_t fn, 
                                           void *userObj)
    {
        return (SPxErrorCode)m_mesgHandlerList->RemoveCallback(fn, userObj);
    }

    /* Channel position. */
    virtual SPxErrorCode SetLatLong(double latDegs, double longDegs);
    virtual SPxErrorCode GetLatLong(double *latDegs, double *longDegs);
    virtual int IsLatLongSet(void) { return m_isLatLongSet; }

    /* Status reporting. */
    virtual SPxErrorCode SetStatusUpdateEnabled(int isEnabled);
    virtual int IsStatusUpdateEnabled(void) { return m_isStatusUpdateEnabled; }

    /* Config load/save. */
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

    /* Stubs for SPxNetAddr control interface functions. If required,
     * these should be implemented in a derived class.
     */
    virtual SPxErrorCode SetAddress(const char *address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;
    virtual SPxErrorCode SetRtspMedia(const char *rtspMedia);
    virtual SPxErrorCode GetRtspMedia(char *buf, unsigned int bufLen) const;
    virtual SPxErrorCode SetOnvifServer(const char *onvifServer);
    virtual SPxErrorCode GetOnvifServer(char *buf, unsigned int bufLen) const;
    virtual SPxErrorCode SetOnvifProfile(const char *onvifProfile);
    virtual SPxErrorCode GetOnvifProfile(char *buf, unsigned int bufLen) const;

    /* Stubs for SPxSerial control interface functions. If required,
     * these should be implemented in a derived class.
     */
    virtual SPxErrorCode SetSerialName(const char *name);
    virtual SPxErrorCode SetSerialBaud(unsigned int baud);
    virtual const char *GetSerialName(void) const;
    virtual unsigned int GetSerialBaud(void) const;

    /* Parameter handling. */
    virtual SPxErrorCode AddExternalParameter(const char *name, const char *defaultValue);
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected variables.
     */

    /* General. */
    SPxRunProcess *m_recPro;                /* Recording process. */
    SPxSQLDatabase *m_sqlDB;                /* SQL database. */
    SPxExtHeartbeatSender *m_extHBSender;   /* Extended heartbeat sender. */
    SPxRadarReplay *m_fileSrc;              /* Replay source. */
    SPxCallbackList *m_mesgHandlerList;     /* Handler for received messages. */
    SPxSerialPort *m_serialSender;          /* Serial sender. */
    SPxObj *m_extHbStreamObj;                    /* Network object object. */

    /*
     * Protected functions.
     */
    
    /* Add/remove channel to/from database (to be called in a derived class). */
    virtual SPxErrorCode AddToDatabase(void);
    virtual SPxErrorCode RemoveFromDatabase(void);

    /* Set the name of a channel sub-object. */
    virtual void SetSubObjectName(SPxObj *obj, const char *suffix);

    /* Live input config functions implemented in a derived class. */
    virtual SPxErrorCode SetLiveInputEnabled(int isEnabled)=0;
    virtual int IsLiveInputEnabled(void) const=0;

    /* Replay input config functions. */
    virtual SPxErrorCode SetReplayInputEnabled(int isEnabled);
    virtual int IsReplayInputEnabled(void) const;

    /* Saved output settings. */
    virtual SPxErrorCode SetOutputPending(unsigned int index, int isEnabled);
    virtual int IsOutputPending(unsigned int index) const;

    /* Report packet received for data rate calculation. */
    virtual void ReportPacketReceived(unsigned int numBytes);

    /* Check and get channel status implemented in a derived class. */
    virtual SPxStatus_t CheckStatus(char *mesgBuf, unsigned int mesgBufLen)=0;
    virtual int CheckIsIdle(void) { return FALSE; }

    /* Replay packet handler implemented in a derived class. */
    virtual void HandleReplayPacket(UINT16 packetType,
                                    const unsigned char *data,
                                    unsigned int dataSizeBytes)=0;

    /* Background processing function called from thread. */
    virtual UINT32 doBackgroundProcessing(void);
    virtual void stopBackgroundThread(void);

    /* Serial port handling. */
    virtual int CheckOpenSerialPort(void);
    virtual void CloseSerialPort(void);

    /* Heartbeat stream. */
    virtual void UpdateExtHeartbeatStreamName(void);

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

    /* Parent database. */
    SPxChannelDatabase *m_channelDB;   /* Parent database. */

    /* Channel settings. */
    UINT8 m_id;                     /* Unique ID for this channel. */
    UINT8 m_fileID;                 /* ID to use in recording files. */
    Type_t m_type;                  /* Channel type. */
    Media_t m_inputMedia;           /* Input media. */
    Media_t m_outputMedias;         /* Output medias. */
    int m_isAddedToDatabase;        /* Is channel added to the database? */
    int m_isReplayNotLive;          /* Replay or live input? */
    int m_isReplayEnabled;          /* Is replay input enabled? */
    UINT8 m_linkedChanID;           /* Linked channel ID. */
    UINT32 m_linkedChanFlags;       /* Linked channel control flags. */
    double m_latDegs;               /* Channel latitude. */
    double m_longDegs;              /* Channel longitude. */
    int m_isLatLongSet;             /* Is lat/long set? */
    SPxStatus_t m_status;           /* Channel status. */
    int m_isIdle;                   /* Is channel idle? */
    int m_requestStatusUpdate;      /* Set this flag to request status update. */
    int m_forceStatusChange;        /* In addition set this flag to force a status change. */
    int m_areOutputsInhibited;      /* Outputs inhibited? */
    int m_savedOutputEnables[MAX_NUM_MEDIAS]; /* Saved output enables. */
    int m_isOutputOnlyHint;         /* Is channel output only? */
    UINT32 m_lastInputEnabledTime;  /* Last time input was enabled. */
    int m_isStatusUpdateEnabled;    /* Status update enabled? */

    /* General. */
    SPxThread *m_thread;
    UINT32 m_packetsReceivedInInterval;
    UINT32 m_packetsPerSec;
    UINT32 m_bytesReceivedInInterval;
    UINT32 m_bytesPerSec;
    UINT32 m_lastStatusUpdateTime;
    UINT32 m_lastLinkedChanUpdateTime;
    UINT32 m_lastDataRateCalcTime;
    UINT32 m_lastDataPacketTime;
    UINT32 m_lastConfigPacketTimeMsecs; /* Time of last config packet. */
 
    /*
     * Private static variables.
     */

    /*
     * Private functions.
     */

    virtual SPxErrorCode setType(Type_t type);
    virtual SPxErrorCode setInputMedia(Media_t media);
    virtual SPxErrorCode setOutputMedias(Media_t medias);
    UserItem_t *getUserItem(void *obj);

    /*
     * Private static functions.
     */

    static void *backgroundThreadHandler(SPxThread *thread);
    static unsigned int getTypeInfoIndex(Type_t type);
    static unsigned int getMediaInfoIndex(Media_t type);

}; /* SPxChannel */

#endif /* _SPX_CHANNEL_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
