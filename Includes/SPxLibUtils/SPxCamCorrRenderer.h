/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxCamCorrRenderer base class which supports the display
*   of symbols for points selected for camera correction,
*   but only via a derived class.
*
* Revision Control:
*   19/10/18 v1.6    AGC	Pass correction point as const pointer.
*
* Previous Changes:
*   17/09/18 1.5   BTB	Add multi-language support.
*   24/08/18 1.4   BTB	Change namespace name to avoid future conflicts.
*   23/08/18 1.3   BTB	Allow showing/hiding of points.
*   20/08/18 1.2   BTB	Give function unique name.
*   17/08/18 1.1   BTB	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_CAM_CORR_RENDERER_H
#define _SPX_CAM_CORR_RENDERER_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxCamCorrDefs.h"
/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxRenderer.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxCamCorrRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxCamCorrRenderer(void);
    virtual ~SPxCamCorrRenderer(void);

    SPxErrorCode SetPoint(unsigned int index, const SPxCamCorrectionNS::CorrectionPoint *point);
    SPxErrorCode ShowPoint(unsigned int index, int show);
    SPxErrorCode ClearAllPoints(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxCriticalSection m_mutex;

    SPxCamCorrectionNS::CorrectionPoint m_points[SPX_MAX_CAM_CORRECTION_POINTS];
    int m_showPoint[SPX_MAX_CAM_CORRECTION_POINTS];

    /*
     * Private functions.
     */

    virtual SPxErrorCode renderAll(void);
    SPxErrorCode renderCorrectionPoint(unsigned int index);

}; /* SPxCamCorrRenderer */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_CAM_CORR_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
