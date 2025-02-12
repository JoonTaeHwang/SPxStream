/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlRecvVision4ce class - a class
*	for receiving and responding to Vision4ce camera control messages.
*
* Revision Control:
*   25/06/19 v1.7    AGC	Add detections port parameter.
*
* Previous Changes:
*   19/11/18 1.6    AGC	Add single/dual control option.
*   22/10/18 1.5    AGC	Suppress erroneous cppcheck warnings.
*   28/09/18 1.4    AGC	Add support for shared control.
*   21/08/18 1.3    AGC	Add parameters for tracking limits and dropped messages.
*   20/08/18 1.2    AGC	Use track database for video tracks.
*   17/08/18 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_RECV_VISION4CE_H
#define _SPX_CAM_CTRL_RECV_VISION4CE_H

/* For SOCKET_TYPE. */
#include "SPxLibUtils/SPxCommon.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlRecv.h"

/* Forward declarations. */
class SPxNetTCP;
class SPxPacketDecoder;
struct sockaddr_in;
struct SPxTime_tag;

class SPxCamCtrlRecvVision4ce : public SPxCamCtrlRecv
{
public:
    SPxCamCtrlRecvVision4ce(void);
    virtual ~SPxCamCtrlRecvVision4ce(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetSrcType(SPxMesgIOType_t srcType);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetTrackDatabase(SPxTrackDatabase *trackDB);
    virtual SPxErrorCode GetPrimarySensor(unsigned int *primarySensor) const;

    SPxErrorCode SetDropCamIntervalMS(unsigned int ms);
    UINT32 GetDropCamIntervalMS(void) const;
    SPxErrorCode SetMaxTrackTimeMS(unsigned int ms);
    UINT32 GetMaxTrackTimeMS(void) const;
    SPxErrorCode SetMaxCoastTimeMS(unsigned int ms);
    UINT32 GetMaxCoastTimeMS(void) const;
    SPxErrorCode SetSharedControl(int sharedControl);
    int IsSharedControl(void) const;
    SPxErrorCode SetDualControl(int dualControl);
    int IsDualControl(void) const;
    SPxErrorCode SetDetectionsPort(int detectionsPort);
    int GetDetectionsPort(void) const;

    /* Generic parameter setting/getting. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void handleMesg(SPxPacketDecoder *pktDecoder,
			   void *userArg,
			   UINT32 packetType,
			   struct sockaddr_in *from,
			   struct SPxTime_tag *time,
			   const unsigned char *payload,
			   unsigned int numBytes);

    UINT8 handleSystemMesg(const unsigned char *payload,
			   unsigned int numBytes);

    UINT8 handleDirectorMesg(const unsigned char *payload,
			     unsigned int numBytes);

    UINT8 handleCameraMesg(UINT8 index,
			   const unsigned char *payload,
			   unsigned int numBytes);

    UINT8 handleTrackerMesg(const unsigned char *payload,
			    unsigned int numBytes);

    UINT8 readSystemMesg(UINT8 dataItem);
    UINT8 readDirectorMesg(UINT8 dataItem);
    UINT8 readCameraMesg(UINT8 index, UINT8 dataItem);
    UINT8 readTrackerMesg(UINT8 dataItem);

    void addToReply(UINT8 val);
    void addToReply(const UINT8 *begin, unsigned int numBytes);

    void reply(UINT8 subSystemID, UINT8 index,
	       UINT8 status,
	       const unsigned char *data=NULL,
	       unsigned int numBytes=0);

    void periodicSystemMesg(void);
    void periodicDirectorMesg(void);
    void periodicCameraMesg(void);
    void periodicTrackerMesg(void);

    static void startDetectionHandler(SPxPacketDecoder *pktDecoder,
				      void *userArg,
				      UINT32 packetType,
				      struct sockaddr_in *from,
				      struct SPxTime_tag *time,
				      const unsigned char *payload,
				      unsigned int numBytes);
    void sendDetections(void);

    void updateTargets(void);

    static UINT8 calcChecksum(const unsigned char *data,
			      unsigned int numBytes);

    static int trackUpdated(void *invokingObject,
			    void *userObject,
			    void *arg);

}; /* SPxCamCtrlRecvVision4ce */

#endif /* _SPX_CAM_CTRL_RECV_VISION4CE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
