/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxModeSCoverageRendererWin class which supports 
*   the display of Mode-S coverage maps.
*
* Revision Control:
*   17/09/20 v1.1    AGC	Initial Version.
*
* Previous Changes:
*
**********************************************************************/
#ifndef _SPX_MODE_S_COVERAGE_RENDERER_WIN_H
#define _SPX_MODE_S_COVERAGE_RENDERER_WIN_H

/* We need the base classes. */
#include "SPxLibData/SPxModeSCoverageRenderer.h"
#include "SPxLibWin/SPxRendererWin.h"

/* Forward declarations. */
class SPxModeSCoverageRenderer;
class SPxRendererWin;
class SPxModeSCoverageMap;

class SPxModeSCoverageRendererWin : public SPxModeSCoverageRenderer, public SPxRendererWin
{
public:
    /* Constructor and destructor. */
    explicit SPxModeSCoverageRendererWin(SPxModeSCoverageMap *map);
    virtual ~SPxModeSCoverageRendererWin(void);

};

#endif /* _SPX_MODE_S_COVERAGE_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
