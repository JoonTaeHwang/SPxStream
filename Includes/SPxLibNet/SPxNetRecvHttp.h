/*********************************************************************
*
* (c) Copyright 2015, 2016, 2019, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for HTTP-based network receiving module.
*
*
* Revision Control:
 *  26/08/22 v1.13   AGC	Suppress spurious cppcheck warnings.
*
* Previous Changes:
*   25/06/19 1.12   AGC	Derive from SPxPlotSourceIface.
*   02/09/16 1.11   AGC	Support testing.
*   23/08/16 1.10   AGC	Improvements to radar video output.
*   03/03/16 1.9    AGC	Improvements to record/replay.
*   01/02/16 1.8    AGC	Support detections.
*   03/11/15 1.7    AGC	Packet decoder callback data now const.
*   06/08/15 1.6    AGC	Add blanking region support.
*   15/05/15 1.5    AGC	Use geolocation message.
*   11/05/15 1.4    AGC	Support async gps/compass retrieval.
*   29/04/15 1.3    AGC	Support recording.
*			Support UDP input.
*   22/04/15 1.2    REW	Fix typo. Support NavData links etc.
*   25/03/15 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECVHTTP_H
#define _SPX_NETRECVHTTP_H

/*
 * Other headers required.
 */
/* We need the base class header (which includes several others as well). */
#include "SPxLibNet/SPxNetRecv.h"
#include "SPxLibData/SPxPlotSourceIface.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declare any classes we use. */
class SPxRIB;
class SPxNavData;
class SPxTimer;
class SPxRunProcess;
class SPxPacketDecoderNet;
class SPxPacketDecoder;
class SPxActiveRegion;
class SPxCallbackList;
class SPxRadarReplay;

/*
 * Define our specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveHttp :public SPxNetworkReceive, public SPxPlotSourceIface
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    explicit SPxNetworkReceiveHttp(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveHttp();
    virtual void Enable(int state);
    virtual UINT32 GetMsecsSinceLastReturn(void) { return 0; }
    virtual UINT32 GetMsecsSinceLastAziChange(void) { return 0; }
    virtual SPxErrorCode GetInitialised(void);

    /* UDP input functions. */
    SPxErrorCode EnableUDP(int enable);
    int IsUDPEnabled(void) const { return m_enableUDP; }
    SPxErrorCode SetUDPAddress(const char *addr, int port=0,
			       const char *ifAddr=NULL);
    SPxErrorCode SetUDPAddress(UINT32 addr, int port=0,
			       const char *ifAddr=NULL);
    UINT32 GetUDPAddress(void) const;
    const char *GetUDPAddressString(void) const;
    SPxErrorCode SetUDPPort(int port);
    virtual int GetUDPPort(void) const;
    virtual UINT32 GetUDPIfAddress(void) const;
    virtual const char *GetUDPIfAddressString(void) const;

    /* Configuration functions. */
    SPxErrorCode SetProtocol(unsigned int protocol);
    unsigned int GetProtocol(void) const { return(m_protocol); }
    SPxErrorCode SetPollTracksMsecs(unsigned int msecs);
    unsigned int GetPollTracksMsecs(void) const { return(m_pollTracksMsecs); }
    SPxErrorCode SetPollGeoMsecs(unsigned int msecs);
    unsigned int GetPollGeoMsecs(void) const { return(m_pollGeoMsecs); }
    SPxErrorCode SetURLSuffix(const char *suffix);
    const char *GetURLSuffix(void) const { return(m_urlSuffix); }

    SPxErrorCode SetBlankingRegion(SPxActiveRegion *region);

    /* Navigation links. */
    SPxErrorCode SetNavData(SPxNavData *navData);
    SPxNavData *GetNavData(void) const	{ return(m_navData); }
    SPxErrorCode SetPassLatLongToNavData(int enable);
    int GetPassLatLongToNavData(void) const { return(m_passLatLongToNavData); }
    SPxErrorCode SetPassHeadingToNavData(int enable);
    int GetPassHeadingToNavData(void) const { return(m_passHeadingToNavData); }
    
    /* Status functions.  */
    double GetRadarHeading(void) const	{ return(m_radarHeadingDegs); }
    double GetRadarLatDegs(void) const	{ return(m_radarLatDegs); }
    double GetRadarLongDegs(void) const	{ return(m_radarLongDegs); }
    SPxErrorCode GetRadarModel(char *buf, unsigned int bufLen, int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode GetRadarSerial(char *buf, unsigned int bufLen, int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode GetRadarVersion(char *buf, unsigned int bufLen, int mutexAlreadyLocked=FALSE) const;

    /* Callback handling. */
    virtual SPxErrorCode AddPlotCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode RemovePlotCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode AddTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode AddStatusCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode RemoveStatusCallback(SPxCallbackListFn_t fn, void *userObj);
    
    /* Input from file replay. */
    SPxErrorCode SetReplaySource(SPxRadarReplay *replaySource);
    
    /* Output to recording process. */
    SPxErrorCode SetRecordingProcess(SPxRunProcess *recProc);

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /* Overwrite the base class functions we want to handle ourselves. */
    virtual int ThreadReadDataFn(unsigned char *buf,
				unsigned int bufSizeBytes,
				unsigned int timeoutMsecs);
    virtual int ThreadBackgroundFn(void);

protected:
    /* Functions that may be overridden for testing. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode downloadFile(const char *url,
				      unsigned char *buffer,
				      unsigned int *bufferSizeBytes,
				      unsigned int timeoutMsecs);

    void setTrackBuffer(char *buffer, unsigned int bufSizeBytes);
    void readDetections(void);
    void readTracks(void);
    void readGeo(void);
    void readFov(void);

private:
    /*
     * Private fields.
     */
    /* Protocol. */
    unsigned int m_protocol;		/* 0, 1, ... */

    /* Rates. */
    unsigned int m_pollTracksMsecs;	/* Time between updates */
    unsigned int m_pollGeoMsecs;	/* Time between updates */

    /* Timer. */
    SPxTimer *m_timer;
    char *m_buffer;			/* Buffer from base class - only use for tracks. */
    unsigned int m_bufSizeBytes;
    SPxAutoPtr<char[]> m_bufferGeo;	/* Buffer for GPS/rotation data. */
    unsigned int m_bufGeoSizeBytes;
    SPxAutoPtr<char[]> m_bufferFov;	/* Buffer for GPS/rotation data. */
    unsigned int m_bufFovSizeBytes;
    int m_readingGeo;
    int m_readingFov;
    int m_lastReadFailed;

    /* UDP input. */
    int m_enableUDP;
    SPxPacketDecoderNet *m_pktDecoder;

    /* Configuration options. */
    char *m_urlSuffix;			/* Suffix for requests */
    SPxActiveRegion *m_blankRegion;	/* Blanking region */

    /* Information from radar. */
    double m_radarHeadingDegs;		/* Heading of radar */
    double m_radarLatDegs;		/* Latitude */
    double m_radarLongDegs;		/* Longitude */
    int m_gotStatus;			/* Status received? */
    SPxAutoPtr<char[]> m_model;		/* Radar status */
    SPxAutoPtr<char[]> m_serial;
    SPxAutoPtr<char[]> m_version;
    int m_gotEndRange;			/* End range received? */
    double m_endRangeMetres;		/* Radar end range. */


    /* Navigation info.  */
    SPxNavData *m_navData;		/* NavData object to update, or NULL */
    int m_passLatLongToNavData;		/* Flag to say update the NavData */
    int m_passHeadingToNavData;		/* Flag to say update the NavData */

    /* Callbacks for events. */
    SPxAutoPtr<SPxCallbackList> m_plotCallbacks;
    int m_numTrackCallbacks;			    /* Number of track callbacks. */
    SPxAutoPtr<SPxCallbackList> m_trackCallbacks;
    SPxAutoPtr<SPxCallbackList> m_statusCallbacks;

    /* Replay. */
    SPxRadarReplay *m_replay;

    /* Recording. */
    SPxRunProcess *m_recProc;

    /*
     * Private functions.
     */
    void readGeoAsync(void);
    void readFovAsync(void);
    void processDetections(const char *data, unsigned int numBytes);
    void processTracks(const char *data, unsigned int numBytes);
    void processGeo(const char *data, unsigned int numBytes);
    void processFov(const char *data, unsigned int numBytes);
    void processStatus(const char *data, unsigned int numBytes);

    /* Decoding functions. */
    static void decodeWrapper(SPxPacketDecoder *pktDecoder, void *userArg, 
	UINT32 packetType, struct sockaddr_in *from, SPxTime_t *time, 
	const unsigned char *payload, unsigned int numBytes);
    void decode(const unsigned char *payload, unsigned int numBytes);
    SPxErrorCode decodeDetection(const char *buf, unsigned int numBytes);
    SPxErrorCode decodeTrack(const char *buf, unsigned int numBytes);

    /* Data access functions. */
    unsigned int getFile(const char *filename, char *buf,
			 unsigned int bufSizeBytes);
    SPxErrorCode getFieldValue(const char *buf, unsigned int numBytes,
			       const char *field, const char **startPtrPtr, int skipQuote);
    SPxErrorCode getFieldValue(const char *buf, unsigned int numBytes,
			       const char *field, char *str, unsigned int bufSizeBytes);
    SPxErrorCode getFieldValue(const char *buf, unsigned int numBytes,
			       const char *field, unsigned int *uintPtr);
    SPxErrorCode getFieldValue(const char *buf, unsigned int numBytes,
			       const char *field, int *intPtr);
    SPxErrorCode getFieldValue(const char *buf, unsigned int numBytes,
			       const char *field, double *doublePtr);

    SPxErrorCode recordJson(const char *file, const char *buf);

    /* Radar video functions. */
    SPxErrorCode outputSpoke(UINT16 azi16,
			     int includeTarget,
			     double rangeMetres);

}; /* SPxNetworkReceiveHttp class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVHTTP_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
