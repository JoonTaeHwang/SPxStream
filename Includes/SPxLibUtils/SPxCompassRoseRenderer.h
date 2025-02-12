/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxCompassRoseRenderer base class which supports the display
*   of a compass rose around a circular PPI window area,
*   but only via a derived class.
*
* Revision Control:
*   03/12/19 v1.2    AGC        Support destination clipping.
*
* Previous Changes:
*   01/11/19 1.1    BTB	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_COMPASS_ROSE_RENDERER_H
#define _SPX_COMPASS_ROSE_RENDERER_H

/*
 * Other headers required.
 */

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
class SPxCompassRoseRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxCompassRoseRenderer(void);
    virtual ~SPxCompassRoseRenderer(void);

    SPxErrorCode SetColour(UINT32 col);
    UINT32 GetColour(void) const { return m_colour; }

    SPxErrorCode SetInnerRadiusPixels(double radiusPixels);
    double GetInnerRadiusPixels(void) const { return m_innerRadiusPixels; }

    SPxErrorCode SetOuterRadiusPixels(double radius);
    double GetOuterRadiusPixels(void) const { return m_outerRadiusPixels; }

    SPxErrorCode SetClipDestination(int clipDestination);
    int GetClipDestination(void) const { return m_clipDestination; }

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

    UINT32 m_colour;
    double m_innerRadiusPixels;
    double m_outerRadiusPixels;
    int m_clipDestination;          /* Will destination be clipped from inner radius? */

    SPxCriticalSection m_mutex;

    /*
     * Private functions.
     */

    virtual SPxErrorCode renderAll(void);

}; /* SPxCompassRoseRenderer */

#endif /* _SPX_COMPASS_ROSE_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
