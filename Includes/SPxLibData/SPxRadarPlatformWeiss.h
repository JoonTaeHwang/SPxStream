/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for SPxRadarPlatformWeiss class, a class for
*   controlling a Weiss radar platform.
*
* Revision Control:
*   26/10/18 v1.2    AGC	Support debug.
*				Support messages configuration.
*
* Previous Changes:
*   17/10/18 1.1    AGC	Initial version.
**********************************************************************/

#ifndef _SPX_RADAR_PLATFORM_WEISS_H
#define _SPX_RADAR_PLATFORM_WEISS_H

/* Standard headers. */

/* For base class. */
#include "SPxLibData/SPxRadarPlatform.h"

/* Forward declarations. */
class SPxPacketDecoder;
struct sockaddr_in;
struct SPxTime_tag;

class SPxRadarPlatformWeiss : public SPxRadarPlatform
{
public:
    /* Construction and destruction. */
    SPxRadarPlatformWeiss(void);
    virtual ~SPxRadarPlatformWeiss(void);

    SPxErrorCode SetAddress(const char *address);
    SPxErrorCode GetAddress(char *buf, int bufLen) const;
    SPxErrorCode SetSendPort(UINT16 sendPort);
    UINT16 GetSendPort(void) const;
    SPxErrorCode SetRecvPort(UINT16 recvPort);
    UINT16 GetRecvPort(void) const;
    SPxErrorCode SetInterfaceAddress(const char *ifAddress);
    SPxErrorCode GetInterfaceAddress(char *buf, int bufLen) const;

    SPxErrorCode SetUseNetworkByteOrder(int useNetByteOrder);
    int GetUseNetworkByteOrder(void) const;

    SPxErrorCode SetMessageLength(unsigned int numWords);
    unsigned int GetMessageLength(void) const;
    SPxErrorCode SetMessageBearingWord(unsigned int word);
    unsigned int GetMessageBearingWord(void) const;
    SPxErrorCode SetReqMessageValue(unsigned int word, UINT32 value);
    UINT32 GetReqMessageValue(unsigned int word) const;
    SPxErrorCode SetReqMessageMask(unsigned int word, UINT32 mask);
    UINT32 GetReqMessageMask(unsigned int word) const;

    virtual SPxErrorCode SetBearing(double bearingDegs);
    virtual SPxErrorCode GetBearing(double *bearingDegs) const;

    /* Parameter settings. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);
    virtual SPxErrorCode SetConfigFromState(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void timer(void);

    static void recvMesg(SPxPacketDecoder *decoder,
			 void *userArg,
			 UINT32 packetType,
			 struct sockaddr_in *from,
			 struct SPxTime_tag *time,
			 const unsigned char *payload,
			 unsigned int numBytes);

}; /* SPxRadarPlatformWeiss */

#endif /* _SPX_RADAR_PLATFORM_WEISS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
