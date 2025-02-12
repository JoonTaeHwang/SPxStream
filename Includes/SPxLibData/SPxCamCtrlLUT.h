/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlLUT class - a class
*	for managing a camera zoom or focus LUT.
*
* Revision Control:
*   08/08/18 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAM_CTRL_LUT_H
#define _SPX_CAM_CTRL_LUT_H

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */

class SPxCamCtrlLUT : public SPxObj
{
public:
    SPxCamCtrlLUT(void);
    virtual ~SPxCamCtrlLUT(void);

    SPxErrorCode ReadFile(const char *filename);
    const char *GetFilename(void) const;
    const char *GetShortPath(void) const;
    const char *GetFullPath(void) const;
    unsigned int GetSize(void) const;
    float GetValueForCamPos(UINT16 camPos) const;
    UINT16 GetCamPosForValue(float value) const;
    SPxErrorCode GetLUTBounds(UINT16 *firstPos, UINT16 *lastPos,
			      float *firstVal, float *lastVal) const;

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    UINT16 getCamPosForValue(float value) const;
    UINT16 getCamPosForValueReverse(float value) const;

}; /* SPxCamCtrlLUT */

#endif /* _SPX_CAM_CTRL_LUT_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
