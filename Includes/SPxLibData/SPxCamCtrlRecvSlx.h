/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlRecvSlx class - a class
*	for receiving and responding to SLX camera control messages.
*
* Revision Control:
*   27/04/22 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAM_CTRL_RECV_SLX_H
#define _SPX_CAM_CTRL_RECV_SLX_H

/* For SOCKET_TYPE. */
#include "SPxLibUtils/SPxCommon.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlRecv.h"

/* Forward declarations. */
class SPxPacketDecoder;
struct sockaddr_in;
struct SPxTime_tag;

class SPxCamCtrlRecvSlx : public SPxCamCtrlRecv
{
public:
    SPxCamCtrlRecvSlx(void);
    virtual ~SPxCamCtrlRecvSlx(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetSrcType(SPxMesgIOType_t srcType);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void handleMesgWrapper(SPxPacketDecoder *pktDecoder,
				  void *userArg,
				  UINT32 packetType,
				  struct sockaddr_in *from,
				  struct SPxTime_tag *time,
				  const unsigned char *payload,
				  unsigned int numBytes);

    void handleMesg(struct SPxTime_tag *time,
		    const char *data,
		    unsigned int numBytes,
		    SOCKET_TYPE sock);

    void sendResponse(const char *msg,
		      const char *val,
		      SOCKET_TYPE sock);

}; /* SPxCamCtrlRecvSlx */

#endif /* _SPX_CAM_CTRL_RECV_SLX_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
