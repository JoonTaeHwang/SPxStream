/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSdbStream class.
 *
 * Revision Control:
 *   07/09/22 v1.49  SP 	Make interface address volatile.
 *
 * Previous Changes:
 *   08/08/22 1.48  AGC	Remove duplicate include.
 *   07/04/22 1.47  SP 	Simplify display names and show instance number.
 *   10/02/22 1.46  SP 	Add SUBTYPE_JSON.
 *   05/01/22 1.45  SP 	Make dist addr and port dynamic for RTSP.
 *   05/01/22 1.44  SP 	Remove ClearDesc().
 *   28/10/21 1.43  AGC	Add TYPE_MULTILATERATION.
 *                     	Add TYPE_WEATHER.
 *   02/06/21 1.42  SP 	Make detected sender ID and ASTERIX cat available.
 *   13/05/21 1.41  SP 	Add Combine().
 *   28/04/21 1.40  SP 	Add TYPE_VERSION.
 *   09/12/20 1.39  SP 	Add IsLocal().
 *   21/10/20 1.38  BTB	Add SUBTYPE_SGF.
 *   27/05/20 1.37  SP 	Add TYPE_META.
 *   19/05/20 1.36  SP 	Remove trailing comma.
 *   19/05/20 1.35  SP 	Rename TYPE_OTHER/SUBTYPE_OTHER to TYPE_GENERIC/SUBTYPE_GENERIC.
 *   13/03/20 1.34  SP 	Add comment for TYPE_OTHER and SUBTYPE_OTHER.
 *   12/03/20 1.33  SP 	Fix build warnings.
 *   12/03/20 1.32  SP 	Add GetNMEAType().
 *   09/03/20 1.31  SP 	Improve packet detection.
 *   09/03/20 1.30  SP 	Correct comments.
 *   06/03/20 1.29  SP 	Fix typo.
 *   05/03/20 1.28  SP 	Improve use of sender ID.
 *   04/03/20 1.27  SP 	Add GetSenders().
 *   20/02/20 1.26  SP 	Add TCP flag to descriptor.
 *   19/02/20 1.25  SP 	Add AppendHTML().
 *   07/02/20 1.24  SP 	Fix detection of RTP packets.
 *   06/02/20 1.23  SP 	Remove GetLastPacketTime().
 *   05/02/20 1.22  SP 	Remove IsLocal().
 *   28/01/20 1.21  SP 	Add IsLocal().
 *   20/01/20 1.20  SP 	Add IsDetected().
 *   09/12/19 1.19  SP 	Protect descriptor with mutex.
 *   06/12/19 1.18  SP 	Extract sender ID from received stream.
 *   04/12/19 1.17  SP 	Support JSON message handling in base class.
 *   22/11/19 1.16  SP 	Changes to descriptor mask.
 *   19/11/19 1.15  SP 	Support saving sitemap to config.
 *   19/11/19 1.14  SP 	Support saving sitemap to config.
 *   19/11/19 1.13  SP 	Disable packet detection by default.
 *   18/11/19 1.12  SP 	Support packet detection.
 *   15/11/19 1.11  SP 	Modify stream types.
 *   15/11/19 1.10  SP 	Don't make public functions virtual.
 *   14/11/19 1.9   SP 	Add GetPacketType().
 *   12/11/19 1.8   SP 	Changes to descriptor.
 *   08/11/19 1.7   SP 	Add interface address.
 *   07/11/19 1.6   SP 	Fix build warning.
 *   06/11/19 1.5   SP 	Improve operation of Compare().
 *   30/10/19 1.4   SP 	Further development.
 *   29/10/19 1.3   SP 	Support generation of JSON messages.
 *   25/10/19 1.2   SP 	Rename class and further development.
 *   24/10/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SDB_STREAM_H
#define _SPX_SDB_STREAM_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxSdbItem.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxTimer.h"

/*********************************************************************
 *
 *   Constants
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

/* Forward declare classes we use. */
class SPxSiteDatabase;
class SPxSdbApplication;
class SPxPacketDecoder;
class SPxPacketDecoderNet;
class SPxAsterixDecoder;
class SPxAsterixMsg;
class SPxNetAddr;
class SPxPIM;

/*
 *  SPxSdbStream class.
 */
class SPxSdbStream : public SPxSdbItem
{
public:
    /*
     * Public variables.
     */

    /*
     * Public types.
     */

    /* Stream type (DO NOT change values). 
     * Check if SPxSdbApplication::GetMaxStreamCount() also needs to be 
     * updated when adding new types.
     */
    enum Type_t
    {
        TYPE_UNKNOWN                = 0,    /* Unknown type - reserved. */
        TYPE_GENERIC                = 1,    /* Unspecified type or stream with multiple types (see note below). */
        TYPE_RADAR_VIDEO            = 2,    /* Radar video. */
        TYPE_TRACKS                 = 3,    /* Radar tracks. */
        TYPE_PLOTS                  = 4,    /* Radar plot reports. */
        TYPE_AIS                    = 5,    /* AIS tracks. */
        TYPE_ADSB                   = 6,    /* ADS-B tracks. */
        TYPE_NAV_DATA               = 7,    /* Navigation or GPS data. */
        TYPE_VIDEO                  = 8,    /* Camera video. */
        TYPE_AUDIO                  = 9,    /* Audio. */
        TYPE_VIDEO_TRACKS           = 10,   /* Tracks from a video tracker. */
        TYPE_HEARTBEATS             = 11,   /* Regular heartbeats. */
        TYPE_EXTENDED_HEARTBEATS    = 12,   /* Extended heartbeats. */
        TYPE_PROJECT_HEARTBEATS     = 13,   /* Project specific heartbeats. */
        TYPE_RADAR_IMAGES           = 14,   /* Radar PPI images. */
        TYPE_SCREEN_IMAGES          = 15,   /* Images from a screen recorder. */
        TYPE_TARGETS                = 16,   /* Simulated targets. */
        TYPE_LICENSE_DATA           = 17,   /* License data. */
        TYPE_ALERTS                 = 18,   /* Alert messages. */
        TYPE_FLIGHT_PLAN_DATA       = 19,   /* Flight plan data. */
        TYPE_CAMERA_CTRL            = 20,   /* Camera control or position data. */
        TYPE_STATUS                 = 21,   /* Status information. */
        TYPE_META                   = 22,   /* Meta data. */
        TYPE_VERSION                = 23,   /* Version information. */
        TYPE_MULTILATERATION        = 24,   /* Multilateration targets. */
        TYPE_WEATHER                = 25,   /* Weather data. */

        /* Deprecated types. */
        TYPE_OTHER                  = TYPE_GENERIC
    };

    /* Stream subtype (DO NOT change values).
     * Check if SPxSdbApplication::GetMaxStreamCount() also needs to be 
     * updated when adding new subtypes.
     */
    enum Subtype_t
    {
        SUBTYPE_UNKNOWN      = 0,    /* Unknown subtype - reserved. */
        SUBTYPE_GENERIC      = 1,    /* Unspecified subtype or stream with multiple subtypes (see note below). */
        SUBTYPE_PROJECT      = 2,    /* Defined by project code. */
        SUBTYPE_SPX          = 3,    /* Packets with an SPx header. */
        SUBTYPE_ASTERIX      = 4,    /* Packets with an ASTERIX header. */
        SUBTYPE_TTM          = 5,    /* TTM with no header. */
        SUBTYPE_NMEA         = 6,    /* NMEA with no header. */
        SUBTYPE_RTP          = 7,    /* RTP stream. */
        SUBTYPE_RTSP         = 8,    /* RTSP stream. */
        SUBTYPE_DF17         = 9,    /* ADS-B DF17 extended squitter. */
        SUBTYPE_SGF          = 10,   /* SGF 1.1 wrapped tracks/plots. */
        SUBTYPE_JSON         = 11,   /* Raw JSON message (no header). */

        /* Deprecated subtypes. */
        SUBTYPE_OTHER        = SUBTYPE_GENERIC
    };

    /* ## Use of TYPE_GENERIC and SUBTYPE_GENERIC ##
     *
     * These should be used when the stream type is unspecified, 
     * can change, or can contain multiple different types/subtypes, 
     * e.g. the output stream from a network repeater application. 
     * When a stream is configured for TYPE_GENERIC/SUBTYPE_GENERIC, 
     * the packet matching code in handlePacket() treats this as a wildcard 
     * and will allow through any packet type/subtype.
     *
     * If the type/subtype is known but not in the list then add a
     * new entry. Do not use TYPE_UNKNOWN/SUBTYPE_UNKNOWN.
     */

    /* Details of a packet sender. */
    struct Sender_t
    {
        UINT32 hostAddr;            /* Host address of sender. */
        unsigned int port;          /* Local port used by sender. */
        SPxTime_t lastPacketTime;   /* Time of last valid packet. */
    };

    /* Descriptor static field masks. */
    static const UINT32 DESC_MASK_NAME          = 0x00000001;
    static const UINT32 DESC_MASK_TYPE          = 0x00000002;
    static const UINT32 DESC_MASK_SUBTYPE       = 0x00000004;
    static const UINT32 DESC_MASK_SENDER_ID     = 0x00000008;
    static const UINT32 DESC_MASK_IS_TCP        = 0x00000010;

    /* Descriptor static/dynamic field masks (depending on stream subtype). */
    static const UINT32 DESC_MASK_DIST_ADDR     = 0x00001000;
    static const UINT32 DESC_MASK_DIST_PORT     = 0x00002000;

    /* Descriptor dynamic field masks. */
    static const UINT32 DESC_MASK_DIST_IF_ADDR  = 0x00010000;
    static const UINT32 DESC_MASK_SENDER_PORT   = 0x00020000;

    /* All descriptor mask bits. */
    static const UINT32 DESC_MASK_ALL           = 0xFFFFFFFF;

    /* Stream descriptor. This consists of both static and 
     * dynamic fields. Dynamic fields also have a last updated 
     * time and may be purged if not updated regularly. Unlike
     * static fields, dynamic fields are not saved to config.
     */
    struct Desc_t
    {
        /* Which of the following fields are set? */
        UINT32 mask;

        /* Static fields. */
        char name[NAME_MAX_LEN];            /* Stream name. */
        Type_t type;                        /* Stream type. */
        Subtype_t subtype;                  /* Stream subtype. */
        UINT16 senderID;                    /* Expected sender ID. */
        int isTCP;                          /* TCP or UDP? */

        /* Static/dynamic fields (depending on stream subtype). */
        UINT32 distAddr;                    /* Distribution address. */
        SPxTime_t distAddrUpdateTime;       /* Time of last update. */
        int distPort;                       /* Distribution port. */
        SPxTime_t distPortUpdateTime;       /* Time of last update. */

        /* Dynamic fields. */
        UINT32 distIfAddr;                  /* Distribution interface address. */
        SPxTime_t distIfAddrUpdateTime;     /* Time of last update. */
        unsigned int senderPort;            /* Sender port. */
        SPxTime_t senderPortUpdateTime;     /* Time of last update. */

    };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSdbStream(SPxSdbApplication *application,
                 const char *uuid,
                 const Desc_t *desc);
    virtual ~SPxSdbStream(void);

    /* General. */
    SPxErrorCode UpdateDesc(const Desc_t *desc, UINT32 clearMask=0);
    SPxErrorCode GetDesc(Desc_t *desc);
    int IsLocal(void);
    SPxErrorCode SetInstanceID(unsigned int id);
    unsigned int GetInstanceID(void) { return m_instanceID; }
    unsigned int Compare(const char *uuid, const Desc_t *desc);
    SPxErrorCode Combine(SPxSdbStream **stream);
    SPxErrorCode HandleJSON(const void *jsonStrVoid,
                            const MonitorScope_t *scope,
                            EndPoint_t source);
    SPxErrorCode AppendJSON(void *writerVoid,
                            const MonitorScope_t *scope,
                            EndPoint_t dest);
    SPxErrorCode AppendHTML(char **buf, 
                            unsigned int *bufLen, 
                            unsigned int level,
                            int isExpected);

    /* Packet detection. */
    SPxErrorCode SetReceiveEnabled(int isEnabled, int allSenders=FALSE);
    int IsReceiveEnabled(void) { return m_isReceiveEnabled; }
    int IsDetected(double timeoutSecs, 
                   UINT16 *lastSenderIDRtn=NULL, 
                   UINT8 *lastAsterixCatRtn=NULL);
    unsigned int GetSenders(Sender_t *sendersRtn[]);

    /* Linked sobjects. */
    void SetSenderObj(SPxObj *obj) { m_senderObj = obj; }
    SPxObj *GetSenderObj(void) { return m_senderObj; }
    void SetSenderNetAddr(SPxNetAddr *netAddr) { m_senderNetAddr = netAddr; }
    SPxNetAddr *GetSenderNetAddr(void) { return m_senderNetAddr; }
    void SetPIM(SPxPIM *pim) { m_pim = pim; }
    SPxPIM *GetPIM(void) { return m_pim; }

    /*
     * Public static functions.
     */

    static const char *GetTypeName(Type_t type);
    static const char *GetSubtypeName(Subtype_t subtype);

    static SPxErrorCode GetPacketType(const UINT8 *payload,
                                      unsigned int numBytes,
                                      Type_t *typeRtn,
                                      Subtype_t *subtypeRtn);
    static SPxErrorCode GetSPxPacketBType(UINT16 packetType,
                                          Type_t *typeRtn,
                                          Subtype_t *subtypeRtn);
    static SPxErrorCode GetAsterixType(UINT8 cat, 
                                       Type_t *typeRtn,
                                       Subtype_t *subtypeRtn);
    static SPxErrorCode GetNMEAType(const char *nmea,
                                    Type_t *typeRtn,
                                    Subtype_t *subtypeRtn);

    static SPxErrorCode GetSPxSenderID(const UINT8 *payload,
                                       unsigned int numBytes,
                                       UINT16 *senderIDRtn);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* General. */
    // cppcheck-suppress virtualCallInConstructor
    virtual void updateDisplayName(void);
    virtual SPxErrorCode purgeDesc(double timeoutSecs);

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;                   /* Private data. */
    Desc_t m_desc;                          /* Our descriptor. */
    unsigned int m_instanceID;              /* Instance ID. */
    SPxPacketDecoderNet *m_receiver;        /* Packet receiver. */
    SPxCriticalSection m_receiverMutex;     /* Mutex for packet receiver. */
    int m_isReceiveEnabled;                 /* Enable receiving of packets? */
    int m_isReceiveAllSenders;              /* Receive from all senders? */
    Sender_t m_lastSender;                  /* Details of last packet sender. */
    SPxTime_t m_lastValidPacketTime;        /* Time of last valid packet. */
    SPxAsterixDecoder *m_asterixDecoder;    /* ASTERIX decoder. */
    UINT8 m_tmpAsterixCat;                  /* Temporary ASTERIX category set from tmpAsterixHandler(). */
    UINT8 m_tmpAsterixSAC;                  /* Temporary ASTERIX SAC set from tmpAsterixHandler(). */
    UINT8 m_tmpAsterixSIC;                  /* Temporary ASTERIX SIC set from tmpAsterixHandler(). */
    UINT16 m_lastDetectedSenderID;          /* Last detected sender ID (or ASTERIC SAC/SIC). */
    UINT8 m_lastDetectedAsterixCat;         /* Last detected ASTERIX category. */
    SPxTimer m_timer;                       /* Background timer. */
    SPxObj *m_senderObj;                    /* Linked sender object. */
    SPxNetAddr *m_senderNetAddr;            /* SPxNetAddr for the above object. */
    SPxPIM *m_pim;                          /* Linked PIM. */

    /*
     * Private functions.
     */

    /* General. */
    int isDistAddrPortDynamic(void);
    void handlePacket(const struct sockaddr_in *sender,
                      UINT32 distAddr,
                      UINT16 distPort,
                      const UINT8 *payload,
                      unsigned int numBytes);
    void handleSPxPacket(const UINT8 *payload,
                         unsigned int numBytes,
                         const Sender_t *senderInfo);
    void handleAsterixPacket(const UINT8 *payload,
                             unsigned int numBytes,
                             const Sender_t *senderInfo);
    void handleGenericPacket(const Sender_t *senderInfo);

    void updateReceiver(void);
    SPxErrorCode getAsterixAttributes(const UINT8 *payload, 
                                      unsigned int numBytes,
                                      UINT8 *catRtn,
                                      UINT8 *sacRtn,
                                      UINT8 *sicRtn);

    /*
     * Private static functions.
     */

    static void updateReceiverTimer(void *userArg);

    static void packetHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *sender,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    static void tmpAsterixHandler(const SPxAsterixDecoder *decoder,
                                  void *userArg,
                                  UINT8 category,
                                  SPxAsterixMsg *mesg);

}; /* SPxSdbStream */

#endif /* _SPX_SDB_STREAM_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
