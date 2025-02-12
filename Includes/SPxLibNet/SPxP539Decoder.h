/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxP539Decoder class.
 *
 * Revision Control:
 *   04/08/22 v1.2   SP 	Support packed sensor specific data.
 *
 * Previous Changes:
 *   04/08/22 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_P539_DECODER_H
#define _SPX_P539_DECODER_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibNet/SPxPacketDecoderNet.h"

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

/* Packed structure for sensor specific data. This structure
 * should be 4-byte aligned. The fields are in host-endian 
 * order but should be converted to big-endian when copied 
 * into sensorSpecificData[] in SPxPacketTrackExtended. This
 * structure must not exceed 128 bytes. Update the version
 * number when making changes to the structure.
 */
struct SPxP539SensorData_t
{
    UINT16 id;                  /* Set to 539. */
    UINT16 version;             /* Version number, unique to the following layout. */
    INT16 signalling;           /* COLREG signal (collision avoidance). */
    INT16 wayStatus;            /* Way status. */
    UINT32 classifier;          /* Classification. */
    char detectedMarkings[64];  /* Null-terminated string. */
};

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*
 * Define our class.
 */
class SPxP539Decoder : public SPxPacketDecoderNet
{
public:

    /*
     * Public types.
     */

    /* Estimated size. */
    struct SizeEstimate_t 
    {
        int isSet;              /* TRUE or FALSE. */
        double lengthMetres;    /* Length of observed object in metres. */
        double lengthError;     /* Length error in metres. */
        double widthMetres;     /* Width of observed object in metres. */
        double widthError;      /* Width error in metres. */
        double heightMetres;    /* Height of observed object in metres. */
        double heightError;     /* Height error in metres. */
    };

    /* Estimated azimuth. */
    struct AzimuthEstimate_t
    {
        int isSet;              /* TRUE or FALSE. */
        double degrees;         /* Object azimuth in degrees relative to own ship's heading. */
        double error;           /* Object azimuth error in degrees. */
    };

    /* Type of range. */
    enum RangeEstimateKind_t
    {
        RANGE_EST_KIND_UNKNOWN = 0,
        RANGE_EST_KIND_MONOCULAR_ANGLE = 1,
        RANGE_EST_KIND_MONOCULAR_SCALE = 2
    };

    /* Estimated range. */
    struct RangeEstimate_t
    {
        int isSet;                  /* TRUE or FALSE. */
        RangeEstimateKind_t kind;   /* Type of range. */
        double metres;              /* Object range in metres. */
        double error;               /* Object range error in metres. */
    };

    /* Estimated heading. */
    struct HeadingEstimate_t 
    {
        int isSet;                  /* TRUE or FALSE. */
        double degrees;             /* Object heading in degrees. */
        double error;               /* Object heading error in degrees. */
    };

    /* Camera observation. */
    struct Observation_t
    {
        SPxTime_t timestamp;                    /* Timestamp. */
        int intentity;                          /* Indentity (not currently used). */
        SizeEstimate_t estimatedSize;           /* Estimated size. */
        AzimuthEstimate_t estimatedAzimuth;     /* Estimated azimuth. */
        RangeEstimate_t estimatedRange;         /* Estimated range. */
        HeadingEstimate_t estimatedHeading;     /* Estimated heading. */
        SPxP539SensorData_t sensorData;         /* Sensor specific data. */
    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP539Decoder(void);
    virtual ~SPxP539Decoder(void);

    /* Configuration. */
    SPxErrorCode SetAzimuthOffset(double degs);
    double GetAzimuthOffset(void) { return m_azimuthOffsetDegs; }

    /* Debug info. */
    SPxErrorCode SetDebugFilename(const char *filename);
    SPxErrorCode GetDebugFilename(char *buf, unsigned int bufLen);

    /* Callback handling functions. The callback will be invoked with
     * a pointer to a Obseravtion_t structure when an update is received, 
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
    SPxCallbackList *m_callbackList;
    FILE *m_debugFile;
    double m_azimuthOffsetDegs;     /* Azimuth correction. */

    /*
     * Private functions.
     */

    void handleMesg(const struct sockaddr_in *sender,
                    const UINT8 *payload,
                    unsigned int numBytes);
    void writeDebug(const char *text);

    /*
     * Private static functions.
     */

    static void mesgHandler(SPxPacketDecoder *decoder,
                            void *userArg,
                            UINT32 packetType,
                            struct sockaddr_in *sender,
                            SPxTime_t *timestamp,
                            const unsigned char *payload,
                            unsigned int numBytes);

}; /* SPxP539Decoder */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* _SPX_P539_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
