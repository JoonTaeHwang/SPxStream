/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlSafran class - a library
*	class for Safran LRF control.
*
* Revision Control:
*   27/09/22 v1.4    AGC	Support specifying number of measurements to MeasureRange().
*
* Previous Changes:
*   02/03/22 1.3    AGC	Send TCP messages synchronously.
*   22/02/22 1.2    AGC	Fix cppcheck warnings.
*   18/02/22 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_SAFRAN_H
#define _SPX_CAM_CTRL_SAFRAN_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/*********************************************************************
*
*   Type/constant definitions
*
**********************************************************************/

/* Forward declarations. */

/*
 * Define our class.
 */
class SPxCamCtrlSafran : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlSafran(void);
    virtual ~SPxCamCtrlSafran(void);
    virtual int IsTcpSendAsync(void) const { return FALSE; }
    
    virtual int IsOpModeSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetOpMode(int opMode);
    virtual int GetOpMode(void) const;

    virtual int IsMeasuredRangeSupported(void) const { return TRUE; }
    virtual SPxErrorCode MeasureRange(int numMeasurements=-1);
    virtual SPxErrorCode GetMeasuredRange(double *rangeMetres) const;
    virtual SPxErrorCode GetMeasuredRange(double *rangesMetres, unsigned int *numRanges) const;
    
    virtual int IsLaserPointerSupported(void) const { return TRUE; }
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetLaserPointerArmed(int arm);
    virtual int IsLaserPointerArmed(void) const;
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetLaserPointerOn(int on);
    virtual int IsLaserPointerOn(void) const;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    SPxErrorCode sendMessage(const char *mesgID, unsigned int val);
    SPxErrorCode recvMessage(const char *mesgID,
			     char *respBuf,
			     unsigned int respBufLen,
			     unsigned int respLen);
    SPxErrorCode recvPrompt(void);
    SPxErrorCode validateChecksum(const char *msg);

}; /* SPxCamCtrlSafran */

#endif /* _SPX_CAM_CTRL_SAFRAN_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
