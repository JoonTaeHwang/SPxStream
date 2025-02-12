/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamMeasureRangeIface class - an interface for
*	basic range measurement camera control.
*
* Revision Control:
*   27/09/22 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAM_MEASURE_RANGE_IFACE_H
#define _SPX_CAM_MEASURE_RANGE_IFACE_H

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

class SPxCamMeasureRangeIface
{
public:
    /* Construction and destruction. */
    SPxCamMeasureRangeIface() {}
    virtual ~SPxCamMeasureRangeIface() {}

    virtual int IsMeasuredRangeSupported(void) const = 0;
    virtual SPxErrorCode MeasureRange(int numMeasurements=-1) = 0;
    virtual SPxErrorCode GetMeasuredRange(double *rangesMetres,
					  unsigned int *numRanges) const = 0;

}; /* SPxCamMeasureRangeIface */

#endif /* _SPX_CAM_MEASURE_RANGE_IFACE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
