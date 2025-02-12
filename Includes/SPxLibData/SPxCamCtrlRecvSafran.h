/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlRecvSafran class - a class
*	for receiving and responding to Safran LRF camera control messages.
*
* Revision Control:
*  27/09/22 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAM_CTRL_RECV_SAFRAN_H
#define _SPX_CAM_CTRL_RECV_SAFRAN_H

/* For SOCKET_TYPE. */
#include "SPxLibUtils/SPxCommon.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlRecv.h"

/* Forward declarations. */
class SPxPacketDecoder;
struct sockaddr_in;
struct SPxTime_tag;

class SPxCamCtrlRecvSafran : public SPxCamCtrlRecv
{
public:
    SPxCamCtrlRecvSafran(void);
    virtual ~SPxCamCtrlRecvSafran(void);

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
		      SOCKET_TYPE sock,
		      int sendPrompt);

    SPxErrorCode validateChecksum(const char *msg);

}; /* SPxCamCtrlRecvSafran */

#endif /* _SPX_CAM_CTRL_RECV_SAFRAN_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
