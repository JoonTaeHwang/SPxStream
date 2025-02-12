/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxModeSCoverageRendererD2D class which supports 
*   the display of Mode-S coverage maps.
*
* Revision Control:
*   17/09/20 v1.1    AGC	Initial Version.
*
* Previous Changes:
*
**********************************************************************/
#ifndef _SPX_MODE_S_COVERAGE_RENDERER_D2D_H
#define _SPX_MODE_S_COVERAGE_RENDERER_D2D_H

/* We need the base classes. */
#include "SPxLibData/SPxModeSCoverageRenderer.h"
#include "SPxLibWin/SPxRendererD2D.h"

/* Forward declarations. */
class SPxModeSCoverageRenderer;
class SPxRendererD2D;
class SPxModeSCoverageMap;

class SPxModeSCoverageRendererD2D : public SPxModeSCoverageRenderer, public SPxRendererD2D
{
public:
    /* Constructor and destructor. */
    explicit SPxModeSCoverageRendererD2D(SPxModeSCoverageMap *map);
    virtual ~SPxModeSCoverageRendererD2D(void);

};

#endif /* _SPX_MODE_S_COVERAGE_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
