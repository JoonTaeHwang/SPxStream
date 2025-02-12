/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxHistogramRenderer class which is used to render
 *   histogram processing area in a PPI window.
 *
 * Revision Control:
 *   30/11/18 v1.1   AGC	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_HISTOGRAM_RENDERER_H
#define _SPX_HISTOGRAM_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxRenderer.h"

/* Forward declare other classes required. */
class SPxRunProcess;

/*
 * Define our class.
 */
class SPxHistogramRenderer : virtual public SPxRenderer
{
public:
    /* Constructor and destructor. */
    SPxHistogramRenderer(SPxRunProcess *histRunProcess);
    virtual ~SPxHistogramRenderer(void);

private:
    SPxRunProcess *m_histRunProcess;

    SPxErrorCode renderAll(void);

}; /* SPxHistogramRenderer */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_HISTOGRAM_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
