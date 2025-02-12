/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSdbSystem class.
 *
 * Revision Control:
 *   23/05/22 v1.28  SP 	Fix another issue with application instance IDs.
 *
 * Previous Changes:
 *   16/05/22 1.27  SP 	Add ping detection.
 *   07/04/22 1.26  SP 	Simplify display names.
 *   05/01/22 1.25  SP 	Remove ClearDesc().
 *   25/05/21 1.24  SP 	Fix N/C #856 - Hostname look-up slows down SPxMonitor.
 *   22/12/20 1.23  SP 	Improve syncing of UUIDs.
 *   09/03/20 1.22  SP 	Correct comments.
 *   19/02/20 1.21  SP 	Add AppendHTML().
 *   13/02/20 1.20  SP 	Remove deprecated functions.
 *   06/02/20 1.19  SP 	Return app after handling extended heartbeat.
 *   03/02/20 1.18  SP 	Support auto start.
 *   28/01/20 1.17  SP 	Support discovery, aggregation and reporting scope.
 *   20/01/20 1.16  SP 	Support applications start/stop.
 *   07/01/20 1.15  SP 	Support compressed JSON messages.
 *   04/12/19 1.14  SP 	Fix line endings.
 *   22/11/19 1.13  SP 	Changes to descriptor mask.
 *   19/11/19 1.12  SP 	Support saving sitemap to config.
 *   18/11/19 1.11  SP 	Mark items as expected when set from config.
 *   15/11/19 1.10  SP 	Don't make public functions virtual.
 *   14/11/19 1.9   SP 	Add GetApplications().
 *   08/11/19 1.8   SP 	Add ClearDesc().
 *   07/11/19 1.7   SP 	Remove GetApplications().
 *   06/11/19 1.6   SP 	Improve operation of Compare().
 *   31/10/19 1.5   SP 	Further development.
 *   30/10/19 1.4   SP 	Further development.
 *   29/10/19 1.3   SP 	Support generation of JSON messages.
 *   25/10/19 1.2   SP 	Rename class and further development.
 *   24/10/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SDB_SYSTEM_H
#define _SPX_SDB_SYSTEM_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxSdbItem.h"
#include "SPxLibUtils/SPxSdbApplication.h"

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
class SPxPing;

/*
 *  SPxSdbSystem class.
 */
class SPxSdbSystem : public SPxSdbItem
{
public:
    /*
     * Public variables.
     */

    /*
     * Public types.
     */

    /* Descriptor static field masks. */
    static const UINT32 DESC_MASK_IS_LOCAL      = 0x00000001;
    static const UINT32 DESC_MASK_NAME          = 0x00000002;
    static const UINT32 DESC_MASK_HOST_ADDR     = 0x00000004;
    static const UINT32 DESC_MASK_ALL_STATIC    = 0x0000FFFF;

    /* Descriptor dynamic field masks. */
    static const UINT32 DESC_MASK_ALL_DYNAMIC   = 0xFFFF0000;

    /* All descriptor mask bits. */
    static const UINT32 DESC_MASK_ALL           = 0xFFFFFFFF;

    /* System descriptor. This consists of both static and 
     * dynamic fields. Dynamic fields also have a last updated 
     * time and may be purged if not updated regularly. Unlike
     * static fields, dynamic fields are not saved to config.
     */
    struct Desc_t
    {
        /* Which of the following fields are set? */
        UINT32 mask;

        /* Static fields. */
        int isLocal;                    /* Local or remote? */
        char name[NAME_MAX_LEN];        /* System name. */
        UINT32 hostAddr;                /* Host IP address. */
    };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSdbSystem(SPxSdbSite *site,
                 const char *uuid,
                 const Desc_t *desc);
    virtual ~SPxSdbSystem(void);

    /* General. */
    SPxErrorCode UpdateDesc(const Desc_t *desc, UINT32 clearMask=0);
    SPxErrorCode GetDesc(Desc_t *desc);
    int IsLocal(void);
    unsigned int Compare(const char *uuid, const Desc_t *desc);
    SPxSdbApplication *AddOrGetApplication(const char *uuid,
                                           const SPxSdbApplication::Desc_t *desc, 
                                           int allowCreate);
    unsigned int GetApplications(const SPxSdbApplication::Desc_t *desc,
                                 SPxSdbApplication **appsRtn[]);
    unsigned int GetNumApplications(SPxSdbApplication::Type_t type);
    SPxErrorCode HandleJSON(const void *jsonSysVoid, 
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

    /* Ping detection. */
    SPxErrorCode SetPingDetectionEnabled(int isEnabled);
    int IsPingDetectionEnabled(void) { return m_isPingDetectionEnabled; }
    INT32 GetAveragePingMsecs(void) { return m_averagePingMsecs; }
    double GetPingSuccessRate(void) { return m_pingSuccessRate; }

    /* Extended heartbeat support. */
    SPxErrorCode DecodeExtHeartbeatJSON(const UINT8 *payload,
                                        unsigned int numBytes,
                                        const MonitorScope_t *scope,
                                        UINT32 senderAddr,
                                        UINT16 fromPort,
                                        SPxSdbApplication **appRtn);
    SPxErrorCode ParseExtHeartbeatJSON(const char *jsonText,
                                       const MonitorScope_t *scope,
                                       SPxSdbApplication **appRtn);

    /*
     * Public static functions.
     */

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

    /* Application info. */
    struct AppInfo_t
    {
        SPxSdbApplication::Type_t type; /* Type. */
        unsigned int count;             /* Instance count. */
        unsigned nextInstanceID;        /* Next instance ID to assign. */
    };

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;           /* Private data. */
    Desc_t m_desc;                  /* Our descriptor. */
    UINT8 *m_decompBuf;             /* Decompression buffer. */
    unsigned int m_decompBufSize;   /* Decompression buffer size. */
    SPxTimer m_timer;               /* Background timer. */
    int m_isPingDetectionEnabled;   /* Enable ping detection? */
    SPxPing *m_ping;                /* Ping client. */
    INT32 m_averagePingMsecs;       /* Average ping time. */
    double m_pingSuccessRate;       /* Ping success rate. */

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode addOrUpdateApplication(const void *jsonAppVoid,
                                        const MonitorScope_t *scope,
                                        EndPoint_t source,
                                        int uuidsAreDefinitive,
                                        SPxSdbApplication **appRtn);
    void updateAppInstanceIDs(void);
    void updatePingDetection(void);
    void syncHostname(void);

    /*
     * Private static functions.
     */

    /* Timer callbacks. */
    static void updatePingDetectionTimer(void *userArg);
    static void syncHostnameTimer(void *userArg);

}; /* SPxSdbSystem */

#endif /* _SPX_SDB_SYSTEM_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
