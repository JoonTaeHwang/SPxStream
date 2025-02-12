/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlCamMngr class - a class
*	to allow control of SPxCameraManager via an SPxCamera object.
*
* Revision Control:
*   25/07/22 v1.6    AGC	Update SetParamSocket().
*
* Previous Changes:
*   29/03/22 1.5    AGC	Correctly handle socket re-use.
*   17/03/22 1.4    AGC	Fix single connection controlling multiple cameras.
*   17/06/21 1.3    AGC	Disable base class camera manager rechecks.
*   22/03/21 1.2    AGC	Avoid const_cast.
*   16/03/21 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_CAM_MNGR_H
#define _SPX_CAM_CTRL_CAM_MNGR_H

/* For base class. */
#include "SPxLibData/SPxCamCtrlPelco.h"

/* Forward declarations. */
class SPxCameraManager;

class SPxCamCtrlCamMngr : public SPxCamCtrlPelco
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlCamMngr(SPxCameraManager *camMngr,
			       UINT8 cameraAddress);
    virtual ~SPxCamCtrlCamMngr(void);

    virtual SPxMesgIOType_t GetMesgType(void) const SPX_VIRTUAL_OVERRIDE { return SPX_MESG_IO_TYPE_NETWORK_RAW; }
    virtual int IsConnected(void) const SPX_VIRTUAL_OVERRIDE { return TRUE; }

    virtual void SetParamSocket(SOCKET_TYPE sock, UINT32 id, const char *clientID) SPX_VIRTUAL_OVERRIDE;

private:
    /* Private variables. */
    SPxCameraManager *m_camMngr;

    virtual SPxErrorCode SendMessage(const void *command,
				     unsigned int numBytes) const SPX_VIRTUAL_OVERRIDE;

    static unsigned int responseFn(const unsigned char *payload,
				   unsigned int payloadBytes,
				   void *userArg);

    /* Disable camera manager rechecks. */
    virtual int recheckCameraManager(void) const SPX_VIRTUAL_OVERRIDE { return FALSE; }

}; /* SPxCamCtrlCamMngr */

#endif /* _SPX_CAM_CTRL_CAM_MNGR_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
