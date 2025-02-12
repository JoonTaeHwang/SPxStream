/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Class used to interface to a remote object detector and receives 
 *   detections as JSON network messages.
 *
 * Revision Control:
 *   25/06/21 v1.2   SP 	Calculate real-world position.
 *
 * Previous Changes:
 *   24/06/21 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_OBJECT_DETECTOR_H
#define _SPX_OBJECT_DETECTOR_H

/*
 * Other headers required.
 */

#include "SPxLibNet/SPxPacketDecoderNet.h"
#include "SPxLibUtils/SPxViewControl.h" /* For SPxLatLong_t */
#include "SPxLibUtils/SPxTime.h"        /* For SPxLatLong_t */

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
class SPxCamera;

/*
 *  SPxObjectDetector class.
 */
class SPxObjectDetector : public SPxPacketDecoderNet
{
public:
    /*
     * Public variables.
     */

    /*
     * Public types.
     */

    /* Detected object. */
    struct Detection_t
    {
        unsigned int instance;  /* Zero-based instance number. */
        unsigned int classID;   /* Object class ID. */
        char className[256];    /* Object name. */
        UINT32 classARGB;       /* Object colour as 0xAARRGGBB */
        double confidence;      /* Confidence in range 0.0 to 1.0. */
        double boxXPx;          /* X position in video pixels. */
        double boxYPx;          /* Y position in video pixels. */
        double boxWPx;          /* Width in video pixels. */
        double boxHPx;          /* Height in video pixels. */
        unsigned int imageWPx;  /* Image width in pixels. */
        unsigned int imageHPx;  /* Image height in pixels. */
        double bearingDegs;     /* Bearing from camera in degrees. */
        double elevationDegs;   /* Elevation from camera in degrees. */
        double rangeMetres;     /* Range from camera in metres. */
        int isLLSet;            /* Is lat/long set? */
        SPxLatLong_t ll;        /* Lat/long. */
    };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxObjectDetector(SPxCamera *camera);
    virtual ~SPxObjectDetector(void);

    /* Access. */
    SPxErrorCode IterateDetections(SPxCallbackListFn_t fn, void *userArg);

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

private:

    /*
     * Private types.
     */
    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;           /* Private data. */
    SPxCriticalSection m_mutex;     /* Mutex for object. */
    SPxCamera *m_camera;            /* Associated camera. */
    SPxThread *m_thread;            /* Background thread. */
    SPxTime_t m_lastUpdateTime;     /* Time of last update. */

    /*
     * Private functions.
     */

    SPxErrorCode handlePacket(struct sockaddr_in *fromAddr,
                              const SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    void calcDetectionLatLong(Detection_t *det);

    void backgroundProc(void);

    /*
     * Private static functions.
     */

    static void packetHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *fromAddr,
                              struct SPxTime_tag *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    static void *threadHandler(SPxThread *thread);

}; /* SPxObjectDetector */

#endif /* _SPX_OBJECT_DETECTOR_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
