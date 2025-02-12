/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSdbField class.
 *
 * Revision Control:
 *   17/06/22 v1.29  SP 	Add FKEY_UPTIME.
 *
 * Previous Changes:
 *   16/05/22 1.28  SP 	Add FKEY_PING_SUCCESS_RATE and FKEY_PING_TIME_MSECS.
 *   10/02/22 1.27  SP 	Add FKEY_GPSA_NAV_OUTPUT_MODE.
 *   09/02/22 1.26  SP 	Rename more SPxGPSAssist field keys.
 *   09/02/22 1.25  SP 	Rename SPxGPSAssist field keys.
 *   14/12/21 1.24  SP 	Add SPxGPSAssist field keys.
 *   16/07/21 1.23  SP 	Add FKEY_WEB_PROTOCOL.
 *   02/07/21 1.22  SP 	Rename FKEY_RTSP_NUM_CLIENTS to FKEY_RTSP_NUM_ACT_CLIENTS.
 *                     	Add FKEY_RTSP_NUM_CON_CLIENTS.
 *   02/07/21 1.21  SP 	Add FKEY_RTSP_NUM_CLIENTS.
 *   02/06/21 1.20  SP 	Add various FKEY_DETECTED_xxx fields.
 *   28/04/21 1.19  SP 	Add FKEY_CAPABILITY.
 *   21/12/20 1.18  SP 	Add FKEY_WEB_PORT.
 *   09/10/20 1.17  SP 	Add FKEY_TIME.
 *   12/03/20 1.16  SP 	Support value type.
 *   05/03/20 1.15  SP 	Change formatting of field keys.
 *   04/03/20 1.14  SP 	Add fields to support SPxOutputManager.
 *   10/02/20 1.13  SP 	Remove FKEY_IS_RUNNING.
 *   16/12/19 1.12  SP 	Remove FKEY_IMAGE_NAME.
 *   16/12/19 1.11  SP 	Remove FKEY_COMMAND_LINE.
 *   10/12/19 1.10  SP 	Add FKEY_COMMAND_LINE and KFEY_IMAGE_NAME.
 *   09/12/19 1.9   SP 	Add FKEY_MEMORY_LOAD.
 *   21/11/19 1.8   SP 	Add more field keys.
 *   19/11/19 1.7   SP 	Remove support for static fields.
 *   15/11/19 1.6   SP 	Add FKEY_IS_DETECTED.
 *   15/11/19 1.5   SP 	Don't make public functions virtual.
 *   12/11/19 1.4   SP 	Add more field keys.
 *   08/11/19 1.3   SP 	Move all field keys here.
 *   06/11/19 1.2   SP 	Add more set functions.
 *   04/11/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SDB_FIELD_H
#define _SPX_SDB_FIELD_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"

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
class SPxSdbItem;

/*
 *  SPxSdbField class.
 */
class SPxSdbField : public SPxObj
{
public:
    /*
     * Public types.
     */

    /* Value type. */
    enum ValueType_t
    {
        VALUE_TYPE_NONE,
        VALUE_TYPE_STRING,
        VALUE_TYPE_INT,
        VALUE_TYPE_HEX,
        VALUE_TYPE_DOUBLE,
        VALUE_TYPE_BOOL,
        VALUE_TYPE_ADDR
    };

    /*
     * Public variables.
     */

    /* General field keys - add any generic/reusable keys here. */
    static const char *FKEY_CAPABILITY;
    static const char *FKEY_CLIENT_ADDRESS;
    static const char *FKEY_COMMAND_PORT;
    static const char *FKEY_CPU_LOAD;
    static const char *FKEY_DETECTED_ASTERIX_CAT;
    static const char *FKEY_DETECTED_ASTERIX_SAC;
    static const char *FKEY_DETECTED_ASTERIX_SIC;
    static const char *FKEY_DETECTED_SENDER_ID;
    static const char *FKEY_ERROR_CODE;
    static const char *FKEY_FREE_DISK_SPACE_GB;
    static const char *FKEY_IS_DETECTED;
    static const char *FKEY_IS_ENABLED;
    static const char *FKEY_IS_RECORDING;
    static const char *FKEY_IS_REPLAYING;
    static const char *FKEY_IS_STANDBY;
    static const char *FKEY_LATITUDE_DEGS;
    static const char *FKEY_LICENCE;
    static const char *FKEY_LONGITUDE_DEGS;
    static const char *FKEY_MEMORY_LOAD;
    static const char *FKEY_NET_HEALTH_STATUS;
    static const char *FKEY_NET_INTERFACE;
    static const char *FKEY_NET_INPUT_LOAD;
    static const char *FKEY_NET_OUTPUT_LOAD;
    static const char *FKEY_NET_SOURCE_STATUS;
    static const char *FKEY_NET_SOURCE_TYPE;
    static const char *FKEY_NUM_CHANNELS;
    static const char *FKEY_NUM_TRACKS;
    static const char *FKEY_PING_SUCCESS_RATE;
    static const char *FKEY_PING_TIME_MSECS;
    static const char *FKEY_RTP_DROP_RATE;
    static const char *FKEY_RTSP_MEDIA;
    static const char *FKEY_RTSP_MODES;
    static const char *FKEY_RTSP_NUM_ACT_CLIENTS;
    static const char *FKEY_RTSP_NUM_CON_CLIENTS;
    static const char *FKEY_RTSP_SERVER_PORT;
    static const char *FKEY_RTSP_REALM;
    static const char *FKEY_RTSP_TRACK;
    static const char *FKEY_SERVER_GROUP_ID;
    static const char *FKEY_SERVER_PRIORITY;
    static const char *FKEY_STATE;
    static const char *FKEY_TIME;
    static const char *FKEY_UPTIME;
    static const char *FKEY_VERSION;
    static const char *FKEY_WEB_PROTOCOL;
    static const char *FKEY_WEB_PORT;

    /* Field keys that are specific to SPxGPSAssist. */
    static const char *FKEY_GPSA_EST_POS_CONFIDENCE;
    static const char *FKEY_GPSA_EST_POS_CONFIDENCE_OK;
    static const char *FKEY_GPSA_NAV_INPUT_STATUS;
    static const char *FKEY_GPSA_NAV_OUTPUT_MODE;
    static const char *FKEY_GPSA_NAV_OUTPUT_STATUS;
    static const char *FKEY_GPSA_POSITION_DIVERGENCE_METRES;
    static const char *FKEY_GPSA_POSITION_DIVERGENCE_OK;
    static const char *FKEY_GPSA_RADAR_INPUT_STATUS;

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSdbField(SPxSdbItem *parent, const char *key);
    virtual ~SPxSdbField(void);

    /* General. */
    const char *GetKey(void);
    int CompareKey(const char *key);
    SPxErrorCode SetLastUpdateTime(const SPxTime_t *time=NULL);
    SPxErrorCode GetLastUpdateTime(SPxTime_t *time);
    SPxErrorCode SetValueString(const char *value);
    SPxErrorCode SetValueInt(int value);
    SPxErrorCode SetValueInt(unsigned int value);
    SPxErrorCode SetValueHex(unsigned int value, unsigned int numDigits);
    SPxErrorCode SetValueDouble(double value, unsigned int numDp);
    SPxErrorCode SetValueBool(bool isTrue);
    SPxErrorCode SetValueBool(int isTrue);
    SPxErrorCode SetValueAddr(UINT32 addr);
    ValueType_t GetValueType(void) { return m_valueType; }
    const char *GetValueAsString(void);
    int GetValueAsInt(void);
    unsigned int GetValueAsUInt(void);
    double GetValueAsDouble(void);
    bool GetValueAsBool(void);

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

    /* General. */

    /* Debugging. */

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;           /* Private data. */
    SPxSiteDatabase *m_database;    /* Parent database. */
    SPxSdbItem *m_parent;           /* Parent item. */
    SPxTime_t m_lastUpdateTime;     /* Time of last update. */
    ValueType_t m_valueType;        /* Value type. */
    int m_valueInt;                 /* Integer value. */
    double m_valueDouble;           /* Double value. */

    /*
     * Private functions.
     */

    /* General. */

    /*
     * Private static functions.
     */

}; /* SPxSdbField */

#endif /* _SPX_SDB_FIELD_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
