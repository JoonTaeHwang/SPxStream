/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxExtViewsRenderer base class which supports
*   displaying outlines of other view areas,
*   but only via a derived class.
*
* Revision Control:
*   25/11/19 v1.2    BTB	Fix build warning.
*
* Previous Changes:
*   25/11/19 1.1    BTB	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_EXT_VIEWS_RENDERER_H
#define _SPX_EXT_VIEWS_RENDERER_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
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
class SPxExtViewsRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxExtViewsRenderer(void);
    virtual ~SPxExtViewsRenderer(void);

    SPxErrorCode SetColour(UINT32 col);
    UINT32 GetColour(void) { return m_colour; }

    SPxErrorCode AddView(unsigned int index, SPxLatLong_t centreLL, double widthMetres, double heightMetres, const char *name=NULL);
    SPxErrorCode RemoveView(unsigned int index);
    SPxErrorCode ShowView(unsigned int index, int show);

    SPxErrorCode SetViewCentreLatLong(unsigned int index, SPxLatLong_t centreLL);
    SPxErrorCode SetViewWidthMetres(unsigned int index, double widthMetres);
    SPxErrorCode SetViewHeightMetres(unsigned int index, double heightMetres);
    SPxErrorCode SetViewName(unsigned int index, const char *name);

    using SPxRenderer::SetViewCentreLatLong;

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

    UINT32 m_colour;

    SPxCriticalSection m_mutex;

    /*
     * Private functions.
     */

    virtual SPxErrorCode renderAll(void);

}; /* SPxExtViewsRenderer */

#endif /* _SPX_EXT_VIEWS_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
