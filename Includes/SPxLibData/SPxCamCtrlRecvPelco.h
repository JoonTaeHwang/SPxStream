/*********************************************************************
*
* (c) Copyright 2018, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlRecvPelco class - a class
*	for receiving and responding to Pelco camera control messages.
*
* Revision Control:
*   14/01/21 v1.4    AGC	Support dual Pelco control.
*
* Previous Changes:
*   22/10/18 1.3    AGC	Suppress erroneous cppcheck warning.
*   17/08/18 1.2    AGC	Move SetSrcType() here from base class.
*   10/08/18 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_RECV_PELCO_H
#define _SPX_CAM_CTRL_RECV_PELCO_H

/* For SOCKET_TYPE. */
#include "SPxLibUtils/SPxCommon.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlRecv.h"

/* Forward declarations. */
class SPxNetTCP;
class SPxPacketDecoder;
struct sockaddr_in;
struct SPxTime_tag;

class SPxCamCtrlRecvPelco : public SPxCamCtrlRecv
{
public:
    SPxCamCtrlRecvPelco(void);
    virtual ~SPxCamCtrlRecvPelco(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetSrcType(SPxMesgIOType_t srcType);

    SPxErrorCode SetCameraAddress(UINT8 camAddress, int isPrimary = TRUE);

protected:
    explicit SPxCamCtrlRecvPelco(SPxNetTCP *tcp);

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

    void handlePelcoDMesg(struct SPxTime_tag *time,
			  const unsigned char *data,
			  unsigned int numBytes,
			  SOCKET_TYPE sock);

    void handlePelcoDExtMesg(struct SPxTime_tag *time,
			     const unsigned char *data,
			     unsigned int numBytes,
			     SOCKET_TYPE sock);

    void handlePelcoDQueryMesg(struct SPxTime_tag *time,
			       const unsigned char *data,
			       unsigned int numBytes,
			       SOCKET_TYPE sock);

    void sendResponse(const unsigned char *data,
		      unsigned int numBytes,
		      SOCKET_TYPE sock);

    void sendPelcoDGenResponse(UINT8 addr, SOCKET_TYPE sock);

    void sendPelcoDExtResponse(UINT8 addr,
			       UINT16 opcode,
			       UINT16 value,
			       SOCKET_TYPE sock);

}; /* SPxCamCtrlRecvPelco */

#endif /* _SPX_CAM_CTRL_RECV_PELCO_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
