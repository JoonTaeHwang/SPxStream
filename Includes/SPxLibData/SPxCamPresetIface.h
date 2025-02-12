/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamPresetIface class - an interface for
*	basic camera preset control.
*
* Revision Control:
*   10/08/18 v1.0    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAM_PRESET_IFACE_H
#define _SPX_CAM_PRESET_IFACE_H

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

class SPxCamPresetIface
{
public:
    /* Construction and destruction. */
    SPxCamPresetIface() {}
    virtual ~SPxCamPresetIface() {}

    virtual SPxErrorCode GoToPreset(UINT8 id) = 0;
    virtual SPxErrorCode GoToPreset(const char *preset) = 0;

}; /* SPxCamPresetIface */

#endif /* _SPX_CAM_PRESET_IFACE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
