/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSiteAlert class.
 *
 * Revision Control:
 *   21/04/22 v1.23  SP 	Add UUID.
 *
 * Previous Changes:
 *   13/04/22 1.22  SP 	Add DESC_MOD_ITEM1_OR_ITEM2 modifier.
 *   21/12/20 1.21  SP 	Add web port clash warning.
 *   09/10/20 1.20  SP 	Add server time not synced warning.
 *   19/03/20 1.19  SP 	Add server priority clash alert.
 *   13/03/20 1.18  SP 	Remove unused descriptor fields.
 *   27/02/20 1.17  SP 	Change error code.
 *   05/02/20 1.16  SP 	Remove support for fatal errors.
 *   05/02/20 1.15  SP 	Rename alert.
 *   04/02/20 1.14  SP 	Use item local ID instead of UUID.
 *   03/02/20 1.13  SP 	Fix warning.
 *   31/01/20 1.12  SP 	Support fatal error.
 *   09/12/19 1.11  SP 	Add memory load alert.
 *   08/01/20 1.10  SP 	Add application source status alerts.
 *   08/01/20 1.9   SP 	Add application health status alerts.
 *   07/01/20 1.8   SP 	Record alert resolved time.
 *   09/12/19 1.7   SP 	Add stream address clash alert.
 *   05/12/19 1.6   SP 	Add stream detection alerts.
 *   05/12/19 1.5   SP 	Add high CPU and net load alerts.
 *   04/12/19 1.4   SP 	Add command port clash alert.
 *   04/12/19 1.3   SP 	Support per-item alert configuration.
 *   22/11/19 1.2   SP 	Support link to database items.
 *   21/11/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SITE_ALERT_H
#define _SPX_SITE_ALERT_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"

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
class SPxSiteNotifier;
class SPxSiteDatabase;

/*
 *  SPxSiteAlert class.
 */
class SPxSiteAlert : public SPxObj
{
public:
    /*
     * Public types.
     */

    /* Keep in sync with SPxSdbItem UUID. */
    static const unsigned int UUID_MAX_LEN = 64;

    /* Type. */
    enum Type_t
    {
        TYPE_NONE           = 0,
        TYPE_WARNING        = 1,
        TYPE_ERROR          = 2
    };

    /* State. */
    enum State_t
    {
        STATE_UNKNOWN   = 0,
        STATE_ACTIVE    = 1,
        STATE_RESOLVED  = 2
    };

    /* Alert codes - DO NOT CHANGE VALUES. */
    enum Code_t
    {
        /* No error or warning. */
        CODE_NONE       = 0,

        /* Warnings are 1 to 499, more specifically:
         * General warnings         1 - 99
         * Site warnings            100 - 199
         * System warnings          209 - 299
         * Application warnings     309 - 399
         * Stream warnings          409 - 499
         */
        CODE_WARN_FIRST                         = 1,  /* First warning. */
        CODE_WARN_UNEXPECTED_ITEM               = 1,
        CODE_WARN_HW_OVER_TEMPERATURE           = 2,
        CODE_WARN_SYSTEM_HIGH_CPU_LOAD          = 200,
        CODE_WARN_SYSTEM_HIGH_MEM_LOAD          = 201,
        CODE_WARN_SYSTEM_HIGH_NET_LOAD          = 202,
        CODE_WARN_SYSTEM_TIME_NOT_SYNCED        = 203,
        CODE_WARN_STREAM_DISABLED               = 400,
        CODE_WARN_LAST                          = 499, /* Last warning */

        /* Errors are 500 to 999, more specifically:
         * General errors       500 - 599
         * Site errors          600 - 699
         * System errors        700 - 799
         * Application errors   800 - 899
         * Stream errors        900 - 999
         */
        CODE_ERR_FIRST                          = 500, /* First error. */
        CODE_ERR_EXPECTED_ITEM_MISSING          = 500,
        CODE_ERR_APP_CMD_PORT_CLASH             = 800,
        CODE_ERR_APP_GENERAL                    = 801,
        CODE_ERR_APP_LICENSE                    = 802,
        CODE_ERR_APP_OVERLOAD                   = 803,
        CODE_ERR_APP_SRC_GENERAL                = 804,
        CODE_ERR_APP_SRC_SETUP                  = 805,
        CODE_ERR_APP_SRC_NO_RETURNS             = 806,
        CODE_ERR_APP_SRC_NO_AZIMUTHS            = 807,
        CODE_ERR_APP_SRC_NO_VIDEO               = 808,
        CODE_ERR_APP_SRC_SECONDARY              = 809,
        CODE_ERR_APP_SRC_AV_SETUP               = 810,
        CODE_ERR_APP_SRC_AV_NO_DATA             = 811,
        CODE_ERR_APP_SVR_PRIORITY_CLASH         = 812,
        CODE_ERR_APP_WEB_PORT_CLASH             = 813,
        CODE_ERR_STREAM_PACKETS_NOT_DETECTED    = 900,
        CODE_ERR_STREAM_ADDR_CLASH              = 901,
        CODE_ERR_LAST                           = 999  /* Last error */
    };

    /* Descriptor field masks. */
    static const UINT32 DESC_MASK_CODE          = 0x00000001;
    static const UINT32 DESC_MASK_ITEM1         = 0x00000002;
    static const UINT32 DESC_MASK_ITEM2         = 0x00000004;
    static const UINT32 DESC_MASK_TYPE          = 0x00000008;
    static const UINT32 DESC_MASK_STATE         = 0x00000010;
    static const UINT32 DESC_MASK_ALL           = 0xFFFFFFFF;

    /* Descriptor modifier flags. */
    static const UINT32 DESC_MOD_ITEM1_DESCENDANTS  = 0x00000001;   /* Match descendants of item1. */
    static const UINT32 DESC_MOD_ITEM2_DESCENDANTS  = 0x00000002;   /* Match descendants of item2. */
    static const UINT32 DESC_MOD_ITEM1_OR_ITEM2     = 0x00000004;   /* Match either item1 or item2. */

    /* Alert descriptor. */
    struct Desc_t
    {
        /* Which of the following fields are set? */
        UINT32 mask;

        /* Modifier flags used during comparison. */
        UINT32 modifiers;

        /* Fields. */
        Code_t code;                                /* Alert code. */
        UINT32 item1LocalID;                        /* Item 1 local ID. */
        UINT32 item2LocalID;                        /* Item 2 local ID. */

        /* These fields are set by the object when created. */
        Type_t type;                                /* Type. */
        State_t state;                              /* State. */
    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSiteAlert(SPxSiteNotifier *notifier, 
                 const char *uuid,
                 const Desc_t *desc);
    virtual ~SPxSiteAlert(void);

    /* General. */
    SPxSiteNotifier *GetNotifier(void) { return m_notifier; }
    UINT32 GetLocalID(void);
    SPxErrorCode SetUUID(const char *uuid);
    SPxErrorCode GetUUID(char *buf, unsigned int bufLen);
    int CompareUUID(const char *uuid);
    SPxErrorCode SetStillActive(void);
    SPxErrorCode GetDesc(Desc_t *desc);
    unsigned int Compare(const char *uuid, const Desc_t *desc);
    SPxErrorCode GetCreateTime(SPxTime_t *time);
    SPxErrorCode GetResolveTime(SPxTime_t *time);
    SPxErrorCode GetLastUpdateTime(SPxTime_t *time);
    SPxErrorCode GetDescription(char *buf, unsigned int bufLen);
    SPxErrorCode Update(void);

    /*
     * Public static functions.
     */

    static const char *GetTypeName(Type_t type);
    static const char *GetStateName(State_t state);
    static const char *GetLongCodeName(Code_t code);
    static SPxErrorCode GetShortCodeName(Code_t code, char *buf, unsigned int bufLen);
    static SPxErrorCode GetTimeString(SPxTime_t *time, char *buf, unsigned int bufLen);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* General. */

    /* Debugging. */

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;           /* Private data. */
    SPxSiteNotifier *m_notifier;    /* Parent notifier. */
    SPxSiteDatabase *m_database;    /* Site database. */
    UINT32 m_localID;               /* Local ID. */
    SPxTime_t m_createTime;         /* Time alert was created. */
    SPxTime_t m_resolveTime;        /* Time alert was resolved. */
    SPxTime_t m_lastUpdateTime;     /* Time of last update. */
    Desc_t m_desc;                  /* Descriptor. */

    /*
     * Private functions.
     */

    /* General. */
    void updateDescription(void);
    unsigned int getNumMaskBits(UINT32 mask);
    SPxErrorCode resolveAlert(void);

    /*
     * Private static functions.
     */

}; /* SPxSiteAlert */

#endif /* _SPX_SITE_ALERT_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
