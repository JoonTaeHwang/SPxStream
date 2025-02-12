/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlRecvNexus class - a class
*	for receiving and responding to Nexus camera control messages.
*
* Revision Control:
*   22/10/18 v1.2    AGC	Suppress erroneous cppcheck warning.
*
* Previous Changes:
*   10/08/18 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_RECV_NEXUS_H
#define _SPX_CAM_CTRL_RECV_NEXUS_H

/* For SOCKET_TYPE. */
#include "SPxLibUtils/SPxCommon.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlRecv.h"

/* Forward declarations. */
class SPxNetTCP;
class SPxPacketDecoder;
struct sockaddr_in;
struct SPxTime_tag;

class SPxCamCtrlRecvNexus : public SPxCamCtrlRecv
{
public:
    SPxCamCtrlRecvNexus(void);
    virtual ~SPxCamCtrlRecvNexus(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetSrcType(SPxMesgIOType_t srcType);

protected:
    explicit SPxCamCtrlRecvNexus(SPxNetTCP *tcp);

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

    void sendJson(SOCKET_TYPE sock);

}; /* SPxCamCtrlRecvNexus */

#endif /* _SPX_CAM_CTRL_RECV_NEXUS_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
