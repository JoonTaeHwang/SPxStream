/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSdbApplication class.
 *
 * Revision Control:
 *   23/05/22 v1.54  SP 	Fix another issue with stream instance IDs.
 *
 * Previous Changes:
 *   20/05/22 1.53  SP 	Improve combining of duplicated applications.
 *   19/05/22 1.52  SP 	Add TYPE_SPX_REPLAY_UDP.
 *   07/04/22 1.51  SP 	Simplify display names.
 *   06/04/22 1.50  SP 	Add TYPE_SPX_RADARVIEW_LITE and TYPE_SPX_RADARVIEW_240.
 *   05/04/22 1.49  AJH	Put TYPE_SPX_NET_REPEAT_MULTI in correct position.
 *   05/04/22 1.48  AJH	Add TYPE_SPX_NET_REPEAT_MULTI.
 *   29/03/22 1.47  AGC	Add TYPE_SPX_VIEWER_3D.
 *   09/03/22 1.46  BTB	Updated enum entry for Track Manager.
 *   05/01/22 1.45  SP 	Remove ClearDesc().
 *   20/10/21 1.44  SP 	Add TYPE_SPX_GPS_ASSIST.
 *   19/07/21 1.43  AJH	Add TYPE_SPX_HPX346_SERVER.
 *   16/07/21 1.42  SP 	Add GetWebInterfaceURL().
 *   07/07/21 1.41  SP 	Add TYPE_SPX_AV_VIDEO_OVERLAY.
 *   30/06/21 1.40  AGC	Add TYPE_SPX_TRACKER_3D.
 *   25/06/21 1.39  AJH	Add TYPE_SPX_RADAR_WEB_SERVER and TYPE_SPX_VID_PROC_RELAY.
 *   09/06/21 1.38  SP 	Add TYPE_SPX_SC_SERVER.
 *   31/03/21 1.37  SP 	Add TYPE_SPX_ALARM_SERVER.
 *   16/03/21 1.36  AGC	Add TYPE_SPX_CAMERA_MANAGER.
 *   22/12/20 1.35  SP 	Improve syncing of UUIDs.
 *   09/12/20 1.34  SP 	Add TYPE_SPX_NDS_ENTRY.
 *   19/06/20 1.33  SP 	Add TYPE_SPX_THREAT_PROCESSOR.
 *   04/06/20 1.32  SP 	Add TYPE_SPX_P172_RADAR_CTRL.
 *   19/05/20 1.31  SP 	Add TYPE_SPX_INPUT_SELECTOR.
 *   24/03/20 1.30  SP 	Add Combine().
 *   13/03/20 1.29  SP 	Add some recent applications.
 *   10/03/20 1.28  SP 	Override SetExpected().
 *   09/03/20 1.27  SP 	Correct comments.
 *   04/03/20 1.26  SP 	Add TYPE_SPX_OUTPUT_REPEATER.
 *   19/02/20 1.25  SP 	Add AppendHTML().
 *   13/02/20 1.24  SP 	Remove deprecated functions.
 *   06/02/20 1.23  SP 	Remove heartbeat sender ports from descriptor.
 *   03/02/20 1.22  SP 	Support auto start.
 *   28/01/20 1.21  SP 	Add IsLocal().
 *   20/01/20 1.20  SP 	Support application start/stop.
 *   16/01/20 1.19  SP 	Move image name to descriptor.
 *   16/01/20 1.18  SP 	Add processIDExcludeMask to descriptor.
 *   16/01/20 1.17  SP 	Move command line to descriptor.
 *   04/12/19 1.16  SP 	Fix line endings.
 *   22/11/19 1.15  SP 	Correct descriptor mask.
 *   19/11/19 1.14  SP 	Support saving sitemap to config.
 *   18/11/19 1.13  SP 	Mark items as expected when set from config.
 *   15/11/19 1.12  SP 	Add GetMaxStreamCount().
 *   15/11/19 1.11  SP 	Don't make public functions virtual.
 *   14/11/19 1.10  SP 	Add GetStreams().
 *   12/11/19 1.9   SP 	Changes to descriptor.
 *   08/11/19 1.8   SP 	Support generation of extended heartbeats.
 *   07/11/19 1.7   SP 	Support extended heartbeats.
 *   06/11/19 1.6   SP 	Add new descriptor field.
 *   31/10/19 1.5   SP 	Support more applications.
 *   30/10/19 1.4   SP 	Further development.
 *   29/10/19 1.3   SP 	Support generation of JSON messages.
 *   25/10/19 1.2   SP 	Rename class and further development.
 *   24/10/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SDB_APPLICATION_H
#define _SPX_SDB_APPLICATION_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxSdbItem.h"
#include "SPxLibUtils/SPxSdbStream.h"

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

/*
 *  SPxSdbApplication class.
 */
class SPxSdbApplication : public SPxSdbItem
{
public:
    /*
     * Public variables.
     */

    /*
     * Public types.
     */

    /* Application types. Append only - DO NOT CHANGE VALUES! 
     * The TYPE_NAMES array must be updated when adding a new 
     * application.
     */
    enum Type_t
    {
        TYPE_UNKNOWN                = 0,    /* Unknown application. */
        TYPE_SPX_UNKNOWN            = 1,    /* Unknown SPx application. */
        TYPE_SPX_MONITOR            = 2,
        TYPE_SPX_ASD                = 3,
        TYPE_SPX_RADAR_VIEW         = 4,
        TYPE_SPX_RADAR_WATCH        = 5,
        TYPE_SPX_RDR                = 6,
        TYPE_SPX_SERVER             = 7,
        TYPE_SPX_VIDEO_PROCESSOR    = 8,
        TYPE_SPX_VSD                = 9,
        TYPE_SPX_RADAR_SIMULATOR    = 10,
        TYPE_SPX_VIDEO_SIMULATOR    = 11,
        TYPE_SPX_FUSION_SERVER      = 12,
        TYPE_SPX_SCREEN_RECORDER    = 13,
        TYPE_SPX_SAFETY_NET         = 14,
        TYPE_SPX_RADAR_IMAGE_SERVER = 15,
        TYPE_SPX_AV_SERVER          = 16,
        TYPE_SPX_RDC                = 17,
        TYPE_SPX_NET_RECORD         = 18,
        TYPE_SPX_NET_REPLAY         = 19,
        TYPE_SPX_NET_REPEAT         = 20,
        TYPE_SPX_OUTPUT_REPEATER    = 21,
        TYPE_SPX_MDF                = 22,
        TYPE_SPX_MANUAL_TRACKER     = 23,
        TYPE_SPX_RADAR_COVERAGE     = 24,
        TYPE_SPX_SERIAL_TO_NET      = 25,
        TYPE_SPX_TRACK_MANAGER      = 26,
        TYPE_SPX_INPUT_SELECTOR     = 27,
        TYPE_SPX_P172_RADAR_CTRL    = 28,
        TYPE_SPX_CXEYE              = 29,
        TYPE_SPX_CXEYE_D            = 30,
        TYPE_SPX_THREAT_PROCESSOR   = 31,
        TYPE_SPX_NDS_ENTRY          = 32,
        TYPE_SPX_CAMERA_MANAGER     = 33,
        TYPE_SPX_ALARM_SERVER       = 34,
        TYPE_SPX_SC_SERVER          = 35,
        TYPE_SPX_RADAR_WEB_SERVER   = 36,
        TYPE_SPX_VID_PROC_RELAY     = 37,
        TYPE_SPX_TRACKER_3D         = 38,
        TYPE_SPX_AV_VIDEO_OVERLAY   = 39,
        TYPE_SPX_HPX346_SERVER      = 40,
        TYPE_SPX_GPS_ASSIST         = 41,
        TYPE_SPX_VIEWER_3D          = 42,
        TYPE_SPX_NET_REPEAT_MULTI   = 43,
        TYPE_SPX_RADARVIEW_LITE     = 44,
        TYPE_SPX_RADARVIEW_240      = 45,
        TYPE_SPX_REPLAY_UDP         = 46,
        MAX_NUM_TYPES   /* MUST BE LAST ENTRY. */
    };

    /* Maximum lengths. */
    static const unsigned int IMAGE_NAME_MAX_LEN    = 256;
    static const unsigned int CMD_LINE_MAX_LEN      = 512;

    /* Descriptor static field masks. */
    static const UINT32 DESC_MASK_NAME                  = 0x00000001;
    static const UINT32 DESC_MASK_TYPE                  = 0x00000002;
    static const UINT32 DESC_MASK_IS_64BIT              = 0x00000004;
    static const UINT32 DESC_MASK_IMAGE_NAME            = 0x00000008;
    static const UINT32 DESC_MASK_CMD_LINE              = 0x00000010;
    static const UINT32 DESC_MASK_ALL_STATIC            = 0x0000FFFF;

    /* Descriptor dynamic field masks. */
    static const UINT32 DESC_MASK_PROCESS_ID            = 0x00010000;
    static const UINT32 DESC_MASK_ALL_DYNAMIC           = 0xFFFF0000;

    /* All descriptor mask bits. */
    static const UINT32 DESC_MASK_ALL                   = 0xFFFFFFFF;

    /* Application descriptor. This consists of both static and 
     * dynamic fields. Dynamic fields also have a last updated 
     * time and may be purged if not updated regularly. Unlike
     * static fields, dynamic fields are not saved to config.
     */
    struct Desc_t
    {
        /* Which of the following fields are set? */
        UINT32 mask;

        /* Static fields. */
        char name[NAME_MAX_LEN];            /* Application instance name. */
        Type_t type;                        /* Application type. */
        int is64Bit;                        /* 32-bit or 64-bit application? */
        char imageName[IMAGE_NAME_MAX_LEN]; /* Image name including full path. */
        char cmdLine[CMD_LINE_MAX_LEN];     /* Command line. */

        /* Dynamic fields. */
        UINT32 processID;                   /* Local process ID. */
        UINT32 processIDExcludeMask;        /* Which bits of process ID are not set? */
        SPxTime_t processIDUpdateTime;      /* Time of last update. */
    };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSdbApplication(SPxSdbSystem *system,
                      const char *uuid,
                      const Desc_t *desc);
    virtual ~SPxSdbApplication(void);

    /* General. */
    SPxErrorCode UpdateDesc(const Desc_t *desc, UINT32 clearMask=0);
    SPxErrorCode GetDesc(Desc_t *desc);
    int IsLocal(void);
    SPxErrorCode SetInstanceID(unsigned int id);
    unsigned int GetInstanceID(void) { return m_instanceID; }
    unsigned int Compare(const char *uuid, const Desc_t *desc);
    SPxErrorCode Combine(SPxSdbApplication **app);
    int IsSameProcessID(const Desc_t *desc);
    SPxSdbStream *AddOrGetStream(const char *uuid,
                                 const SPxSdbStream::Desc_t *desc, 
                                 int allowCreate);
    unsigned int GetStreams(const SPxSdbStream::Desc_t *desc,
                            SPxSdbStream **streamsRtn[]);
    unsigned int GetNumStreams(SPxSdbStream::Type_t type);
    SPxErrorCode HandleJSON(const void *jsonAppVoid,
                            const MonitorScope_t *scope,
                            EndPoint_t source,
                            int uuidsAreDefinitive);
    SPxErrorCode AppendJSON(void *writerVoid, 
                            const MonitorScope_t *scope,
                            EndPoint_t dest);
    SPxErrorCode AppendHTML(char **buf, 
                            unsigned int *bufLen, 
                            unsigned int level,
                            int isExpected);
    SPxErrorCode GetWebInterfaceURL(char *url, unsigned int bufLen);

    /* Extended heartbeat support. */
    SPxErrorCode GetExtHeartbeatJSON(char *buf, unsigned int bufLen);

    /* Process control. */
    int IsRunning(void);
    SPxErrorCode Start(int checkOnly=FALSE, int allowMulti=FALSE);
    int CanStart(void);
    SPxErrorCode Stop(int checkOnly=FALSE);
    int CanStop(void);
    SPxErrorCode SetAutoStart(int isEnabled, int checkOnly=FALSE);
    int CanSetAutoStart(int isEnabled);
    int IsAutoStart(void) { return m_isAutoStartEnabled; }
    void CheckAutoStart(void);

    /* Overrides. */
    SPxErrorCode SetExpected(int isExpected, int setDescendants=TRUE, int setAncestors=TRUE);

    /*
     * Public static functions.
     */

    static const char *GetTypeName(Type_t type);

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
    virtual SPxErrorCode addChild(SPxSdbItem *child);
    virtual SPxErrorCode removeChild(SPxSdbItem *child);

private:

    /*
     * Private types.
     */

    /* Stream info. */
    struct StreamInfo_t
    {
        SPxSdbStream::Type_t type;          /* Type. */
        unsigned int count;                 /* Instance count. */
        unsigned nextInstanceID;            /* Next instance ID to assign. */
    };

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;           /* Private data. */
    Desc_t m_desc;                  /* Our descriptor. */
    unsigned int m_instanceID;      /* Instance ID. */
    int m_isAutoStartEnabled;       /* Is auto start enabled? */
    SPxTime_t m_lastAutoStartTime;  /* Time of last auto start. */

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode addOrUpdateStream(const void *jsonStrVoid,
                                   const MonitorScope_t *scope,
                                   EndPoint_t source,
                                   int uuidsAreDefinitive);
    void updateStreamInstanceIDs(void);

    /*
     * Private static functions.
     */

}; /* SPxSdbApplication */

#endif /* _SPX_SDB_APPLICATION_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
