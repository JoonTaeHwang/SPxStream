/*********************************************************************
*
* (c) Copyright 2017 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxVision4ce class - a class
*	to communicate with a Vision4ce video tracker.
*
* Revision Control:
*   11/03/22 v1.29   AGC	Pass track altitude to SendTrack().
*
* Previous Changes:
*   08/10/21 1.28   AGC	Support sensor specific presets.
*   26/09/19 1.27   AGC	Support track reporting.
*   25/03/19 1.26   AGC	Return change time from GetCamFocusPos().
*   30/11/18 1.25   AGC	Support secondary sensor detection size.
*   19/11/18 1.24   AGC	Add single/dual control option.
*   05/10/18 1.23   AGC	Support inhibition of auto-acquisition.
*   05/10/18 1.22   AGC	Support minimum auto-acquisition time.
*   21/08/18 1.21   AGC	Fix compiler warnings.
*   21/08/18 1.20   AGC	Improve boresight/detection priority behaviour.
*   06/08/18 1.19   AGC	Support separate sensor tracker settings.
*   31/07/18 1.18   AGC	Support max auto-acquire distance.
*			Support setting input sensor.
*			Support setting zoom mode.
*   29/05/18 1.17   AGC	Support multiple re-sends.
*   14/03/18 1.16   AGC	Support Linux build.
*   14/03/18 1.15   AGC	Support format warnings with clang on Windows.
*			Remove unused functions.
*   06/03/18 1.14   AGC	Support writeDebug() format warnings.
*   05/02/18 1.13   AGC	Use short coast time during acquisition.
*   18/01/18 1.12   AGC	Re-initiate track list receipt.
*			Avoid sending director commands while video tracking.
*   14/11/17 1.11   AGC	Resend set commands when device busy.
*   23/10/17 1.10   AGC	Avoid possible lock-up with IsConnected().
*   11/09/17 1.9    AGC	Improve absolute command completion detection.
*   31/08/17 1.8    AGC	Rename various detection functions.
*			Remove zoom mode options.
*			Support track handler callback.
*   31/07/17 1.7    AGC	Remove GetDirMaxAzi/EleRate().
*   28/07/17 1.6    AGC	Support presets.
*   18/07/17 1.5    AGC	Support track list port config.
*   13/07/17 1.4    AGC	Add GetTrackTargetID().
*   29/06/17 1.3    AGC	Add Zoom mode support.
*			Add secondary detection support.
*   27/06/17 1.2    AGC	Add Auto target polarity.
*   22/06/17 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef SPX_VISION4CE_H_
#define SPX_VISION4CE_H_

/* For base class. */
#include "SPxLibNet/SPxVideoTrackerExt.h"

/* For SPxArea. */
#include "SPxLibUtils/SPxArea.h"

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* Forward declarations. */
class SPxPacketDecoder;

class SPxVision4ce : public SPxVideoTrackerExt
{
public:
    enum SubSystem
    {
	SUB_SYSTEM_ID_SYSTEM = 0x01,
	SUB_SYSTEM_ID_DIRECTOR = 0x02,
	SUB_SYSTEM_ID_CAMERA = 0x03,
	SUB_SYSTEM_ID_TRACKER = 0x05
    };
    enum SystemMode
    {
	SYSTEM_MODE_STANDBY = 0x00,
	SYSTEM_MODE_MANUAL = 0x01,
	SYSTEM_MODE_ACQUIRE = 0x02,
	SYSTEM_MODE_TRACK = 0x03,
	SYSTEM_MODE_SCAN = 0x04,
	SYSTEM_MODE_PARK = 0x06,
	SYSTEM_MODE_REMOTE = 0x08,
	SYSTEM_MODE_WASH = 0x0A
    };
    enum TargetType
    {
	TARGET_TYPE_AIR = 0,
	TARGET_TYPE_LAND = 1,
	TARGET_TYPE_SEA = 2,
	TARGET_TYPE_CUSTOM = 3
    };
    enum TrackMode
    {
	TRACK_MODE_DETECT = 0x00,
	TRACK_MODE_TRACK = 0x01,
	TRACK_MODE_COAST = 0x02
    };
    enum DetectAlg
    {
	DETECT_ALG_MANUAL = 0x00,
	DETECT_ALG_HOT_SPOT = 0x01,
	DETECT_ALG_MOTION = 0x02,
	DETECT_ALG_NAVAL = 0x03
    };
    enum DetectPriority
    {
	DETECT_PRIORITY_NONE = 0x00,
	DETECT_PRIORITY_SIZE = 0x01,
	DETECT_PRIORITY_BORESIGHT = 0x02,
	DETECT_PRIORITY_COMBINED = 0x03
    };
    enum TrackAlg
    {
	TRACK_ALG_COMBINED = 0x00,
	TRACK_ALG_CORRELATION = 0x01,
	TRACK_ALG_MTT = 0x02,
	TRACK_ALG_CENTROID = 0x03,
	TRACK_ALG_SCENELOCK = 0x04
    };
    enum TargetPolarity
    {
	TARGET_POLARITY_POS = 0x00,
	TARGET_POLARITY_NEG = 0x01,
	TARGET_POLARITY_BI = 0x02,
	TARGET_POLARITY_AUTO = 0x03
    };
    enum ObjStatus
    {
	OBJ_STATUS_INVALID = 0x00,
	OBJ_STATUS_VALID = 0x01,
	OBJ_STATUS_COAST = 0x02
    };

    struct DetectInfo
    {
	UINT32 id;
	INT32 cx;
	INT32 cy;
	UINT32 width;
	UINT32 height;
	int coasting;
    };

    struct TrackInfo
    {
	float cx;
	float cy;
	float width;
	float height;
	int coasting;
    };

    typedef void(*DetectFn)(const DetectInfo *detectInfo);
    typedef void (*TrackFn)(const TrackInfo *trackInfo);

    /* Construction and destruction. */
    SPxVision4ce(void);
    virtual ~SPxVision4ce(void);

    /* Address/port. */
    virtual SPxErrorCode SetAddress(const char *address, int port = 0, const char *ifAddr = NULL) SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0, const char *ifAddr = NULL) SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode SetAddress(const char *address, int port, const char *ifAddr, int initialise);
    virtual UINT32 GetAddress(void) const SPX_VIRTUAL_OVERRIDE;
    virtual const char *GetAddressString(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetPort(void) const SPX_VIRTUAL_OVERRIDE;
    virtual UINT32 GetIfAddress(void) const SPX_VIRTUAL_OVERRIDE;
    virtual const char *GetIfAddressString(void) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode SetTrackListPort(int port);
    int GetTrackListPort(void) const;
    SPxErrorCode SetTrackRptPort(int port);
    int GetTrackRptPort(void) const;

    int IsConnected(int reapplyConfig=TRUE);
    SPxErrorCode StartCommandQueue(SubSystem subSystem,
				   UINT8 subSystemIndex);
    SPxErrorCode SendCommandQueue(void);

    SPxErrorCode SetAutoAcquisition(int enable);
    int IsAutoAcquisitionEnabled(void) const;

    SPxErrorCode SetMinAutoAcquisitionTime(double seconds);
    SPxErrorCode GetMinAutoAcquisitionTime(double *seconds) const;

    SPxErrorCode SetMaxAutoAcquisitionDistance(unsigned int distPx);
    SPxErrorCode GetMaxAutoAcquisitionDistance(unsigned int *distPx) const;

    SPxErrorCode InhibitAutoAcqusition(int inhibit);

    SPxErrorCode SetActiveAreaZoom(int enable);
    int IsActiveAreaZoomEnabled(void) const;

    SPxErrorCode SetRadarFusion(int enable);
    int IsRadarFusionEnabled(void) const;

    SPxErrorCode SetSlaveCamera(int slaveCamera);
    int IsSlavingCamera(void) const;

    SPxErrorCode SetDualCameras(int dualCameras);
    int IsDualCameras(void) const;

    /* Track handler. */
    template<typename F, typename O>
    SPxErrorCode InstallHandlerTracks(F fn, O *obj)
    {
	return m_trackCallback.Add(fn, obj);
    }
    template<typename F, typename O>
    SPxErrorCode RemoveHandlerTracks(F fn, O *obj)
    {
	return m_trackCallback.Remove(fn, obj);
    }

    /* System control. */
    SPxErrorCode SetSysMode(UINT8 subSystemIndex, SystemMode sysMode);
    SPxErrorCode GetSysMode(UINT8 subSystemIndex, SystemMode *sysMode) const;
    SPxErrorCode SetSysSensor(UINT8 subSystemIndex, UINT8 sensor);
    SPxErrorCode GetSysSensor(UINT8 subSystemIndex, UINT8 *sensor) const;
    SPxErrorCode GetSysSoftwareVersion(UINT8 subSystemIndex, char *buffer, unsigned int bufLen) const;

    /* Director control. */
    int IsDirConnected(UINT8 subSystemIndex) const;
    SPxErrorCode SetDirAziPos(UINT8 subSystemIndex, float aziPosMrads);
    SPxErrorCode GetDirAziPos(UINT8 subSystemIndex, float *aziPosMrads, UINT32 *changeTime = NULL) const;
    SPxErrorCode SetDirElePos(UINT8 subSystemIndex, float elePosMrads);
    SPxErrorCode GetDirElePos(UINT8 subSystemIndex, float *elePosMrads, UINT32 *changeTime = NULL) const;
    SPxErrorCode SetDirAziRate(UINT8 subSystemIndex, float aziRateMradsPerSec);
    SPxErrorCode SetDirEleRate(UINT8 subSystemIndex, float eleRateMradsPerSec);
    SPxErrorCode SetDirPreset(UINT8 subSystemIndex, UINT8 id);

    /* Camera control. */
    int IsCamConnected(UINT8 subSystemIndex) const;
    SPxErrorCode SetCamFocusPos(UINT8 subSystemIndex, UINT16 focusPos);
    SPxErrorCode GetCamFocusPos(UINT8 subSystemIndex, UINT16 *focusPos, UINT32 *changeTime = NULL) const;
    SPxErrorCode SetCamZoomFov(UINT8 subSystemIndex, INT16 fovMrads);
    SPxErrorCode GetCamZoomFov(UINT8 subSystemIndex, INT16 *fovMrads, UINT32 *changeTime = NULL) const;
    SPxErrorCode SetCamFocusRate(UINT8 subSystemIndex, INT16 focusRate);
    SPxErrorCode SetCamZoomRate(UINT8 subSystemIndex, INT16 zoomRate);
    SPxErrorCode SetCamZoomMode(UINT8 subSystemIndex, UINT16 zoomMode);
    SPxErrorCode GetCamZoomMode(UINT8 subSystemIndex, UINT16 *zoomMode) const;
    SPxErrorCode SetCamFocusMode(UINT8 subSystemIndex, UINT8 focusMode);
    SPxErrorCode GetCamFocusMode(UINT8 subSystemIndex, UINT8 *focusMode) const;
    SPxErrorCode SetCamBoresight(UINT8 subSystemIndex, UINT16 posX, UINT16 posY);
    SPxErrorCode SetCamBoresightX(UINT8 subSystemIndex, UINT16 posX);
    SPxErrorCode SetCamBoresightY(UINT8 subSystemIndex, UINT16 posY);
    SPxErrorCode GetCamBoresight(UINT8 subSystemIndex, UINT16 *posX, UINT16 *posY) const;
    SPxErrorCode SetCamPreset(UINT8 subSystemIndex, UINT8 id);

    /* Video tracking control. */
    virtual int IsTrackingEnabled(UINT8 subSystemIndex) const SPX_VIRTUAL_OVERRIDE;
    virtual int IsTracking(UINT8 subSystemIndex) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode SetTrackTargetType(UINT8 subSystemIndex, TargetType targetType, int isPrimary = TRUE);
    SPxErrorCode GetTrackTargetType(UINT8 subSystemIndex, TargetType *targetType, int isPrimary = TRUE) const;
    SPxErrorCode GetTrackMode(UINT8 subSystemIndex, TrackMode *trackMode) const;
    SPxErrorCode SetTrackTargetID(UINT8 subSystemIndex, UINT32 targetID);
    SPxErrorCode GetTrackTargetID(UINT8 subSystemIndex, UINT32 *targetID) const;
    SPxErrorCode SetTrackDetectPriority(UINT8 subSystemIndex, DetectPriority detectPriority, int isPrimary = TRUE);
    SPxErrorCode GetTrackDetectPriority(UINT8 subSystemIndex, DetectPriority *detectPriority, int isPrimary = TRUE) const;
    SPxErrorCode SetTrackDetectAlgorithm(UINT8 subSystemIndex, DetectAlg detectAlg, int isPrimary = TRUE);
    SPxErrorCode GetTrackDetectAlgorithm(UINT8 subSystemIndex, DetectAlg *detectAlg, int isPrimary = TRUE) const;
    SPxErrorCode SetTrackAlgorithm(UINT8 subSystemIndex, TrackAlg alg, int isPrimary = TRUE);
    SPxErrorCode GetTrackAlgorithm(UINT8 subSystemIndex, TrackAlg *alg, int isPrimary = TRUE) const;
    SPxErrorCode SetTrackTargetPolarity(UINT8 subSystemIndex, TargetPolarity targetPolarity, int isPrimary = TRUE);
    SPxErrorCode GetTrackTargetPolarity(UINT8 subSystemIndex, TargetPolarity *targetPolarity, int isPrimary = TRUE) const;
    SPxErrorCode SetTrackAcquireCoastDuration(UINT8 subSystemIndex, float coastSecs);
    SPxErrorCode GetTrackAcquireCoastDuration(UINT8 subSystemIndex, float *coastSecs) const;
    SPxErrorCode SetTrackCoastDuration(UINT8 subSystemIndex, float coastSecs);
    SPxErrorCode GetTrackCoastDuration(UINT8 subSystemIndex, float *coastSecs) const;
    SPxErrorCode SetTrackDetectPos(UINT8 subSystemIndex, INT16 posX, INT16 posY, int isPrimary = TRUE);
    SPxErrorCode GetTrackDetectPos(UINT8 subSystemIndex, INT16 *posX, INT16 *posY, int isPrimary = TRUE) const;
    SPxErrorCode SetTrackDetectSize(UINT8 subSystemIndex, UINT16 sizeW, UINT16 sizeH, int isPrimary = TRUE);
    SPxErrorCode GetTrackDetectSize(UINT8 subSystemIndex, UINT16 *sizeW, UINT16 *sizeH, int isPrimary = TRUE) const;
    SPxErrorCode GetTrackObjectStatus(UINT8 subSystemIndex, ObjStatus *objectStatus) const;
    SPxErrorCode GetTrackObjectPos(UINT8 subSystemIndex, float *posX, float *posY) const;
    SPxErrorCode GetTrackObjectSize(UINT8 subSystemIndex, float *sizeW, float *sizeH) const;

    template<typename F, typename O>
    SPxErrorCode IterateDetections(F fn, O *obj) const
    {
	SPxEventHandler<DetectFn> handler;
	handler.Add(fn, obj);
	return IterateDetections(handler);
    }
    SPxErrorCode IterateDetections(const SPxEventHandler<DetectFn>& handler) const;
    SPxErrorCode FindNearestDetection(UINT8 subSystemIndex,
				      UINT32 x, UINT32 y, float marginPx, UINT32 *id) const;

    SPxErrorCode SetMaxResends(unsigned int maxResends);
    unsigned int GetMaxResends(void) const;

    virtual SPxErrorCode SendTrack(UINT32 id,
				   double rangeMetres,
				   double bearingDegs,
				   double elevationDegs,
				   double altitudeMetres,
				   const char *name,
				   int isRadarTrackingEnabled,
				   int isVideoTrackingEnabled) SPX_VIRTUAL_OVERRIDE;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue) SPX_VIRTUAL_OVERRIDE;
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen) SPX_VIRTUAL_OVERRIDE;

    void SetLogFile(const char *logFileName);
    const char *GetLogFile(void) const;
    void SetVerbosity(UINT32 verbosity);
    UINT32 GetVerbosity(void) const;

private:
    /* Private variables. */
    struct StatusMsg;
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxEventHandler<TrackFn> m_trackCallback;

    void InitTracker(void);

    SPxErrorCode setAddress(const char *address, int port,
			    const char *ifAddr,
			    int trackListPort,
			    int initialise=TRUE);
    void initTracker(void);
    int isConnected(void);
    int isTracking(UINT8 subSystemIndex) const;
    SPxErrorCode setTrackTargetID(UINT8 subSystemIndex, UINT32 targetID);
    void enableTrackOutput(void);
    void checkTrackOutput(void);
    void checkAutoAcquire(void);
    static SPxErrorCode validateChecksum(const unsigned char *payload,
					 unsigned int numBytes);
    static void calcChecksum(unsigned char *payload,
			     unsigned int numBytes);
    void handleStatusSystem(const StatusMsg *status,
			    const unsigned char *data,
			    unsigned int dataBytes);
    void handleStatusDirector(const StatusMsg *status,
			      const unsigned char *data,
			      unsigned int dataBytes);
    void handleStatusCamera(const StatusMsg *status,
			    const unsigned char *data,
			    unsigned int dataBytes);
    void handleStatusTracker(const StatusMsg *status,
			     const unsigned char *data,
			     unsigned int dataBytes);

    static void netHandlerWrapper(SPxPacketDecoder *pktDecoder,
				  void *userArg,
				  UINT32 packetType,
				  struct sockaddr_in *from,
				  struct SPxTime_tag *time,
				  const unsigned char *payload,
				  unsigned int numBytes);
    void netHandler(const unsigned char *payload,
		    unsigned int numBytes);

    static void netTrackHandlerWrapper(SPxPacketDecoder *pktDecoder,
				       void *userArg,
				       UINT32 packetType,
				       struct sockaddr_in *from,
				       struct SPxTime_tag *time,
				       const unsigned char *payload,
				       unsigned int numBytes);
    void netTrackHandler(const unsigned char *payload,
			 unsigned int numBytes);

    void
#if defined(__GNUC__) || defined(__clang__)
	__attribute__((format(printf, 3, 4)))
#endif
    writeDebug(UINT32 verbosity, const char *format, ...) const;

    template<typename T>
    SPxErrorCode readVal(int dataItemID,
			 const unsigned char*& data,
			 unsigned int& dataBytes, T& val,
			 UINT32* changeTime);

    SPxErrorCode readVal(int dataItemID,
			 const unsigned char*& data,
			 unsigned int& dataBytes, float& val,
			 UINT32* changeTime);

    template<typename T>
    SPxErrorCode set(SubSystem subSystemID, UINT8 subSystemIndex,
		     UINT8 dataItemID, T val);

    SPxErrorCode set(SubSystem subSystemID, UINT8 subSystemIndex,
		     UINT8 dataItemID, float val);

    SPxErrorCode get(SubSystem subSystemID, UINT8 subSystemIndex,
		     UINT8 dataItemID);

    SPxErrorCode send(SubSystem subSystemID, UINT8 subSystemIndex,
		      const unsigned char *data,
		      unsigned int dataBytes);

    SPxErrorCode requestPeriodic(SubSystem subSystemID, UINT8 subSystemIndex,
				 const UINT8 *dataItems);

    void removeResend(SubSystem subSystemID, UINT8 subSystemIndex,
		      UINT8 dataItemID);
    void resendMessages(void);

}; /* SPxVision4ce */

#endif /* SPX_VISION4CE_H_ */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
