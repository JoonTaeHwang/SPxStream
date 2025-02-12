/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAeroscopeDecoder class which supports the decoding
 *   of drone detections from a DJI AeroScope.
 *
 * Revision Control:
 *   16/06/22 v1.11  SP 	Support altitude above take-off point.
 *
 * Previous Changes:
 *   21/03/22 1.10  SP 	Report controller position.
 *   14/03/22 1.9   SP 	Support status 250, 251 and 252.
 *   09/03/22 1.8   SP 	Support periodic status updates.
 *   09/03/22 1.7   SP 	Support auto request window size.
 *   09/03/22 1.6   SP 	Remove UseSourceTimestamps().
 *   08/03/22 1.5   SP 	Rename token to key.
 *   08/03/22 1.4   SP 	Only report most recent drone record.
 *   07/03/22 1.3   SP 	Add debug support.
 *   07/03/22 1.2   SP 	Add more access functions.
 *   02/03/22 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_AEROSCOPE_DECODER_H
#define _SPX_AEROSCOPE_DECODER_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibNet/SPxNetAddr.h"

/* Forward declare any classes required. */

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

/*
 * Define our class.
 */
class SPxAeroscopeDecoder : public SPxObj, public SPxNetAddr
{
public:

    /*
     * Public types.
     */

    /* AeroScope status. Update handleInfoForAeroscope() and 
     * isStatusNormal() when adding more error codes.
     */
    enum Status_t
    {
        STATUS_OFFLINE = 0,
        STATUS_NORMAL = 100,
        STATUS_NORMAL_UPGRADING = 101,
        STATUS_NORMAL_UPGRADE_SUCCESS = 102,
        STATUS_ERROR = 200,
        STATUS_ERROR_UPGRADE_FAILURE = 201,
        STATUS_ERROR_ZERO_SATELLITES = 202,
        STATUS_ERROR_VOLTAGE_OUT_OF_RANGE = 203,
        STATUS_FATAL_ERROR = 250,
        STATUS_FATAL_ERROR_LB_SDR_WIFI = 251,
        STATUS_FATAL_ERROR_NO_HEARTBEAT = 252,
        STATUS_UNKNOWN = 9999   /* Used internally only. */
    };

    /* Drone report. */
    struct DroneReport_t
    {
        SPxTime_t timestamp;        /* Timestamp. */
        char droneID[64];           /* Drone unique ID. */
        char droneType[64];         /* Drone type if known. */
        int isPositionSet;          /* Is drone lat/long set? */
        double latDegs;             /* Drone latitude in degrees. */
        double longDegs;            /* Drone longitude in degrees. */
        int isVelocitySet;          /* Is drone speed/course set? */
        double speedMps;            /* Drone speed in m/s. */
        double courseDegs;          /* Drone course in degrees. */
        int isAltitudeAMSLSet;      /* Is drone altitude above mean sea level set? */
        double altitudeAMSLMetres;  /* Drone altitude above mean sea level in metres. */
        int isAltitudeATOPSet;      /* Is drone altitude above take off point set? */
        double altitudeATOPMetres;  /* Drone altitude above take off point in metres. */
        int isCtrlPositionSet;      /* Is controller lat/long set? */
        double ctrlLatDegs;         /* Controller latitude in degrees. */
        double ctrlLongDegs;        /* Controller longitude in degrees. */
    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxAeroscopeDecoder(void);
    virtual ~SPxAeroscopeDecoder(void);
    virtual SPxErrorCode Create(double pollIntervalSecs=1.0);

    /* Configuration. */
    SPxErrorCode SetKey(const char *key);
    SPxErrorCode GetKey(char *buf, unsigned int bufLen);
    SPxErrorCode SetUserName(const char *name);
    SPxErrorCode GetUserName(char *buf, unsigned int bufLen);
    SPxErrorCode SetAeroscopeID(const char *id);
    SPxErrorCode GetAeroscopeID(char *buf, unsigned int bufLen);
    SPxErrorCode SetReceiveWindow(double secs);
    double GetReceiveWindow(void) { return m_receiveWindowSecs; }
    SPxErrorCode SetLatencyCorrection(double secs);
    double GetLatencyCorrection(void) { return m_latencyCorrectionSecs; }

    /* Info on connected AeroScope info. */
    Status_t GetAeroscopeStatus(void) { return m_status; }
    SPxErrorCode GetAeroscopeName(char *buf, unsigned int bufLen, 
                                  int mutexAlreadyLocked=FALSE);
    SPxErrorCode GetAeroscopeLatLong(double *latDegs, double *longDegs, 
                                     int mutexAlreadyLocked=FALSE);

    /* Debug info. */
    SPxErrorCode SetDebugFilename(const char *filename);
    SPxErrorCode GetDebugFilename(char *buf, unsigned int bufLen);

    /* Callback handling functions. The callback will be invoked with
     * a pointer to a DroneReport_t structure when an update is received, 
     * but the handle is only valid for the duration of the callback function.
     */
    SPxErrorCode AddCallback(SPxCallbackListFn_t fn, void *userObj)
    {
        SPxErrorCode err = SPX_ERR_NOT_INITIALISED;
        if (m_callbackList)
        {
            err = static_cast<SPxErrorCode>(m_callbackList->AddCallback(fn, userObj));
        }
        return err;
    }

    SPxErrorCode RemoveCallback(SPxCallbackListFn_t fn, void *userObj)
    {
        SPxErrorCode err = SPX_ERR_NOT_INITIALISED;
        if (m_callbackList)
        {
            err = static_cast<SPxErrorCode>(m_callbackList->RemoveCallback(fn, userObj));
        }
        return err;
    }

    /* SPxNetAddr interface functions. */
    SPxErrorCode SetAddress(const char *address, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetAddress(UINT32 address, int port=0, const char *ifAddr=NULL);
    UINT32 GetAddress(void) const;
    const char *GetAddressString(void) const;
    int GetPort(void) const;
    UINT32 GetIfAddress(void) const;
    const char *GetIfAddressString(void) const;

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

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxCriticalSection m_mutex;
    SPxTimer m_timers;
    unsigned int m_recvBufLen;
    UINT8 *m_recvBuf;
    int m_port;
    double m_pollIntervalSecs;
    double m_requestTimeoutSecs;
    double m_receiveWindowSecs;
    double m_latencyCorrectionSecs;
    UINT64 m_lastDroneRecReqTimeMsecs;
    SPxTime_t m_lastStatusCheckTime;
    SPxTime_t m_lastStatusErrorTime;
    SPxCallbackList *m_callbackList;
    FILE *m_debugFile;

    /* AeroScope info. */
    Status_t m_status;
    int m_isPositionSet;
    double m_latDegs;
    double m_longDegs;

    /*
     * Private functions.
     */

    void update(void);
    int isStatusNormal(void);
    SPxErrorCode getResponse(UINT64 timeMsecs,
                             const char *request,
                             unsigned char *buf,
                             unsigned int bufLen,
                             const char *param1=NULL,
                             const char *value1=NULL,
                             const char *param2=NULL,
                             const char *value2=NULL,
                             const char *param3=NULL,
                             const char *value3=NULL,
                             const char *param4=NULL,
                             const char *value4=NULL);
    void handleInfo(void);
    void handleInfoForAeroscope(const void *jsonAsVoid);
    void handleDroneRecords(void);
    void handleDroneRecord(const void *jsonRecVoid);
    void writeDebug(const char *text);

    /*
     * Private static functions.
     */

    static void updateTimer(void *userArg);
    static bool compareDroneReportsDroneID(const DroneReport_t &dr1, const DroneReport_t &dr2);
    static bool compareDroneReportsTimestamp(const DroneReport_t *dr1, const DroneReport_t *dr2);

}; /* SPxAeroscopeDecoder */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* _SPX_AEROSCOPE_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
