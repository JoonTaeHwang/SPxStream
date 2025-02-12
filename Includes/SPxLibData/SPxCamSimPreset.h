/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamSimPreset class - a class
*	for simulating the preset behaviour of a camera.
*
* Revision Control:
*   10/08/18 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAM_SIM_PRESET_H
#define _SPX_CAM_SIM_PRESET_H

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxCamPresetIface.h"

/* Forward declarations. */

class SPxCamSimPreset : public SPxObj, public SPxCamPresetIface
{
public:
    SPxCamSimPreset(void);
    virtual ~SPxCamSimPreset(void);

    virtual SPxErrorCode GoToPreset(UINT8 id);
    virtual SPxErrorCode GoToPreset(const char *preset);
    virtual SPxErrorCode GetLastPreset(UINT8 *id);
    virtual SPxErrorCode GetLastPreset(char *buf, unsigned int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxCamSimPreset */

#endif /* _SPX_CAM_SIM_PRESET_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
