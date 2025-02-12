/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamFocusIface class - an interface for
*	basic focus camera control.
*
* Revision Control:
*   10/08/18 v1.1    AGC	Add Set/GetFocus.
*
* Previous Changes:
*   02/07/18 1.0    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_FOCUS_IFACE_H
#define _SPX_CAM_FOCUS_IFACE_H

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

class SPxCamFocusIface
{
public:
    /* Construction and destruction. */
    SPxCamFocusIface() {}
    virtual ~SPxCamFocusIface() {}

    /* Absolute positioning. */
    virtual int IsAbsFocusCmdInProgress(void) const = 0;

    virtual int IsAbsFocusSupported(void) const = 0;
    virtual SPxErrorCode SetFocus(float rangeMetres) = 0;
    virtual SPxErrorCode GetFocus(float *rangeMetres) const = 0;
    virtual SPxErrorCode SetFocusRangeMetres(float /*minRangeMetres*/, float /*maxRangeMetres*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetFocusPos(UINT16 focusPos) = 0;
    virtual SPxErrorCode GetFocusPos(UINT16 *focusPos) const = 0;
    virtual SPxErrorCode GetFocusRange(UINT16 *minFocusPos, UINT16 *maxFocusPos) const = 0;

    /* Relative positioning. */
    virtual int IsRelFocusSupported(void) const = 0;
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt=FALSE) = 0;

    virtual int IsAutoFocusSupported(void) const = 0;
    virtual SPxErrorCode AutoFocus(int autoFocus) = 0;
    virtual int IsAutoFocusEnabled(void) const = 0;

}; /* SPxCamFocusIface */

#endif /* SPxCamFocusIface */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
