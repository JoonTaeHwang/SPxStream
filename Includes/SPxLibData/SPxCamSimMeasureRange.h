/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamSimMeasureRage class - a class
*	for simulating the range measurement of a camera.
*
* Revision Control:
*   27/09/22 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAM_SIM_MEASURE_RANGE_H
#define _SPX_CAM_SIM_MEASURE_RANGE_H

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxCamMeasureRangeIface.h"

/* Forward declarations. */

class SPxCamSimMeasureRange : public SPxObj, public SPxCamMeasureRangeIface
{
public:
    SPxCamSimMeasureRange(void);
    virtual ~SPxCamSimMeasureRange(void);

    virtual int IsMeasuredRangeSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetMeasuredRange(double *rangesMetres,
					  unsigned int *numRanges) const;

protected:
    SPxErrorCode SetMeasuredRange(double *rangesMetres,
				  unsigned int numRanges);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxCamSimMeasureRange */

#endif /* _SPX_CAM_SIM_MEASURE_RANGE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
