/*********************************************************************
*
* (c) Copyright 2013, 2018, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for camera controller creation functions.
*
* Revision Control:
*   26/09/22 v1.4    AGC	Support laser dazzlers.
*
* Previous Changes:
*   04/03/22 1.3    AGC	Support SkyWall camera control.
*   30/05/18 1.2    AGC	Support Vision4ce camera control.
*   24/10/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_CREATE_H
#define _SPX_CAM_CTRL_CREATE_H

/* For SPxCameraType ernum. */
#include "SPxLibData/SPxCamCtrlBase.h"

/* Forward declarations. */
class SPxCamCtrlBase;
class SPxVision4ce;
class SPxSkyWallConn;
class SPxCamera;

/* Create a new camera controller. */
extern SPxCamCtrlBase *SPxCamCtrlCreate(SPxCameraType camType,
					SPxVision4ce *vision4ce=NULL,
					int saveVision4ceParams=FALSE,
					SPxSkyWallConn *skyWallConn=NULL,
					SPxCamera *camera=NULL,
					SPxCamCtrlBase *lrf=NULL);

#endif /* _SPX_CAM_CTRL_CREATE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
